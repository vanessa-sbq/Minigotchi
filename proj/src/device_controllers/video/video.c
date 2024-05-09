#include "video.h"
#include "video_macros.h"

static vbe_mode_info_t vbe_mem_info; // VBE mode info struct
static char *video_mem; // Frame buffer
static char *second_buffer; // Second frame buffer (for double buffering)
static int draw_on = 1; // Start drawing on back buffer
static unsigned int vram_size; // VRAM's size
static unsigned int vram_base; // VRAM's physical address


/**
 * @brief Function that helps go back to text mode
 */
void returnToTextMode() {
    if (vg_exit() != 0){
        panic("Error in vg_exit()\n");
    }
}


/**
 * @brief Function that gets information about a video mode.
 * @param mode video mode whose information we want to get
 */
int video_get_mode_information(uint16_t mode){
    if (vbe_get_mode_info(mode, &vbe_mem_info) != 0){
        panic("Error in vbe_get_mode_info()\n");
        returnToTextMode();
        return 1;
    }
    return 0;
}


/**
 * @brief Function that maps the video ram.
 */
int video_map_vram(){
    vram_size = video_get_vram_size();  // Size of one page
    vram_base = video_get_phys_addr();  // VRAM's physical address

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + 2*vram_size;
    int r;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){ // Allow memory mapping.
        panic("Error in video_map_vram, sys_privctl failed: %d\n", r);
        return 1;
    }

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size); // Map memory (first buffer)
    second_buffer = vm_map_phys(SELF, (void*) (mr.mr_base + vram_size), vram_size); // Map memory (second buffer)

    if(video_mem == MAP_FAILED || second_buffer == MAP_FAILED) {
        panic("Error in video_map_vram, vm_map_phys() failed\n");
        return 1;
    }

    return 0;
}


/**
 * @brief Function that sets the desired video mode
 * @param mode video mode to be set
 */
int video_set_graphics_mode(uint16_t mode){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); // zero the structure

    // Specify the appropriate register values
    r86.intno = VIDEO_BIOS_SERVICE; // BIOS video services
    r86.ah = VBE_CALL; // Set Video Mode function
    r86.al = VBE_SET_MODE; // mode
    r86.bx = mode | VIDEO_LINEAR_FRAMEBUF_MODE;

    // Make the BIOS call
    if(sys_int86(&r86) != OK) {
        printf("\tsys_int86() failed \n");
        return 1;
    }

    return 0;
}


/**
 * @brief Gets VRAM's physical base address
 */
unsigned int video_get_phys_addr(){
    return vbe_mem_info.PhysBasePtr;
}


/**
 * @brief Gets VRAM size
 */
unsigned int video_get_vram_size(){
    return (vbe_mem_info.XResolution * vbe_mem_info.YResolution) * ((vbe_mem_info.BitsPerPixel + 7) / 8);
}


/**
 * @brief Draws a pixel on the current back buffer
 * @param x x-coordinate of the pixel
 * @param y y-coordinate of the pixel
 * @param color Color of the pixel 
 */
int drawPixel(uint16_t x, uint16_t y, uint32_t color){
    unsigned bytes_per_pixel = (vbe_mem_info.BitsPerPixel + 7) / 8;
    uint32_t bytes_pp_mask = 0;
    for (uint32_t i = 0; i < vbe_mem_info.BitsPerPixel; i++){
        bytes_pp_mask |= BIT(i);
    }

    color &= bytes_pp_mask;
    if (x >= vbe_mem_info.XResolution ||  y >= vbe_mem_info.YResolution) return 0; 

    unsigned index = (vbe_mem_info.XResolution * y + x) * bytes_per_pixel;

    if (draw_on){
        if (memcpy(&second_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
    }
    else {
        if (memcpy(&video_mem[index], &color, bytes_per_pixel) == NULL) return 1;
    }

    return 0;
}


/**
 * @brief Draws a horizontal line on the current back buffer
 * @param x x-coordinate of the starting pixel
 * @param y y-coordinate of the line
 * @param len Length of the line
 * @param color Color of the line 
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (unsigned i = 0 ; i < len ; i++){
        if (drawPixel(x+i, y, color) != 0) return 1;
    }
    return 0;
}


/**
 * @brief Draws a rectangle on the current back buffer
 * @param x x-coordinate of the starting pixel
 * @param y y-coordinate of the starting pixel
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){    
    for (int i = 0; i < height; i++){
        if (vg_draw_hline(x, y + i, width, color) != 0){
            vg_exit();
            return 1;
        }
    }

    return 0;
}


/**
 * @brief Draws a pattern of rectangles on the current back buffer
 * @param no_rectangles Number of rectangles to draw
 * @param first Color of the first rectangle
 * @param step Step to determine next color
 */
int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step){
    if (no_rectangles == 0) return 0; // Draw no rectangles

    uint16_t width = vbe_mem_info.XResolution / no_rectangles;
    uint16_t height = vbe_mem_info.YResolution / no_rectangles;

    // Get bitmask for each component and shift it to the right for extraction
    uint8_t RFirst = (BIT(vbe_mem_info.RedMaskSize) - 1) & (first >> vbe_mem_info.RedFieldPosition);
    uint8_t GFirst = (BIT(vbe_mem_info.GreenMaskSize) - 1) & (first >> vbe_mem_info.GreenFieldPosition);
    uint8_t BFirst = (BIT(vbe_mem_info.BlueMaskSize) - 1) & (first >> vbe_mem_info.BlueFieldPosition);

    uint32_t cor = first;

    for (int i = 0; i < no_rectangles; i++){
        for (int j = 0; j < no_rectangles; j++){

            if (vbe_mem_info.MemoryModel != 0x06){ // Indexed mode
                cor = (first + (i * no_rectangles + j) * step) % (1 << vbe_mem_info.BitsPerPixel);
            }
            else{ // Direct mode
                uint8_t R = (RFirst + j * step) % (1 << vbe_mem_info.RedMaskSize);
                uint8_t G = (GFirst + i * step) % (1 << vbe_mem_info.GreenMaskSize);
                uint8_t B = (BFirst + (j + i) * step) % (1 << vbe_mem_info.BlueMaskSize);

                cor = (R << vbe_mem_info.RedFieldPosition) | (G << vbe_mem_info.GreenFieldPosition) | (B << vbe_mem_info.BlueFieldPosition);
            }

            vg_draw_rectangle(width * j, height * i, width, height, cor);

        }
    }
    return 0;
}


/**
 * @brief Draws a pixmap on the current back buffer
 * @param x x-coordinate of the starting pixel
 * @param y y-coordinate of the starting pixel
 * @param map Pixmap
 * @param pixmap_and_metadata Pixmap's metadata
 */
int vg_draw_pixmap(uint16_t x, uint16_t y, uint8_t* map, xpm_image_t pixmap_and_metadata){
    int width = pixmap_and_metadata.width;
    int height = pixmap_and_metadata.height;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (drawPixel(j + x, i + y, map[j + i * width]) != 0){
                vg_exit();
                return 1;
            }
        }
    }
    return 0;
}


/**
 * @brief Clears the screen on the current back buffer
 */
void vg_clear_screen(){
    if (draw_on) memset(second_buffer,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
    else memset(video_mem,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
}


/**
 * @brief Makes a BIOS call to set the new display start (Page flip) 
 */
int vg_page_flip(){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));	// zero the structure

    unsigned int v_res = 0;  
    if (draw_on) v_res = vbe_mem_info.YResolution;   

    // Specify the appropriate register values
    r86.intno = VIDEO_BIOS_SERVICE; // BIOS video services
    r86.ah = 0x4f; 
    r86.al = 0x07; // Set/Get display start
    r86.bh = 0x00; // Reserved and must be 00h
    r86.bl = 0x80; // Set Display CRTC Start during Vertical Retrace
    r86.cx = 0;  // Starting pixel
    r86.dx = v_res; // Starting scan line

    // Make the BIOS call
    if(sys_int86(&r86) != OK) {
        printf("\tsys_int86() failed \n");
        return 1;
    }

    draw_on = !draw_on; // "Flip" the page

    return 0;
}


// TODO: Remove -> this function is for testing page flipping (Always call vg_page_flip() when using double buffering)
/**
 * @brief Draw an entire page on the back buffer
 */
int vg_draw_page(int i){    
    /* if (draw_on){
        vg_clear_screen();
        vg_draw_rectangle(10, 0, 100, 50, 0x2); // Draw rectangle on B2
    }
    else {
        vg_clear_screen();
        vg_draw_rectangle(100, 100, 100, 50, 0x3); // Draw rectangle on B1
    } */

    vg_clear_screen();
    vg_draw_rectangle(i, 0, 100, 50, 0x2); // Draw rectangle on B2
    vg_page_flip();

    return 0;
}
