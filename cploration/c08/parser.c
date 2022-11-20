#include "parser.h"

enum instr_type {
	invalid = -1,
	Atype,
	Ctype
};

typedef struct {
   opcode a:1;
   opcode comp:7;
   opcode dest:3;
   opcode jump:3;
} c_instruction;

typedef struct {
   opcode a:1;
   hack_addr addr:15;
} a_instruction;

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	

	unsigned int x = strlen(s) + 1;

	char s_new[x];

	unsigned int line_num = 0;

	for (char *s2 = s; *s2; s2++) {
		if (*s2 == '/' && *(s2 + 1) == '/') {
			break;
		} else if (!isspace(*s2)) {
			s_new[line_num++] = *s2;
		}		
	}

	s_new[line_num] = '\0';

	strcpy(s, s_new);
	
    return s;	
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE * file){
	
	char line[MAX_LINE_LENGTH] = {0};

	char label[MAX_LABEL_LENGTH] = {0};

	unsigned int line_num = 0;

	unsigned int instr_num = 0;

	while (fgets(line, sizeof(line), file)) {

		line_num++;

		if (instr_num > MAX_INSTRUCTIONS) {
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}

		strip(line);

		if (*line == NULL) {

		} else {

			char inst_type;

		if (is_Atype(line)) {

				inst_type = 'A';

				//printf("%c  %s\n", inst_type, line);

			} else if (is_label(line)) {

				extract_label(line, label);

				if (isalpha(label[0])) {
					exit_program(EXIT_INVALID_LABEL, line_num, line);
				}

				if (symtable_find(label) != NULL) {
					exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
				}

				symtable_insert(label, instr_num);

				continue;

				inst_type = 'L';

				//printf("%c  %s\n", inst_type, label);

			} else {

				inst_type = 'C';

				//printf("%c  %s\n", inst_type, line);

			}

				printf("%u: %c  %s\n", instr_num, inst_type, line);
			
		}

		instr_num++;

	}
	
}


//Function: is_Atype
bool is_Atype(const char *line) {
	//if line starts with @, return true
	if (*line == '@') {
		return true;
	} else {
		return false;
	}
}

bool is_label(const char *line) {
	//if line starts and ends with (), return true
	if (*line == '(' && *(line + strlen(line) - 1) == ')') {
		return true;
	} else {
		return false;
	}
}

char *extract_label(const char *line, char* label) {

	//take the line and copy it into label, but without the parentheses
	for (int i = 1; i < strlen(line) - 1; i++) {
		*(label + i - 1) = *(line + i);
	}

	//get last index from line and set to terminator
	*(label + strlen(line) - 2) = '\0';

	return label;

}