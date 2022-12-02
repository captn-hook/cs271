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

//	printf("strip: %s\n", s);

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
int parse(FILE * file, instruction *instructions){

	instruction instr;
	
	char line[MAX_LINE_LENGTH] = {0};

	char label[MAX_LABEL_LENGTH] = {0};

	unsigned int line_num = 0;

	unsigned int instr_num = 0;

	add_predefined_symbols();

	while (fgets(line, sizeof(line), file)) {

		line_num++;

	//	printf("line number: %d\n" , line_num);

		if (instr_num > MAX_INSTRUCTIONS) {
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}

		strip(line);

		//printf("line: %s\n", line);

		if (*line == '\0') {

		} else {

		if (is_Atype(line)) {

				instr.type = Atype;

				if (!parse_A_instruction(line, &instr.instr.a)){
    				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
 				}


				if (instr.instr.a.is_addr) {
					printf("A: %d\n", instr.instr.a.value.addr);
				} else {
					printf("A: %s\n", *instr.instr.a.value.symbol);					
				}


			} else if (is_label(line)) {

				extract_label(line, label);

				
				//printf("label: %s\n", label);

				if (!isalpha(label[0])) {
					//test driven development ig
					strip2(line);
					exit_program(EXIT_INVALID_LABEL, line_num, line);
				} else if (symtable_find(label) != NULL) {
					exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
				} else {

				symtable_insert(label, instr_num);

				//printf("A: %s\n", label);

				continue;
				}
				

			} else {

				instr.type = Ctype;
				char tmp_line[MAX_LINE_LENGTH] = {0};

				strcpy(tmp_line, line);

				parse_C_instruction(tmp_line, &instr.instr.c);

				if (instr.instr.c.dest == -1) {
					exit_program(EXIT_INVALID_C_DEST, line_num, line);
				} else if (instr.instr.c.comp == -1) {
					exit_program(EXIT_INVALID_C_COMP, line_num, line);
				} else if (instr.instr.c.jump == -1) {
					exit_program(EXIT_INVALID_C_JUMP, line_num, line);
				}

				if (instr.instr.c.a == 0)
					printf("C: d=%d, c=%d, j=%d\n", instr.instr.c.dest, instr.instr.c.comp, instr.instr.c.jump);
				else {
					//clear last bit of int 
					//printf("%d\n", instr.instr.c.comp);
					int newC;
					if (abs(instr.instr.c.comp) < 16) {
						newC = instr.instr.c.comp & 0xF;
					} else if (abs(instr.instr.c.comp) < 64) {
						newC = instr.instr.c.comp & 0x3F;
					} 
					printf("C: d=%d, c=%d, j=%d\n", instr.instr.c.dest, newC, instr.instr.c.jump);
				}
				
			}


		instructions[instr_num++] = instr;

		}


	}

	return instr_num;
	
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
	
	if (s == s_end) {
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

void parse_C_instruction(char *line, c_instruction *instr) {
	
	char *temp;
	char *jump;
	char *comp;
	char *dest;

	//printf("line: %s\n", line);
	
	//split temp / jump
	temp = strtok(line, ";");
	//split dest / comp (if comp == NULL, then dest == comp
	jump = strtok(NULL, ";");

	if (jump != NULL) {
			
		dest = strtok(line, "=");

		comp = strtok(NULL, "=");

	} else {
		dest = strtok(line, "=");

		comp = strtok(NULL, "=");
	}

	//printf("dest: %s\n", dest);

	int a = 0;
	if (comp == NULL) {
		instr->dest = str_to_destid("0");
		instr->comp = str_to_compid(dest, &a);
		instr->jump = str_to_jumpid(jump);
	} else {
		instr->dest = str_to_destid(dest);
		instr->comp = str_to_compid(comp, &a);
		instr->jump = str_to_jumpid(jump);
	}

	instr->a = a;

}

 void assemble(const char * file_name, instruction* instructions, int num_instructions) {

	//append .hack to file name
	char *hack_file_name = (char*)malloc(strlen(file_name) + 5);
	strcpy(hack_file_name, file_name);
	strcat(hack_file_name, ".hack");

	FILE *out = fopen(hack_file_name, "w");

	int var_addr = 16;

	for (int i = 0; i < num_instructions; i++) {
		instruction instr = instructions[i];
		opcode op;

		if (instr.type == 0) {
			//a instruction
			if (instr.instr.a.is_addr) {
				op = (opcode)instr.instr.a.value.addr;

				//fprintf(out, "%016d", instr.instr.a.value.addr);
			} else {
				
				if (symtable_ask(*instr.instr.a.value.symbol)) {
					op = symtable_get_addr(*instr.instr.a.value.symbol);
			//		printf("FS:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));

					}
				else {
					op = (opcode)var_addr;
					symtable_insert(*instr.instr.a.value.symbol, var_addr++);

			//		printf("NS:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));

				}
				//fprintf(out, "%016d", symtable_get(*instr.instr.a.value.symbol));
			}
		} else {
			op = instruction_to_opcode(instr.instr.c);

		}
		//print opcode to binary
		//printf("FINAL==============:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));
		fprintf(out, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));
	}
 };

 opcode instruction_to_opcode(c_instruction instr) {
	opcode op = 0;

	op |= (7 << 13);
	op |= (instr.a << 12);
	op |= (instr.comp << 6);
	op |= (instr.dest << 3);
	op |= (instr.jump);

	return op;
 
 };