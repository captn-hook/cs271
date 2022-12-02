#include "symtable.h"

struct Symbol* hashArray[SYMBOL_TABLE_SIZE]; 

struct Symbol* hashArray[SYMBOL_TABLE_SIZE]; 
struct Symbol* dummyItem;
struct Symbol* item;


int hash(char *str){
    unsigned long hash = 5381;
    int c = 0;

    while ( ( c = *str++ ) != 0 )
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % SYMBOL_TABLE_SIZE;
}


void symtable_insert(char* key, hack_addr addr) {

    if (strcmp(key, "screen.setcolor") == 0) {
        printf("symtable_insert: %s\n", key);
    }
    
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

    //printf("inserting %s at %d\n", key, hashIndex);
   // printf("hashIndex: %s\n", hashArray[hashIndex]->name);
	
    hashArray[hashIndex] = item;

}

struct Symbol *symtable_find(char * key){

    //get the hash 
    int hashIndex = hash(key);  

   // printf("looking for %s at %d\n", key, hashIndex);
    //printf("hashIndex: %d, hasharray: %s\n", hashIndex, hashArray[hashIndex]->name);

    //move in array until an empty 
    while(hashArray[hashIndex] != NULL) {

       // printf("hashIndex: %d, key: %s, name: %s\n", hashIndex, key, hashArray[hashIndex]->name);
	
        if(strcmp(hashArray[hashIndex]->name, key) == 0) {
            
            //printf("found: %s\n", key);
     //     printf("return hashIndex: %d, hasharray: %s\n", hashIndex, hashArray[hashIndex]->name);

            return hashArray[hashIndex]; 
            
        }
			
        //go to next cell
        ++hashIndex;
		
        //wrap around the table
        hashIndex %= SYMBOL_TABLE_SIZE;
    }        

    if (strcmp(key, "screen.setcolor") == 0) {
        printf("screen.setcolor not found\n");
    printf("not found: %s\n", key);    
    printf("hashIndex: %d\n", hashIndex);

    printf("hashArray[hashIndex]: %s\n", hashArray[hashIndex]->name);
    }
/*
    printf("hashArray[hashIndex + 1]: %s\n", hashArray[hashIndex + 1]->name);


    printf("hashArray[hashIndex + 2]: %s\n", hashArray[hashIndex + 2]->name);


    printf("hashArray[hashIndex + 3]: %s\n", hashArray[hashIndex + 3]->name);
*/
 //   printf("nuffin");
    return NULL;        
}

hack_addr symtable_get_addr(char *key){
    int hashIndex = hash(key);
    return hashArray[hashIndex]->addr;
};

bool symtable_ask(char *key){
    int hashIndex = hash(key);
    return hashArray[hashIndex] != NULL;
};

void symtable_display_table(){
    int i = 0;
	
    for(i = 0; i<SYMBOL_TABLE_SIZE; i++) {
	
        if(hashArray[i] != NULL) {
            printf(" (%s,%d)\n", hashArray[i]->name, hashArray[i]->addr);
         } else {

    }
	
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
