#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1000
#define MAX_COMMANDS 20
#define MAX_DIR 300

void parser(char* input, char* buffer[]) {
    int i = 0;
    char *cur = input;
    while ((i < MAX_COMMANDS) && (*++input != '\0')) {

        if (*input == ' ' || *input == '\n') {
            *input = '\0';
            buffer[i++] = cur;
            cur = input + 1;
        }
    }

    buffer[i++] = NULL;
}


void execute(char* commands[]) {

    pid_t pid;
    int status;

    if (commands[0] == NULL)
        return;

    if (strcmp(commands[0], "cd") == 0) {
        if (chdir(commands[1]) < 0){
            perror("Falha ao mudar de diretorio\n");
        }
        return;
    }
    if (strcmp(commands[0], "exit") == 0)
        exit(0);

    pid = fork();

    if(pid == -1){
        perror("Falha ao realizar o fork()!\n");
        return;
    }

    else if(pid == 0){
        if(execvp(commands[0], commands) <0)
            perror("Falha ao executar comando\n");
    }

    wait(&status);
}


int main(void) {
    char line[MAX_LINE];
    char dir_buff[MAX_DIR];
    char* commands[MAX_COMMANDS];

    while(1) {

        char* directory = getcwd(dir_buff, MAX_DIR);
        int dir_size = strlen(directory);
        write(STDOUT_FILENO, directory, dir_size);
        write(STDOUT_FILENO, " $", 3);

        if (read(STDIN_FILENO, line, MAX_LINE) == -1){
            perror("Leitura incorreta\n");
            }

        else {
            parser(line, commands);
            execute(commands);
        }
    }
}
