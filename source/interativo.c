#include "interativo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            printf("Está faltando argumento para o comando\n");
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
        } else if (pid > 0) {
            wait(NULL);
        }
    }
}

void modo_interativo() {
    int seq = 1, par = 0;
    int comandos_max = 100;

    char comandos[comandos_max];

    printf("Entrou no modo interativo\n");
    while(1) {
        if (seq == 1) {
            printf("Shell seq> ");
        } else if (par == 1) {
            printf("Shell par> ");
        }

        fgets(comandos, comandos_max, stdin);
        comandos[strcspn(comandos, "\n")] = '\0';

        if (strcmp("style sequencial", comandos) == 0) {
            seq = 1;
            par = 0;
        } else if (strcmp("style parallel", comandos) == 0) {
            par = 1;
            seq = 0;
        }

        if (strcmp("exit", comandos) == 0) {
            break;
        }

        char *comandos_separados[comandos_max];
        char *token = strtok(comandos, ";");
        int i = 0;
        while (token != NULL && i < comandos_max) {
            comandos_separados[i] = token;
            token = strtok(NULL, ";");
            i++;
        }
        
        for (int j = 0; j < i; j++) {
            rodar_comando(comandos_separados[j]);
        }
        
    }
}