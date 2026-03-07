#include "interativo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void comando_com_pipe(char *comandos[], int tamanho) {
    int fd[2];
    int comando_max = 64;
    char *arg1[comando_max];
    char *arg2[comando_max];

    char *token = strtok(comandos[0], " ");
    int i = 0;
    while (token != NULL && i < comando_max) {
        arg1[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    arg1[i] = NULL;
    token = strtok(comandos[1], " ");
    i = 0;
    while (token != NULL && i < comando_max) {
        arg2[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    arg2[i] = NULL;
    
    if (pipe(fd) == -1) {
        printf("[ERRO] falha ao criar pipe\n");
        exit(1);
    }

    if (strcmp(arg1[0], "cd") == 0) {
        if (arg1[1] == NULL) {
            
        } else if (chdir(arg1[1]) != 0) {
            printf("Diretório não encontrado\n");
        } else {
            char diretorio[1024];
            getcwd(diretorio, sizeof(diretorio));
            printf("Diretório atual: %s\n", diretorio);
        }
    } else {
        pid_t pid1;
        pid1 = fork();
        
        if (pid1 < 0) {
            printf("[ERRO] falha ao criar fork\n");
            exit(1);
        } else if (pid1 == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            
            execvp(arg1[0], arg1);
        } else {
            close(fd[1]);
        }
        pid_t pid2;
        pid2 = fork();

        if (pid2 < 0) {
            printf("[ERRO] falha ao criar fork\n");
            exit(1);
        } else if (pid2 == 0) {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            execvp(arg2[0], arg2);
        } else {
            close(fd[0]);
        }

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
}

void rodar_comando_sequencial(char *comando_todo, char *comandos[], int tamanho) {

    int c = 0;
    int tem_pipe = 0;
    while (comando_todo[c] != '\0') {
        if (comando_todo[c] == '|') {
            tem_pipe = 1;
            break;
        }
        c++;
    }
    
    if (tem_pipe == 1) {
        comando_com_pipe(comandos, tamanho);
    } else {
        for(int j = 0; j < tamanho; j++) {
            int comando_max = 20;
            char *arg[comando_max];

            char *token = strtok(comandos[j], " ");
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
                } else if (pid > 0) {
                    wait(NULL);
                }
            }
        }
    }
}

void rodar_comando_paralelo(char *comandos[], int tamanho) {

    for (int j = 0; j < tamanho; j++) {
        int comando_max = 20;
        char *arg[comando_max];
    
        char *token = strtok(comandos[j], " ");
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
                exit(0);
            } else {
                continue;
            }
        }
    }
    for (int j = 0; j < tamanho; j++) {
        wait(NULL);
    }
}

void modo_interativo() {
    int seq = 1, par = 0;
    int comandos_max = 100;

    char comandos[comandos_max];
    char comandos_copia[comandos_max];

    printf("Entrou no modo interativo\n");
    while(1) {
        if (seq == 1) {
            printf("Shell seq> ");
        } else if (par == 1) {
            printf("Shell par> ");
        }

        fgets(comandos, comandos_max, stdin);
        comandos[strcspn(comandos, "\n")] = '\0';

        if (strcmp("exit", comandos) == 0) {
            break;
        } else if (strcmp("style sequencial", comandos) == 0) {
            seq = 1;
            par = 0;
        } else if (strcmp("style parallel", comandos) == 0) {
            par = 1;
            seq = 0;
        } else {
            strcpy(comandos_copia, comandos);
            char *comandos_separados[comandos_max];
            char *token = strtok(comandos, ";|");
            int i = 0;
            while (token != NULL && i < comandos_max) {
                comandos_separados[i] = token;
                token = strtok(NULL, ";|");
                i++;
            }
            
            if (seq == 1) {
                rodar_comando_sequencial(comandos_copia, comandos_separados, i);
            } else if (par == 1) {
                rodar_comando_paralelo(comandos_separados, i);
            } 
        } 
        
    }
}