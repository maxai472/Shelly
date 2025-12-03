#include "shelly.h"

int main(int argc, char **argv){
  // Run the start up script:
  // - Some ASCI art to make it look pretty
  // Enter into the REPL loop
  (void)argc;
  int status;

  if (fork() == 0)
    execvp(argv[1], &argv[1]);

  wait(&status);

  return EXIT_SUCCESS;
}
