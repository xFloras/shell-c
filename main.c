#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tokenizer.h"




int search_path(char *fname, char* full_path) {
    char* path_copy = strdup(getenv("PATH"));
    char* strptr;
    char *token = strtok_r(path_copy, ":", &strptr);
    char file_path[PATH_MAX];
    while (token != NULL) {
        strcpy(file_path, token);
        if (file_path[strlen(token) - 1] != '/') {
            strcat(file_path, "/");
        }
        strcat(file_path, fname);
        if (access(file_path, F_OK | X_OK) == 0) {
            strcpy(full_path, file_path);
            free(path_copy);
            return 0;
        }
        token = strtok_r(NULL, ":", &strptr);
    }
    free(path_copy);
    return 1;
}

      int run_command(const char *path, char *const args[],
                      char *const envp[]) {

        pid_t pid = fork();

        if (pid < 0) {
          perror("[ERROR] fork failed.");
          return -1;
        }

        if (pid == 0) {

          char *const default_env[] = {NULL};
          char *const *env_to_use = (envp != NULL) ? envp : default_env;

          execve(path, args, env_to_use);
          perror("[ERROR] excve failed.");

          _exit(127);
        }

        int status = 0;
        pid_t wait_result;

        do {
            wait_result = waitpid(pid, &status, 0);
        } while (wait_result == -1 && errno == EINTR);

        if (wait_result == -1) {

          perror("[ERROR] waitpid failed.");
          return -1;
        }

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            //printf("[PARENT] The child process (PID= %d) ended with code: %d\n", pid, exit_code);
            return exit_code;
        } else if (WIFSIGNALED(status)) {
          int sig_code = WSTOPSIG(status);
          //printf("[PARENT] The child process (PID= %d) terminated by signal: %d\n", pid,
          //sig_code);
          return -1;
        }
        return -1;


      }





int main(int argc, char *argv[]) {

  while (1) {
      setbuf(stdout, NULL);

      printf("$ ");

      char input[1024];
      fgets(input, 1024, stdin);
      rendl(input);

      char** tokenized_input = tokenize_input(input);

      if(tokenized_input == NULL || tokenized_input[0] == NULL) {
          continue;
      }

      char* command = tokenized_input[0];
      char** args = tokenized_input+1;
      char* arg0 = tokenized_input[1];

      if (strcmp(command, "exit") == 0) {
          break;
      }
      if (strcmp(command, "echo") == 0) {
        char **token_ptr = tokenized_input + 1;
        while (*token_ptr != NULL) {

          printf("%s", *token_ptr);
          if (*(token_ptr + 1) != NULL) {

              putchar(' ');
          }
          token_ptr++;
        }
        putchar('\n');
          continue;
      }

      // PWD
      char* cwd;
      if (strcmp(command, "pwd") == 0) {
        if ((cwd = getcwd(NULL, 0)) != NULL) {
          printf("%s\n", cwd);
          continue;
        }else {
          perror("getcwd() error");
          return 1;
        }
      }

      // CD

      if (strcmp(command, "cd") == 0) {
          char* full_path = arg0;
        if (full_path == NULL || strcmp(full_path, "~") == 0) {
            full_path = getenv("HOME");
            if (full_path == NULL) {
              fprintf(stderr, "cd: variable HOME is not set\n");
            }
        }
        char old_cwd[PATH_MAX];
        if (getcwd(old_cwd, sizeof(old_cwd)) == NULL) {
          perror("cd: getcwd");
        }
        if (chdir(full_path) != 0) {
            fprintf(stderr, "cd: %s: %s\n", full_path, strerror(errno));
        }
        char new_cwd[PATH_MAX];
        if (getcwd(new_cwd, sizeof(new_cwd)) != NULL) {
            setenv("OLDPWD", old_cwd, 1);
            setenv("PWD", new_cwd, 1);
        }
        continue;
      }



      char file_path[PATH_MAX];
      if (strcmp(command, "type") == 0) {
          if (arg0 == NULL) {
              continue;
          }
        if (strcmp(arg0, "exit") == 0 || strcmp(arg0, "echo") == 0 ||
            strcmp(arg0, "type") == 0 || strcmp(arg0, "pwd") == 0){
            printf("%s is a shell builtin\n", arg0);
            continue;
        }else if (search_path(arg0, file_path) == 0) {
            printf("%s is %s\n", arg0, file_path);
            continue;

        } else {
            printf("%s: not found\n", arg0);
            continue;
        }
      }
      if (search_path(command, file_path) == 0) {
          char* env[] = {"PATH=/bin", NULL};
          run_command(file_path, tokenized_input, env);
          continue;
      }

      for (int i = 0; tokenized_input[i] != NULL; i++) {

          free(tokenized_input[i]);
      }
      free(tokenized_input);

      printf("%s: command not found\n", input);
  }

  return 0;
}
