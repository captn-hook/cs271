#include <stdint.h>

#define SYMBOL_TABLE_SIZE 1000
#define MAX_NAME_SIZE 100

typedef int16_t hack_addr;

struct Symbol 
{
    char name[MAX_NAME_SIZE];
    hack_addr addr;
};

int hash(char *str);

struct Symbol *symtable_find(char * key);

void symtable_insert(char* key, hack_addr addr);

void symtable_display_table();

void symtable_print_labels();