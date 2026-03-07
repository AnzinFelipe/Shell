#include "interativo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

char **separar_string(char *string, char **separadas, char *separador, int max) {
    char *token = strtok(string, separador);
    int i = 0;
    while (token != NULL && i < max) {
        separadas[i] = token;
        token = strtok(NULL, separador);
        i++;
    }
    separadas[i] = NULL;
    return separadas;
}

void comando_bg(char *comando, int *pid_bg) {
    int comando_max = 64;
    char **comandos = malloc(sizeof(char *) * comando_max);
    comandos = separar_string(comando, comandos, "&", comando_max);

    char **arg = malloc(sizeof(char *) * comando_max);

    arg = separar_string(comandos[0], arg, " ", comando_max);

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("[ERRO] falha ao criar fork\n");
        exit(1);
    } else if (pid == 0) {
        execvp(arg[0], arg);
    } else {
        *pid_bg = pid;
        printf("[1] %d\n", *pid_bg);
    }

    free(comandos);
    free(arg);
}

void comando_red_saida(char * comando_red) {
    int comando_max = 64;
    char **comandos = malloc(sizeof(char *) * comando_max);
    comandos = separar_string(comando_red, comandos, ">", comando_max);

    char **arg1 = malloc(sizeof(char *) * comando_max);
    char **arg2 = malloc(sizeof(char *) * comando_max);

    arg1 = separar_string(comandos[0], arg1, " ", comando_max);
    arg2 = separar_string(comandos[1], arg2, " ", comando_max);

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("[ERRO] falha ao criar fork\n");
        exit(1);
    } else if (pid == 0) {
        int fd = open(arg2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(arg1[0], arg1);
    } else {
        wait(NULL);
    }
    free(comandos);
    free(arg1);
    free(arg2);
}

void comando_red_entrada(char * comando_red) {
    int comando_max = 64;
    char **comandos = malloc(sizeof(char *) * comando_max);
    comandos = separar_string(comando_red, comandos, "<", comando_max);

    char **arg1 = malloc(sizeof(char *) * comando_max);
    char **arg2 = malloc(sizeof(char *) * comando_max);

    arg1 = separar_string(comandos[0], arg1, " ", comando_max);
    arg2 = separar_string(comandos[1], arg2, " ", comando_max);

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("[ERRO] falha ao criar fork\n");
        exit(1);
    } else if (pid == 0) {
        int fd = open(arg2[0], O_RDONLY);
        if (fd == -1) {
            printf("[ERRO] arquivo não encontrado\n");
            exit(1);
        } else {
            dup2(fd, STDIN_FILENO);
            close(fd);
            execvp(arg1[0], arg1);
        }
    } else {
        wait(NULL);
    }
    free(comandos);
    free(arg1);
    free(arg2);
}

void comando_red_saida_final(char * comando_red) {
    int comando_max = 64;
    char **comandos = malloc(sizeof(char *) * comando_max);
    comandos = separar_string(comando_red, comandos, ">", comando_max);

    char **arg1 = malloc(sizeof(char *) * comando_max);
    char **arg2 = malloc(sizeof(char *) * comando_max);

    arg1 = separar_string(comandos[0], arg1, " ", comando_max);
    arg2 = separar_string(comandos[1], arg2, " ", comando_max);

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("[ERRO] falha ao criar fork\n");
        exit(1);
    } else if (pid == 0) {
        int fd = open(arg2[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(arg1[0], arg1);
    } else {
        wait(NULL);
    }
    free(comandos);
    free(arg1);
    free(arg2);
}

void comando_com_pipe(char *comando_com_pipe) {
    int fd[2];
    int comando_max = 64;
    char **comandos = malloc(sizeof(char *) * comando_max);
    comandos = separar_string(comando_com_pipe, comandos, "|", comando_max);

    char **arg1 = malloc(sizeof(char *) * comando_max);
    char **arg2 = malloc(sizeof(char *) * comando_max);

    arg1 = separar_string(comandos[0], arg1, " ", comando_max);
    arg2 = separar_string(comandos[1], arg2, " ", comando_max);
    
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
    free(comandos);
    free(arg1);
    free(arg2);
}

void verificar_separador_sequencial(char *comandos[], int tamanho, int *pid_bg) {
    char c;
    for (int j = 0; j < tamanho; j++) {
        int tem_pipe = 0;
        int background = 0;
        int redirecionamento = 0;
        int k = 0;

        while ((c = comandos[j][k]) != '\0') {
            if (c == '|') {
                comando_com_pipe(comandos[j]);
                tem_pipe = 1;
                break;
            }
            if (c == '>') {
                char c_prox = comandos[j][k+1];
                if ((c_prox) == '>') {
                    comando_red_saida_final(comandos[j]);
                } else {
                    comando_red_saida(comandos[j]);
                }
                redirecionamento = 1;
                break;
            }
            if (c == '<') {
                comando_red_entrada(comandos[j]);
                redirecionamento = 1;
                break;
            }
            if (c == '&') {
                comando_bg(comandos[j], pid_bg);
                background = 1;
                break;
            }
            k++;
        }
        if (tem_pipe == 0 && redirecionamento == 0 && background == 0) {
            rodar_comando_sequencial(comandos[j]);
        }
    }
}

void verificar_separador_paralelo(char *comandos[], int tamanho, int *pid_bg) {
    char c;
    int i = 0;
    char **comandos_paralelos = malloc(sizeof(char *) * tamanho);
    for (int j = 0; j < tamanho; j++) {
        int tem_pipe = 0;
        int background = 0;
        int redirecionamento = 0;
        int k = 0;

        while ((c = comandos[j][k]) != '\0') {
            if (c == '|') {
                comando_com_pipe(comandos[j]);
                tem_pipe = 1;
                break;
            }
            if (c == '>') {
                char c_prox = comandos[j][k+1];
                if ((c_prox) == '>') {
                    comando_red_saida_final(comandos[j]);
                } else {
                    comando_red_saida(comandos[j]);
                }
                redirecionamento = 1;
                break;
            }
            if (c == '<') {
                comando_red_entrada(comandos[j]);
                redirecionamento = 1;
                break;
            }
            if (c == '&') {
                comando_bg(comandos[j], pid_bg);
                background = 1;
                break;
            }
            k++;
        }
        if (tem_pipe == 0 && redirecionamento == 0 && background == 0) {
            comandos_paralelos[i] = comandos[j];
            i++;
        }
    }
    rodar_comando_paralelo(comandos_paralelos, i);
    free(comandos_paralelos);
}

void rodar_comando_sequencial(char *comandos) {
    int comando_max = 64;
    char **arg = malloc(sizeof(char *) * comando_max);

    arg = separar_string(comandos, arg, " ", comando_max);

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
    free(arg);
}

void rodar_comando_paralelo(char *comandos[], int tamanho) {
    for (int j = 0; j < tamanho; j++) {
        int comando_max = 20;
        char **arg = malloc(sizeof(char *) * comando_max);

        arg = separar_string(comandos[j], arg, " ", comando_max);

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
                free(arg);
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

    pid_t pid_bg = -1;

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

        if (strcmp("exit", comandos) == 0) {
            break;
        } else if (strcmp("style sequencial", comandos) == 0) {
            seq = 1;
            par = 0;
        } else if (strcmp("style parallel", comandos) == 0) {
            par = 1;
            seq = 0;
        } else if (strcmp("fg", comandos) == 0 ) {
            if (pid_bg > 0) {
                waitpid(pid_bg, NULL, 0);
                pid_bg = -1;
            } else {
                printf("Não existe trabalho em background\n");
            }
        } else {
            char *comandos_separados[comandos_max];
            char *token = strtok(comandos, ";");
            int i = 0;
            while (token != NULL && i < comandos_max) {
                comandos_separados[i] = token;
                token = strtok(NULL, ";");
                i++;
            }
            
            if (seq == 1) {
                verificar_separador_sequencial(comandos_separados, i, &pid_bg);
            } else if (par == 1) {
                verificar_separador_paralelo(comandos_separados, i, &pid_bg);
            } 
        } 
        
    }
}