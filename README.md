# Implementador por Linha de Comando (Shell)

## Como compilar e executar
para compilar escreva: 
- _make clean_
- _make_

para executar:
- _./shell [batchfile]_ ou apenas _./shell_

para testar:
- _make test_ ou *make test_batch*

## Descrição dos arquivos:
- main.c: estrutura principal do código que leva para as funções do shell batch ou do shell interativo.

- batch.h: define as funções do shell batch.

- batch.c: contêm as funções da biblioteca batch.h. A função para ler o batchfile passado para esse modo e a função para executar os comandos dentro do arquivo.

- interativo.h: define as funções do shell interativo.

- interativo.c: contêm as funções da biblioteca interativo.h. A função central do loop do shell, a função para verificar separadores de comando, as funções para executar os comandos no modo sequencial ou paralelo e as funções que servem para executar os comandos com separadores diversos.

### O código foi implementado num sistema operacional LINUX