#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEBUG false
#define FILE_PATH "../IdeaProjects/BrainFunk/tests/hello_world.bf"
#define TAPE_SIZE 100

char tape[TAPE_SIZE] = {0};
int ptr = 0;

int main() {
	FILE *file = fopen(FILE_PATH, "r");
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* input = malloc(fsize + 1);
	fread(input, fsize, 1, file);
	fclose(file);
	input[fsize] = 0;
	int input_len = strlen(input);
	
	printf("Read %d bytes of file '%s' succesfully\n", input_len, FILE_PATH);

	int i = 0;
	while (i < input_len) {
		switch(input[i]) {
			case '+':
				if (DEBUG) printf("ADD $%d\n", ptr);
				tape[ptr]++;
				break;
			case '-':
				if (DEBUG) printf("SUB $%d\n", ptr);
				tape[ptr]--;
				break;
			case '>':
				ptr++;
				if (DEBUG) printf("RGT $%d\n", ptr);
				break;
			case '<':
				ptr--;
				if (DEBUG) printf("LFT $%d\n", ptr);
				break;
			case '[': {
				if (DEBUG) printf("JEZ $%d\n", ptr);
				int depth = 0;
				for (int j = i + 1; j <= input_len; j++) {
					if (j == input_len) break;
					else if (input[j] == '[') depth++;
					else if (input[j] == ']' && depth-- == 0) {
						if (tape[ptr] == 0) i = j;
						goto end_loop;
					}
				}
				printf("ERROR: Unmatched '['\n");
				return 1;
			}
			case ']': {
				if (DEBUG) printf("JNZ $%d\n", ptr);
				int depth = 0;
				for (int j = i - 1; j >= 0; j--) {
					if (j == 0) break;
					else if (input[j] == ']') depth++;
					else if (input[j] == '[' && depth-- == 0) {
						if (tape[ptr] != 0) i = j;
						goto end_loop;
					}
				}
				printf("ERROR: Unmatched '['\n");
				return 1;
			}
			case '.':
				if (DEBUG) printf("PRINT %c\n", tape[ptr]);
				else printf("%c", tape[ptr]);
				break;
			case ',':
				printf("ERROR: ',' is not implemented yet\n");
				return 1;
			default:
				if (DEBUG) printf("skipping '%c' character\n", input[i]);
		}
end_loop:
		i += 1;
	}
	printf("\n---------------------------\n");
	for (i = 0; i < TAPE_SIZE; ++i) {
		printf("%d ", tape[i]);
	}
}
