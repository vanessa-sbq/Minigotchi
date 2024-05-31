#include "database.h"

const char* filePath = "/usr/save.txt";
static Database *_database;

Database* getDatabase() {
    return _database;
}

void setDatabase(Database* database) {
    _database = database;
}

/**
 * @brief Creates a new database 
 */
Database *new_database(){
    Database *database = malloc(sizeof(*database));

    if (database == NULL) return NULL;
    
    int food_array[] = {0,0,0,0,0,0,0,0,0}; 

    database->minigotchi_name = "Default";
    database->minigotchi_hunger = 1000;
    database->minigotchi_happiness = 1000; 
    database->coins = 0;
    memcpy(database->food_array, food_array, sizeof(food_array));


    _database = database;
    return database;
}

/**
 * @brief Loads the player data from the save file specified in filePath 
 */
int database_load_from_file(Database *database){
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Variables to store the data
    char name[100]; 
    int hunger, happiness, coins;
    int food_array[9];

    // Read Minigotchi name
    if (fgets(name, sizeof(name), file) != NULL) {
        printf("Name: %s", name);
    } else {
        perror("Error reading minigotchi name");
        fclose(file);
        return 1;
    }

    // Read Minigotchi hunger status
    if (fscanf(file, "%d", &hunger) == 1) {
        printf("Hunger: %d\n", hunger);
    } else {
        perror("Error reading hunger status");
        fclose(file);
        return 1;
    }

    // Read Minigotchi happiness status
    if (fscanf(file, "%d", &happiness) == 1) {
        //database_set_happiness(_database, 2);
        printf("Happiness: %d\n", happiness);
    } else {
        perror("Error reading happiness status");
        fclose(file);
        return 1;
    }

    // Read player coins
    if (fscanf(file, "%d", &coins) == 1) {
        printf("Coins: %d\n", coins);
    } else {
        perror("Error reading coins");
        fclose(file);
        return 1;
    }

    // Read food array
    for (int i = 0; i < 9; i++){
        if (fscanf(file, "%d", &food_array[i]) == 1) {
            printf("Food %u: %d\n", i, food_array[i]);
        } else {
            printf("Error reading food %u\n", i);
            fclose(file);
            return 1;
        }
    }

    database_set_minigotchiName(database, name);
    database_set_happiness(database, happiness);
    database_set_hunger(database, hunger);
    database_set_coins(database, coins);
    //int food_debug_array[] = {2, 3, 1, 0, 0, 0, 0, 0, 0};  // TODO: remove
    //database_set_foodArray(database, food_debug_array);   // TODO: remove
    database_set_foodArray(database, food_array);

    fclose(file);
    printf("File 'save.txt' created in '/usr' directory.\n");  // TODO: Remove (DEBUG)

    return 0;
}

/**
 * @brief Write the player data to the file to resume from where he started, the next time he opens the game 
 */
int database_save_to_file(Database *database){
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "%s\n", database->minigotchi_name);
    fprintf(file, "%d\n", database->minigotchi_hunger);
    fprintf(file, "%d\n", database->minigotchi_happiness);
    fprintf(file, "%d\n", database->coins);
    for (int i = 0; i < 9; i++){
        fprintf(file, "%d\n", database->food_array[i]);
    }

    // TODO: REMOVE (DEBUG)
    printf("\nSaving...\n");
    printf("%s\n", database->minigotchi_name);
    printf("%d\n", database->minigotchi_hunger);
    printf("%d\n", database->minigotchi_happiness);
    printf("%d\n", database->coins);
    for (int i = 0; i < 9; i++){
        printf("%d\n", database->food_array[i]);
    }

    return 0;
}

/**
 * @brief Check if the player data file already exists. Creates the save file in case it doesn't exist
 */
bool database_check_file_exists(){ // TODO: Ref
    if (access(filePath, F_OK) != 0) {
        printf("File '%s' does not exist.\n", filePath);  // TODO: Remove (DEBUG)

        FILE *file = fopen(filePath, "w");
        if (file == NULL) {
            perror("Error creating file");
            return false;
        }

        // Write default values to the file
        fprintf(file, "Mini\n");    // Default name
        fprintf(file, "%d\n", 1000);  // Default hunger
        fprintf(file, "%d\n", 1000);  // Default happiness
        fprintf(file, "%d\n", 0);  // Default coins
        for (int i = 0; i < 9; i++){
            fprintf(file, "%d\n", 0);  // Default food
        }

        fclose(file);

        printf("File '%s' created with default values.\n", filePath);  // TODO: Remove (DEBUG)
        return false;
    } 
    else{
        printf("File '%s' exists.\n", filePath); // TODO: Remove (DEBUG)
        return true;
    }
    return false;
}

/**
 * @brief Deletes the player save state (in case new game creation is cancelled)
 */
int database_delete_file(){
    if (remove(filePath) != 0) {
        printf("Error deleting save file");
        return 1;
    }
    printf("File '%s' deleted successfully.\n", filePath);  // TODO: Remove (DEBUG)
    return 0;
}

int database_get_hunger(Database *database) {
    return database->minigotchi_hunger;
}
int database_get_happiness(Database *database) {
    return database->minigotchi_happiness;
}
int database_get_coins(Database *database) {
    return database->coins;
}
char* database_get_minigotchiName(Database *database) {
    return database->minigotchi_name;
}

int* database_get_foodArray(Database *database){
    return database->food_array;
}


void database_set_foodArray(Database *database, int* food_array){
    memcpy(database->food_array, food_array, sizeof(database->food_array));
}

void database_set_hunger(Database *database, int value) {
    database->minigotchi_hunger = value;
}
void database_set_happiness(Database *database, int value) {
    database->minigotchi_happiness = value;
}
void database_set_coins(Database *database, int value) {
    database->coins = value;
}
void database_set_minigotchiName(Database *database, char* value) {
    database->minigotchi_name = value;
}

/**
 * @brief Searches for an empty slot on the hotbar and in case the hotbar is full doesn't add it
 */
void database_add_food_to_array(Database *database, int food_item){
    for (int i = 0; i < 9; i++){
        if (database->food_array[i] == 0){
            database->food_array[i] = food_item;
            return;
        }
    }
}
