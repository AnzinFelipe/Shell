#ifndef INTERATIVO
#define INTERATIVO

void modo_interativo();

void rodar_comando_sequencial(char *comando);

void rodar_comando_paralelo(char *comandos[], int tamanho);

void comando_com_pipe(char *comando_com_pipe, int tamanho);

void verificar_separador_sequencial(char *comandos[], int tamanho);

void verificar_separador_paralelo(char *comandos[], int tamanho);

char **separar_string(char *string, char **separadas, char *separador, int max);

#endif