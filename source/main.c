#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "batch.h"
#include "interativo.h"


int main(int argc, char *argv[]) {

    if (argc == 1) {
        modo_interativo();
    } else if (argc == 2) {
        modo_batch(argv[1]);
    }

    return 0;
}