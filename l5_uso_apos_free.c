#include <stdio.h>
#include <stdlib.h>

/* BUG INTENCIONAL: compilado com AddressSanitizer, gera heap-use-after-free. */
int main(void) {
    int *v = malloc(5 * sizeof *v);
    if (v == NULL) {
        fprintf(stderr, "Falha na alocacao.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        v[i] = i + 1;
    }

    free(v);
    printf("v[2] = %d\n", v[2]);  /* Acesso invalido: apos o free. */
    return 0;
}
