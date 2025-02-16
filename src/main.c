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

void print_prompt();
InputBuffer* new_input_buffer(Arena* arena);
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer, Arena* arena);
void stop();

Arena* arena;

int main(int argc, char **argv) {
  arena = arena_create(10 * 1024);
  InputBuffer* input_buffer = new_input_buffer(arena);

  // Inline argument calculation
  if (argc > 1) {
    char formula[128];
    for (int i = 1; (i < argc); i++) {
      strcat(formula, argv[i]);
    }
    printf("Computing -> %s\n", formula);
    stop();
  }

  // REPL
  while (1) {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, "exit") == 0) {
      stop();
    }

    // Parse Input and evaluate
    printf("Here we should parse the input and evaluate.\n");
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
