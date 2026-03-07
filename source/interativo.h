#ifndef INTERATIVO
#define INTERATIVO

void modo_interativo();

void rodar_comando_sequencial(char *comando_todo, char *comando[], int tamanho);

void rodar_comando_paralelo(char *comandos[], int tamanho);

void comando_com_pipe(char *comandos[], int tamanho);

#endif