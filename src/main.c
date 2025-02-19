#include "../libs/io.c"
#include "../libs/mem.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define KB(x) x*1024

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
  char* value;
  TOKEN_TYPE type;
} Token;

void print_prompt();
InputBuffer* new_input_buffer(Arena* arena);
void read_input(InputBuffer* input_buffer);
void stop();
void tokenize(InputBuffer* input_buffer, Arena* arena);
void find_end_of_token(const InputBuffer* input_buffer, size_t* index);

Arena* buffer_arena;
Arena* loop_arena;

int main(int argc, char **argv) {
  buffer_arena = arena_create(KB(10));
  loop_arena = arena_create(KB(10));
  InputBuffer* input_buffer = new_input_buffer(buffer_arena);

  // Inline argument calculation
  if (argc > 1) {
    printf("Not supported atm.\n");
    stop();
  }

  // REPL
  while (1) {
    print_prompt();
    arena_clear(loop_arena);
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
    tokenize(input_buffer, loop_arena);
  }
}

void stop() {
  arena_destroy(buffer_arena);
  arena_destroy(loop_arena);
  exit(EXIT_SUCCESS);
}

void print_prompt() {
  printf("Calc > ");
}

InputBuffer* new_input_buffer(Arena* arena) {
	InputBuffer* input_buffer = (InputBuffer*)arena_alloc(arena, sizeof(InputBuffer));
	input_buffer->buffer = arena_alloc(arena, sizeof(char) * 128);
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

void tokenize(InputBuffer* input_buffer, Arena* arena){
  Token *token = arena_alloc(arena, sizeof(Token));
  token->value = arena_alloc(arena, sizeof(char) * 128);

  for (size_t i = 0; i < input_buffer->input_length; i++) {
    char value = input_buffer->buffer[i];

    // Remove not needed values, i.e. white spaces => should become a map or something
    if (value == ' ') {
      continue;
    }

    token->value = NULL;
    token->value = &value;
    switch (value) {
      case '+':
        token->type = ADD;
        break;
      case '-':
        token->type = SUBTRACT;
        break;
      case '*':
        token->type = MULTIPLY;
        break;
      case '/':
        token->type = DIVIDE;
        break;
      case '^':
        token->type = POWER;
        break;
      case '(':
      case '{':
      case '[':
        token->type = BRACKET_OPEN;
        break;
      case ')':
      case '}':
      case ']':
        token->type = BRACKET_CLOSE;
        break;
      default:
        token->type = VALUE;
        //int start = i;
        find_end_of_token(input_buffer, &i);
        //int length_of_token = i - start + 1;

        //printf("Start = %d\n", start);
        //printf("Length of token = %d\n", length_of_token);
        //printf("Index = %d\n", (int)i);
        // Why The Fuck does SIMD increment my index in strncpy???
        //char* test = arena_alloc(arena, sizeof(char) * 64);
        //memcpy(token->value, &input_buffer->buffer[start], length_of_token);
        //printf("Value = %s\n", test);
        //printf("Index = %d\n", (int)i);
        break;
    }

    printf("%s => %s\n", token->value, print_token_type(token->type));
  }
}

void find_end_of_token(const InputBuffer* const input_buffer, size_t* index){
  // Check if the next_value is a token of any kind
  bool is_value = true;

  while (is_value && *index < input_buffer->input_length) {
    char next_value = input_buffer->buffer[++*index];
    // TODO: this should really be a map of some sort
    switch (next_value) {
      case ' ':
      case '\0':
      case '+':
      case '-':
      case '*':
      case '/':
      case '^':
      case '(':
      case '{':
      case '[':
      case ')':
      case '}':
      case ']':
        is_value = false;
        break;
      default:
        is_value = true;
        break;
    }
  }

  --*index;
}
