#include "FunctionHeaders.h"


int intermediate_index = 0;
struct intermediate_lang **intermediate_table = NULL;

int symbol_index = 0;
struct symbol_table **symbol_tab = NULL;

int blocks_index = 0;
struct blocks_table **block_tab = NULL;


void const_func(char(*tokens)[10], int *memory, int *memory_index){

	if (symbol_index == 0){
		symbol_tab[symbol_index]->address = variable_memory_start;
		strcpy(symbol_tab[symbol_index]->variable_name, tokens[1]);
		symbol_tab[symbol_index]->size = const_variable_size;
	}
	else{
		strcpy(symbol_tab[symbol_index]->variable_name, tokens[1]);
		symbol_tab[symbol_index]->size = const_variable_size;
		if (symbol_tab[symbol_index - 1]->size != 0)
			symbol_tab[symbol_index]->address = symbol_tab[symbol_index - 1]->address + symbol_tab[symbol_index - 1]->size;
		else
			symbol_tab[symbol_index]->address = symbol_tab[symbol_index - 1]->address + 1;
	}
	symbol_index++;
	memory[*memory_index] = atoi(tokens[3]);
	(*memory_index)++;
}

void data_func(char (*tokens)[10],int *memory,int *memory_index){
	int i = 0, size = 0;;
	char variable_name[variable_length];
	if (symbol_index == 0)
	{
		symbol_tab[symbol_index]->address = variable_memory_start;
		
		while (tokens[1][i] != '\0' && tokens[1][i]!='['){
			variable_name[i] = tokens[1][i];
			i++;
		}
		variable_name[i] = '\0';
		strcpy(symbol_tab[symbol_index]->variable_name,variable_name); // entering variable name
		if (tokens[1][i] == '['){
			while (tokens[1][i] != ']')
				size = size * 10 + tokens[1][i] - '0';
			i++;
		}
		if (size == 0)
			symbol_tab[symbol_index]->size = 1;
		else
			symbol_tab[symbol_index]->size = size;
		*memory_index = symbol_tab[symbol_index]->address + symbol_tab[symbol_index]->size;
		symbol_index++;
		return;
	}
	else{
		while (tokens[1][i] != '\0' && tokens[1][i] != '['){
			variable_name[i] = tokens[1][i];
			i++;
		}
		variable_name[i] = '\0';
		strcpy(symbol_tab[symbol_index]->variable_name, variable_name); // entering variable name
		if (tokens[1][i] == '['){
			i++;
			while (tokens[1][i] != ']')
			{
				size = size * 10 + tokens[1][i] - '0';
				i++;
			}
		}
		if (size == 0)
			symbol_tab[symbol_index]->size = 1;
		else
			symbol_tab[symbol_index]->size = size;
	
		if (symbol_tab[symbol_index - 1]->size != 0)
			symbol_tab[symbol_index]->address = symbol_tab[symbol_index - 1]->address + symbol_tab[symbol_index - 1]->size;
		else
			symbol_tab[symbol_index]->address = symbol_tab[symbol_index - 1]->address + 1;

		*memory_index = symbol_tab[symbol_index]->address + symbol_tab[symbol_index]->size;
		symbol_index++;
		return;
	}
}

int generate_opcode(char *instruction){
	if (strcmp(instruction, "MOV")==0)
		return 1;
	if (strcmp(instruction, "ADD")==0)
		return 3;
	if (strcmp(instruction, "SUB")==0)
		return 4;
	if (strcmp(instruction, "MUL")==0)
		return 5;
	if (strcmp(instruction, "JUMP")==0 || strcmp(instruction, "ELSE") == 0)
		return 6;
	if (strcmp(instruction, "IF") == 0)
		return 7;
	if (strcmp(instruction, "EQ") == 0)
		return 8;
	if (strcmp(instruction, "LT") == 0)
		return 9;
	if (strcmp(instruction, "GT") == 0)
		return 10;
	if (strcmp(instruction, "LTEQ") == 0)
		return 11;
	if (strcmp(instruction, "GTEQ") == 0)
		return 12;
	if (strcmp(instruction, "PRINT") == 0)
		return 13;
	if (strcmp(instruction, "READ") == 0)
		return 14;
	if (strcmp(instruction, "ENDIF") == 0)
		return 15;
	if (strcmp(instruction, "END") == 0)
		return 16;
}

int getAddress(char *variable_name){

	int i = 0, is_array = 0, array_index = 0;
	char temp[variable_length];

	if (variable_name[1] == 'X' && variable_name[0] >= 'A' && variable_name[0] <= 'H'){  // register or not
		return variable_name[0] - 'A';
	}
	else{ // variable => array or noraml variable
		while (variable_name[i] != '\0'){
			if (variable_name[i] == '['){ // array
				sscanf(variable_name, "%[^[]", temp);
				is_array = 1;
				i++;
				while (variable_name[i] != ']'){
					array_index = array_index * 10 + variable_name[i] - '0';
					i++;
				}
				break;
			}
			i++;
		}
		if (is_array == 0){
			sscanf(variable_name, "%[^*]", temp);
		}
	}
	strcpy(variable_name, temp);
	for (int i = 0; i < symbol_index; i++){
		if (strcmp(symbol_tab[i]->variable_name, variable_name) == 0)
		{
			if (is_array)
				return symbol_tab[i]->address + array_index;
			else
				return symbol_tab[i]->address;
		}
	}
	return -1; // variable not present
}

void mov_func(char *param,int instruction_no){
	char dest[variable_length], src[variable_length];
	/* get the first token */
	char *token;

	token = strtok(param, ", ");
	strcpy(dest, token);

	token = strtok(NULL, ", ");
	strcpy(src, token);

	// instruction table //

	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	if (dest[1] == 'X' && dest[0] >= 'A' && dest[0] <= 'H'){ // destination is register
		intermediate_table[intermediate_index]->opcode = 2;
		intermediate_table[intermediate_index]->parameters[0] = getAddress(dest);
		intermediate_table[intermediate_index]->parameters[1] = getAddress(src);
		intermediate_table[intermediate_index]->parameters[2] = -1;  // to run the for loop
	}
	else{
		intermediate_table[intermediate_index]->opcode = 1;   // destination is memory // i.e register to memory
		intermediate_table[intermediate_index]->parameters[0] = getAddress(dest);
		intermediate_table[intermediate_index]->parameters[1] = getAddress(src);
		intermediate_table[intermediate_index]->parameters[2] = -1;  // to run the for loop
	}
	intermediate_index++;
	return;
}

void bianryOperations_func(int opcode,char *param, int instruction_no){

	char dest[variable_length], operand1[variable_length], operand2[variable_length];
	/* get the first token */
	char *token;

	token = strtok(param, ", ");
	strcpy(dest, token);

	token = strtok(NULL, ", ");
	strcpy(operand1, token);

	token = strtok(NULL, ", ");
	strcpy(operand2, token);

	

	intermediate_table[intermediate_index]->opcode = opcode;
	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_table[intermediate_index]->parameters[0] = getAddress(dest);
	intermediate_table[intermediate_index]->parameters[1] = getAddress(operand1);
	intermediate_table[intermediate_index]->parameters[2] = getAddress(operand2);
	intermediate_table[intermediate_index]->parameters[3] = -1;
	intermediate_index++;
	return;
}

void read_func(char *param, int instruction_no){
	intermediate_table[intermediate_index]->parameters[0] = getAddress(param);
	intermediate_table[intermediate_index]->parameters[1] = -1;
	intermediate_table[intermediate_index]->opcode = 14;
	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_index++;
	return;
}

void print_func(char *param, int instruction_no){
	intermediate_table[intermediate_index]->parameters[0] = getAddress(param);
	intermediate_table[intermediate_index]->parameters[1] = -1;
	intermediate_table[intermediate_index]->opcode = 13;
	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_index++;
	return;
}

void if_func(char *param, int instruction_no,int *stack,int *top){
	char operand1[variable_length],oper[4],operand2[variable_length];
	/* get the first token */
	char *token;
	sscanf(param, "%s %s %s", operand1, oper, operand2);
	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_table[intermediate_index]->opcode = 7;
	intermediate_table[intermediate_index]->parameters[0] = getAddress(operand1);
	intermediate_table[intermediate_index]->parameters[1] = getAddress(operand2);
	intermediate_table[intermediate_index]->parameters[2] = generate_opcode(oper);
	intermediate_table[intermediate_index]->parameters[3] = -2;
	intermediate_table[intermediate_index]->parameters[4] = -1;
	// push into stack //
	stack[++(*top)] = instruction_no;
	// push into stack //
	intermediate_index++;
	return;
}

void else_func(int instruction_no, int *stack, int *top){
	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_table[intermediate_index]->opcode = 6;
	intermediate_table[intermediate_index]->parameters[0] = -2;
	intermediate_table[intermediate_index]->parameters[1] = -1;
	// push into stack //
	stack[++(*top)] = instruction_no;
	// push into stack //
	intermediate_index++;
	return;
}

void endif_func(int instruction_no, int *stack, int *top){

	int poped_value = stack[(*top)--];
	int i = intermediate_index;
	while (intermediate_table[i--]->instruc_no != poped_value);
	i++;
	intermediate_table[i]->parameters[0] = instruction_no;

	int temp = poped_value;
	poped_value = stack[(*top)--];
	while (intermediate_table[i--]->instruc_no != poped_value);
	i++;
	intermediate_table[i]->parameters[3] = temp + 1 ;
	return;
}

void jump_func(char *param, int instruction_no){

	intermediate_table[intermediate_index]->instruc_no = instruction_no;
	intermediate_table[intermediate_index]->opcode = 6;
	for (int i = 0; i < blocks_index; i++){
		if (strcmp(block_tab[i]->name, param) == 0){
			intermediate_table[intermediate_index]->parameters[0] = block_tab[i]->instr_no;
			intermediate_table[intermediate_index]->parameters[1] = -1;
			break;
		}
	}
	intermediate_index++;
	return;
}

int main(){

	int stack[stack_size],top=-1;
	int memory_array[memory_size];
	int memory_index = variable_memory_start - 1 ;  // 0 to 7 are already reserved

	

	symbol_tab = (struct symbol_table**)malloc(sizeof(struct symbol_table*) * 25);
	for (int i = 0; i < 25; i++){
		symbol_tab[i] = (struct symbol_table*)malloc(sizeof(struct symbol_table));
	}

	intermediate_table = (struct intermediate_lang**)malloc(sizeof(struct intermediate_lang*)*50);
	for (int i = 0; i < 50; i++){
		intermediate_table[i] = (struct intermediate_lang*)malloc(sizeof(struct intermediate_lang));
	}

	block_tab = (struct blocks_table**)malloc(sizeof(struct blocks_table*) * 50);
	for (int i = 0; i < 50; i++){
		block_tab[i] = (struct blocks_table*)malloc(sizeof(struct blocks_table));
	}

	char filename[25],extension[25];
	printf("Enter the filename : ");
	scanf("%s", filename);

	// extension checking //
	int i = 0,j=0;
	for (i; i < strlen(filename); i++)
		if (filename[i] == '.')
			break;
	for (j = 0; i < strlen(filename); i++,j++)
		extension[j] = filename[i];
	extension[j] = '\0';
	if (strcmp(extension, ".asm") != 0)
	{
		printf("Error : file extension expected .asm found %s",extension);
		_getch();
		return 0;
	}
	// extension checking //

	FILE *fp;
	fp = fopen(filename, "r");

	char line[line_size];
	char tokens[10][10];
	char *buffer = (char*)malloc(10 * sizeof(char));

	// before start //
	while (fgets(line,line_size,fp)){
		if (strcmp(line, "START:\n") == 0)
			break;
		int row = 0,buffer_index = 0;

		// generating tokens //
		for (int i = 0; line[i] != '\0'; i++){
			if (line[i] == ' ' || line[i]=='\n'){
				buffer[buffer_index] = '\0';
				buffer_index = 0;
				strcpy(tokens[row++], buffer);
				free(buffer);
				buffer = (char*)malloc(10 * sizeof(char));
			}
			else{
				buffer[buffer_index++] = line[i];
			}
		}
		// generating tokens //

		if (strcmp(tokens[0], "DATA")==0)
			data_func(tokens,memory_array,&memory_index);
		else if (strcmp(tokens[0], "CONST") == 0)
			const_func(tokens, memory_array, &memory_index);
	}
	// before start //

	//display_symbol_table();

	// after start//
	char instruction[instruction_length],param[parametes_length];
	int opcode = -1,instruction_no = 0;

	while (!feof(fp)){
		instruction_no++;
		fscanf(fp, "%[^\n]\n", line);
		if (line[strlen(line) - 1] == ':'){
			line[strlen(line) - 1] = '\0';
			block_tab[blocks_index]->instr_no = instruction_no;
			strcpy(block_tab[blocks_index]->name, line);
			blocks_index++;
			instruction_no--;
			continue;
		}

		sscanf(line, "%s %[^*]", instruction, param);
		//printf("\ninstruction is %d  : %s %s\n",instruction_no, instruction, param);
		opcode = generate_opcode(instruction);

		switch (opcode)
		{
		    case 1 : mov_func(param,instruction_no);
				    break;
			case 3: bianryOperations_func(opcode, param, instruction_no);
					break;
			case 4: bianryOperations_func(opcode, param, instruction_no);
					break;
			case 5: bianryOperations_func(opcode, param, instruction_no);
					break;
			case 6: if (strcmp(instruction, "ELSE") == 0){
							else_func(instruction_no,stack,&top);
						}
					else{
						jump_func(param, instruction_no);
					}
					break;
			case 7: if_func(param, instruction_no,stack,&top);
					break;
			case 13: print_func(param, instruction_no);
					 break;
			case 14: read_func(param, instruction_no);
					 break;
			case 15: endif_func(instruction_no,stack,&top);
					 instruction_no--;
					 break;
			case 16: goto ending;
				break;
		}
	}
ending: 
	//display_intermediate_table();
	//display_block_table();

	fclose(fp);

	// file writing //
	dump_to_file();
	// file writing //

	// executing the program //
	executor(memory_array, memory_index);
	// executing the program //

	_getch();
	return 0;
}