#include "../libs/io.c"
#include "../libs/mem.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  size_t input_length;
} InputBuffer;

typedef enum {
  VALUE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  POWER,
  BRACKET_OPEN,
  BRACKET_CLOSE
} TOKEN_TYPE;

typedef struct {
  void* value;
  TOKEN_TYPE type;
} Token;

void print_prompt();
InputBuffer* new_input_buffer(Arena* arena);
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer, Arena* arena);
void stop();
void tokenize(InputBuffer* input_buffer);

Arena* arena;

int main(int argc, char **argv) {
  arena = arena_create(10 * 1024);
  InputBuffer* input_buffer = new_input_buffer(arena);

  // Inline argument calculation
  if (argc > 1) {
//    char formula[128];
//    for (int i = 1; (i < argc); i++) {
//      strcat(formula, argv[i]);
//    }
//    printf("Computing -> %s\n", formula);
    printf("Not supported atm.\n");
    stop();
  }

  // REPL
  while (1) {
    print_prompt();
    read_input(input_buffer);

    // Refactor this shit
    if (strcmp(input_buffer->buffer, "exit") == 0) {
      stop();
    }
    if (strcmp(input_buffer->buffer, "q") == 0) {
      stop();
    }
    if (strcmp(input_buffer->buffer, "quit") == 0) {
      stop();
    }

    // Parse Input and evaluate
    tokenize(input_buffer);
  }
}

void stop() {
  arena_destroy(arena);
  exit(EXIT_SUCCESS);
}

void print_prompt() {
  printf("Calc > ");
}

InputBuffer* new_input_buffer(Arena* arena) {
	InputBuffer* input_buffer = (InputBuffer*)arena_alloc(arena, sizeof(InputBuffer));
	input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;

	return input_buffer;
}

void read_input(InputBuffer* input_buffer) {
	size_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if (bytes_read <= 0) {
		printf("Error reading input\n");
		exit(EXIT_FAILURE);
	}

	// Ignore trailing newline
	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer, Arena* arena) {
  arena_clear(arena);
}

char* print_token_type(const TOKEN_TYPE type) {
  switch (type) {
    case VALUE: return "VALUE";
    case ADD: return "ADD";
    case SUBTRACT: return "SUBTRACT";
    case MULTIPLY: return "MULTIPLY";
    case DIVIDE: return "DIVIDE";
    case POWER: return "POWER";
    case BRACKET_OPEN: return "OPEN BRACKET";
    case BRACKET_CLOSE: return "CLOSE BRACKET";
    default: return "Unkown";
  }
}

void tokenize(InputBuffer* input_buffer){
  for (int i = 0; i < input_buffer->input_length; i++) {
    char value = input_buffer->buffer[i];

    // Remove not needed values, i.e. white spaces => should become a map or something
    if (value == ' ') {
      continue;
    }

    Token token = {
      .value  = &value
    };
    switch (value) {
      case '+':
        token.type = ADD;
        break;
      case '-':
        token.type = SUBTRACT;
        break;
      case '*':
        token.type = MULTIPLY;
        break;
      case '/':
        token.type = DIVIDE;
        break;
      case '^':
        token.type = POWER;
        break;
      case '(':
      case '{':
      case '[':
        token.type = BRACKET_OPEN;
        break;
      case ')':
      case '}':
      case ']':
        token.type = BRACKET_CLOSE;
        break;
      default:
        token.type = VALUE;
        // TODO: lookahead to find end of value given
        break;
    }

    printf("%c => %s\n", *(char*)token.value, print_token_type(token.type));
  }
}
