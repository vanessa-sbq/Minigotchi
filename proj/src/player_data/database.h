#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <lcom/lcf.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct{
    char* minigotchi_name;
    int minigotchi_hunger;
    int minigotchi_happiness;
    int coins;
} Database;

Database *new_database();
int database_load_from_file(Database *database);
int database_save_to_file(Database *database);
bool database_check_file_exists();
int database_delete_file();

#endif /* _DATABASE_H_ */
