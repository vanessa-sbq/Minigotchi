#include "database.h"

const char* filePath = "/usr/save.txt";

/**
 * @brief Creates a new database 
 */
Database *new_database(){
    Database *database = malloc(sizeof(*database));

    if (database == NULL) return NULL;

    database->minigotchi_name = "Default";
    database->minigotchi_hunger = 0;
    database->minigotchi_happiness = 0;
    database->coins = 0;

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
    char name[100];  // FIXME: Limit
    int hunger, happiness, coins;

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
    return 0;
}

/**
 * @brief Check if the player data file already exists. Creates the save file in case it doesn't exist
 */
bool database_check_file_exists(){
    if (access(filePath, F_OK) == -1) {
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

        fclose(file);

        printf("File '%s' created with default values.\n", filePath);  // TODO: Remove (DEBUG)
        return false;
    } 
    printf("File '%s' exists.\n", filePath); // TODO: Remove (DEBUG)
    return true;
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
