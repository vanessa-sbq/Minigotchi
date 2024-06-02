#include <lcom/lcf.h>
#include "video.h"
#include "video_macros.h"

static char *video_mem; // Process (virtual) address to which VRAM is mapped
static vbe_mode_info_t vbe_mem_info;

int video_get_mode_information(uint16_t mode){
	if (vbe_get_mode_info(mode, &vbe_mem_info) != 0){
		printf("Error in vbe_get_mode_info()\n");  
		if (vg_exit() != 0){
            printf("Erro in vg_exit()\n");
            return 1;
        }
		return 1;
	}   

    return 0;
}

int video_map_vram(){
    unsigned int vram_base = video_get_phys_addr();  // VRAM's physical address
    unsigned int vram_size = video_get_vram_size();  // VRAM's size, but you can use the frame-buffer size, instead

    struct minix_mem_range mr;
		mr.mr_base = (phys_bytes) vram_base;	
		mr.mr_limit = mr.mr_base + vram_size; 
    int r;  

	// Allow memory mapping (Give Minix permission to map the desired address range)
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);
	}

    // Map memory
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED) {
		panic("couldn't map video memory");
	}

    return 0;
}

int video_set_graphics_mode(uint16_t mode){
	reg86_t r86;
	memset(&r86, 0, sizeof(r86));	// zero the structure

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

unsigned int video_get_phys_addr(){
    return vbe_mem_info.PhysBasePtr;
}
unsigned int video_get_vram_size(){
    return (vbe_mem_info.XResolution * vbe_mem_info.YResolution) * ((vbe_mem_info.BitsPerPixel + 7) / 8);  
}

int drawPixel(uint16_t x, uint16_t y, uint32_t color){
	
	unsigned bytes_per_pixel = (vbe_mem_info.BitsPerPixel + 7) / 8;

	uint32_t bytes_pp_mask = 0;
	for (uint32_t i = 0; i < vbe_mem_info.BitsPerPixel; i++){
		bytes_pp_mask |= BIT(i);
	}

	color &= bytes_pp_mask;

	if (x < 0 || y < 0 ||  x >= vbe_mem_info.XResolution ||  y >= vbe_mem_info.YResolution) return 0;

	unsigned index = (vbe_mem_info.XResolution * y + x) * bytes_per_pixel; 

	if (memcpy(&video_mem[index], &color, bytes_per_pixel) == NULL) return 1;

	return 0;    
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
	for (unsigned i = 0 ; i < len ; i++){
    if (drawPixel(x+i, y, color) != 0) return 1;
	}
	return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
	for (int i = 0; i < height; i++){
		if (vg_draw_hline(x, y + i, width, color) != 0){
 			vg_exit();
			return 1;
		}
	}

	return 0;
}

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

void vg_clear_screen(){
	memset(video_mem,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
}
