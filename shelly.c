#include "shelly.h"
#include <stdlib.h>
#include <string.h>

const char *SHELLY_VERSION = "0.1.0";
const char *SHELLY_AUTHOR = "Max Aitken";
const char *SHELLY_BANNER =
    "  █████████     █████                     ████     ████               \n"
    " ███░░░░░███   ░░███                     ░░███    ░░███               \n"
    "░███    ░░░     ░███████       ██████     ░███     ░███     █████ ████\n"
    "░░█████████     ░███░░███     ███░░███    ░███     ░███    ░░███ ░███ \n"
    " ░░░░░░░░███    ░███ ░███    ░███████     ░███     ░███     ░███ ░███ \n"
    " ███    ░███    ░███ ░███    ░███░░░      ░███     ░███     ░███ ░███ \n"
    "░░█████████     ████ █████   ░░██████     █████    █████    ░░███████ \n"
    " ░░░░░░░░░     ░░░░ ░░░░░     ░░░░░░     ░░░░░    ░░░░░      ░░░░░███ \n"
    "                                                             ███ ░███ \n"
    "                                                            ░░██████  \n"
    "                                                             ░░░░░░   \n";
/*** built-in commands ***/

/*** Initialisation functions ***/
void shellyInit() {
  printf("Welcome to...\n");
  printf(GREEN);
  printf("\n%s\n", SHELLY_BANNER);
  printf(RESET_COLOUR);
}

/*** read line ***/
void shellyPrintPrompt(void) {
  char cwd[1024];
  const char *home = getenv("HOME");

  getcwd(cwd, sizeof(cwd));
  if (strncmp(cwd, home, strlen(home)) == 0) {
    printf(GREEN "shelly: " CYAN "~%s" RESET_COLOUR " >", cwd + strlen(home));
  } else {
    printf(GREEN "shelly: " CYAN " %s" RESET_COLOUR " >", cwd);
  }
}

char *shellyReadLine(void) {
  int bufsize = SHELLY_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);

  if (!buffer) {
    fprintf(stderr, "shelly: memory allocation error\n");
    exit(EXIT_FAILURE);
  }

  shellyPrintPrompt();

  while (1) {
    int c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
      position++;
    }

    if (position >= bufsize) {
      bufsize += SHELLY_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "shelly: memory allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

/*** parse line ***/
// TODO: implement proper lexing and parsing to handle quotes, escapes, etc.
char **shellyParseLine(char *line) {
  int bufsize = SHELLY_TOKEN_BUFSIZE;
  int position = 0;
  char **tokens = malloc(sizeof(char) * bufsize);
  char *token;

  if (!tokens) {
    fprintf(stderr, "shelly: memory allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SHELLY_TOKEN_DELIMS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SHELLY_TOKEN_BUFSIZE;
      tokens = realloc(tokens, bufsize);
      if (!tokens) {
        fprintf(stderr, "shelly: memory allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, SHELLY_TOKEN_DELIMS);
  }
  tokens[position] = NULL;
  return tokens;
}

/*** execute command ***/
int shellyLaunchApplication(char **arguments) {
  pid_t pid, wpid;
  int status;

  if ((pid = fork()) == 0){
    // child runs here
    if (execvp(arguments[0], arguments) == -1) {
      perror("shelly");
    }
  } else if (pid < 0) {
    // error when forking
    perror("shelly");
  } else {
    // parent runs here
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}



/*** REPL loop ***/
void shellyLoop(void) {
  char *line;
  char **arguments;
  int status;

  do {
    line = shellyReadLine();
    arguments = shellyParseLine(line);
    status = shellyLaunchApplication(arguments);
  } while (status);
}

int main(int argc, char **argv) {
  // Run the start up script:
  // - Some ASCI art to make it look pretty
  // Enter into the REPL loop
  shellyInit();
  shellyLoop();

  // (void)argc;
  // int status;
  //
  // if (fork() == 0)
  //   execvp(argv[1], &argv[1]);
  //
  // wait(&status);

  return EXIT_SUCCESS;
}
