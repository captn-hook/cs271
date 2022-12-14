#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#include "error.h"
#include "symtable.h"

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH  (MAX_LINE_LENGTH - 2)

#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS

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

#endif