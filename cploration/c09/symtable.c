#include "symtable.h"

struct Symbol* hashArray[SYMBOL_TABLE_SIZE]; 

struct Symbol* hashArray[SYMBOL_TABLE_SIZE]; 
struct Symbol* dummyItem;
struct Symbol* item;


int hash(char *str){
    unsigned long hash = 5381;
    int c = 0;

    while (c == *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % SYMBOL_TABLE_SIZE;
}


void symtable_insert(char* key, hack_addr addr) {
    
    struct Symbol *item = (struct Symbol*) malloc(sizeof(struct Symbol));
    item->addr = addr;  
    //item->name = (char*)malloc(strlen(key) + 1);
    strcpy(item->name, key);

    //get the hash 
    int hashIndex = hash(key);

    //move in array until an empty or deleted cell

    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->name != NULL) {
        //go to next cell
        ++hashIndex;
		
        //wrap around the table
        hashIndex %= SYMBOL_TABLE_SIZE;
    }
	
    hashArray[hashIndex] = item;

}

struct Symbol *symtable_find(char * key){

    //get the hash 
    int hashIndex = hash(key);  
	
    //move in array until an empty 
    while(hashArray[hashIndex] != NULL) {
	
        if(hashArray[hashIndex]->name == key)
        return hashArray[hashIndex]; 
			
        //go to next cell
        ++hashIndex;
		
        //wrap around the table
        hashIndex %= SYMBOL_TABLE_SIZE;
    }        
	
    return NULL;        
}

void symtable_display_table(){
    int i = 0;
	
    for(i = 0; i<SYMBOL_TABLE_SIZE; i++) {
	
        if(hashArray[i] != NULL) {
            printf(" (%s,%d)\n", hashArray[i]->name, hashArray[i]->addr);
         } else {
         //   printf(" ~~ ");
    }
	
    //printf("\n");
}
}

void symtable_print_labels(){
   int i = 0;    
   for(i = 0; i<SYMBOL_TABLE_SIZE; i++) {
        if(hashArray[i] != NULL){
            printf("{%s,%d}\n",hashArray[i]->name,hashArray[i]->addr);
        }
   }
} 
