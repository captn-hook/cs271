#include "parser.h"

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


char *strip2(char *s){	

	unsigned int x = strlen(s) + 1;

	char s_new[x];

	unsigned int line_num = 0;

	for (char *s2 = s; *s2; s2++) {
		if (*s2 == '/' && *(s2 + 1) == '/') {
			break;
		} else if (*s2 != '(' && *s2 != ')') {
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

	instruction instr;
	
	char line[MAX_LINE_LENGTH] = {0};

	char label[MAX_LABEL_LENGTH] = {0};

	unsigned int line_num = 0;

	unsigned int instr_num = 0;

	add_predefined_symbols();

	while (fgets(line, sizeof(line), file)) {

		line_num++;

		if (instr_num > MAX_INSTRUCTIONS) {
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}

		strip(line);

		if (*line == '\0') {

		} else {

		if (is_Atype(line)) {

				instr.type = Atype;

				if (!parse_A_instruction(line, &instr.instr.a)){
    				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
 				}

				//symtable_insert(instr.instr.a.value.symbol, instr_num);
				//printf("%c  %s\n", instr.instr.a.value.symbol, line);

			} else if (is_label(line)) {

				extract_label(line, label);

				if (!isalpha(label[0])) {
					//test driven development ig
					strip2(line);
					exit_program(EXIT_INVALID_LABEL, line_num, line);
				} else if (symtable_find(label) != NULL) {
					exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
				} else {

				symtable_insert(label, instr_num);

				continue;
				}
				
			
				//printf("%c  %s\n", inst_type, label);

			} else {

				instr.type = Ctype;

				//printf("%c  %s\n", inst_type, line);

			}

				//printf("%u: %c  %s\n", instr_num, inst_type, line);


		instr_num++;
			
		}


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

void add_predefined_symbols() {
	for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
		predefined_symbol symbol = predefined_symbols[i];
		symtable_insert(symbol.name, symbol.value);
	}
};

bool parse_A_instruction(const char *line, a_instruction *instr) {

	char *s = (char*)malloc(strlen(line));

	strcpy(s, line + 1);

	char *s_end = NULL;

	long result = strtol(s, &s_end, 10);

	//printf("%s\n", s_end);

	if (strcmp(s, s_end)) {
		//not a number
		*instr->value.symbol = (char*)malloc(strlen(line));

		strcpy(*instr->value.symbol, s);

		instr->is_addr = false;

	} else if (*s_end != 0) {
		return false;	
	} else {
		instr->value.addr = result;
		instr->is_addr = true;
	}

	return true;
}