/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $YOUR_NAME$
 * [TERM] FALL $YEAR$
 * 
 ****************************************/
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

	unsigned int line_num = 0;

	while (fgets(line, sizeof(line), file)) {

		strip(line);

		if (*line == NULL) {

		} else {

		char inst_type;

		if (is_Atype(line)) {
			inst_type = 'A';
		} else if (is_label(line)) {
			inst_type = 'L';
		} else {
			inst_type = 'C';
		}

		printf("%c  %s\n", inst_type, line);
		
		}

		line_num++;
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