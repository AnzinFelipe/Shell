#include "batch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void rodar_comando(char *comando) {
    int comando_max = 20;
    char *arg[comando_max];

    char *token = strtok(comando, " ");
    int i = 0;
    while (token != NULL && i < comando_max) {
        arg[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    arg[i] = NULL;

    if (strcmp(arg[0], "cd") == 0) {
        if (arg[1] == NULL) {
            
        } else if (chdir(arg[1]) != 0) {
            printf("Diretório não encontrado\n");
        } else {
            char diretorio[1024];
            getcwd(diretorio, sizeof(diretorio));
            printf("Diretório atual: %s\n", diretorio);
        }
    } else {
        pid_t pid;
        pid = fork();
    
        if (pid < 0) {
            printf("[ERRO] falha ao criar fork\n");
            exit(1);
        } else if (pid == 0) {
            execvp(arg[0], arg);
            printf("Comando inexistente\n");
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        }
    }
}

void modo_batch(char *argv) {
    int tem_exit = 0;

    FILE *arquivo;
    arquivo = fopen(argv, "r");
    if (arquivo == NULL) {
        printf("[ERRO] arquivo não encontrado\n");
        exit(1);
    }
    char comando[64];

    while(fgets(comando, 64, arquivo) != NULL) {
        comando[strcspn(comando, "\n")] = '\0';
        if (strcmp(comando, "exit") == 0) {
            tem_exit = 1;
            break;
        }
    }
    rewind(arquivo);
    if (tem_exit == 0) {
        printf("[ERRO] arquivo batchfile sem exit\n");
        exit(1);
    } 
    
    while(fgets(comando, 64, arquivo) != NULL) {
        comando[strcspn(comando, "\n")] = '\0';
        printf("%s\n", comando);
    }
    rewind(arquivo);

    while(fgets(comando, 64, arquivo) != NULL) {
        comando[strcspn(comando, "\n")] = '\0';
        if (strcmp(comando, "exit") == 0) {
            tem_exit = 1;
            break;
        }
        rodar_comando(comando);
    }

    fclose(arquivo);
}