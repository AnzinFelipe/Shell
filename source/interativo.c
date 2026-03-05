#include "interativo.h"
#include <stdio.h>
#include <string.h>

void modo_interativo() {
    char comando[10];
    printf("Entrou no modo interativo\n");
    while(1) {
        printf("Shell> ");
        scanf("%s", comando);

        if (strcmp("exit", comando) == 0) {
            break;
        }
    }
}