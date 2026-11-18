#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *v = malloc(5 * sizeof *v);
    if (v == NULL) {
        fprintf(stderr, "Falha na alocacao.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        v[i] = i + 1;
    }

    printf("v[2] = %d\n", v[2]);  /* Acesso valido: antes de free. */
    free(v);
    v = NULL;
    return 0;
}
