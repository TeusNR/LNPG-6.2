#include <stdio.h>
#include <stdlib.h>

int aloca(void) {
    int *bloco = malloc(1000 * sizeof *bloco);
    if (bloco == NULL) {
        fprintf(stderr, "Falha na alocacao.\n");
        return 1;
    }

    bloco[0] = 1;
    free(bloco);  /* Correcao: libera o bloco antes de retornar. */
    return 0;
}

int main(void) {
    return aloca();
}
