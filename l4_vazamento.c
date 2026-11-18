#include <stdio.h>
#include <stdlib.h>

/* BUG INTENCIONAL: o bloco alocado nao e liberado. */
int aloca(void) {
    int *bloco = malloc(1000 * sizeof *bloco);
    if (bloco == NULL) {
        fprintf(stderr, "Falha na alocacao.\n");
        return 1;
    }

    bloco[0] = 1;
    /* Falta free(bloco); */
    return 0;
}

int main(void) {
    return aloca();
}
