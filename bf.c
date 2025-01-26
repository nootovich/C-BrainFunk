#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEBUG false
#define FILE_PATH "../IdeaProjects/BrainFunk/tests/hello_world.bf"
#define TAPE_SIZE 30000

char tape[TAPE_SIZE] = {0};
int ptr = 0;

typedef enum {
	PLUS,
	MINUS,
	GREATER,
	LESS,
	LBRACKET,
	RBRACKET,
	DOT,
	COMMA
} TokenType;

typedef struct {
	char* file_path;
	int row;
	int col;
} Location;

typedef struct {
	TokenType type;
	Location loc;
	int num;
	char* str;
} Token;

char* read_file_bytes(char* file_path, size_t* file_size) {
	FILE *file = fopen(file_path, "rb");
	fseek(file, 0, SEEK_END);
	*file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* file_bytes = malloc(*file_size + 1);
	fread(file_bytes, *file_size, 1, file);
	fclose(file);
	file_bytes[*file_size] = 0;
	return file_bytes;
}

int main() {

	size_t input_len = 0;
	char* input = read_file_bytes(FILE_PATH, &input_len);
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
}
