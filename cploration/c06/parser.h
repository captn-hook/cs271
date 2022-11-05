/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $YOUR_NAME$
 * [TERM] FALL $YEAR$
 * 
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH  (MAX_LINE_LENGTH - 2)

typedef int16_t hack_addr;
typedef int16_t opcode;

/** function prototypes **/
char *strip(char *s);

void parse(FILE *);

bool is_Atype(const char *);

bool is_label(const char *);

char *extract_label(const char *, char*);

/*redundant
bool is_Ctype(const char *); */