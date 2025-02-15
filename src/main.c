#include "stdio.h"
#include "stdlib.h"

void print_prompt() { printf("calc > "); }

int main(int argc, char **argv) {
  if (argc > 1) {
    printf("Computing -> %s\n", argv[1]);
  } else {
    while (1) {
      print_prompt();
      // TODO: read input
      exit(EXIT_SUCCESS);
    }
  }
}
