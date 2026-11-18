#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int *cria_sequencia(int n) {
    if (n <= 0 || (size_t)n > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    int *sequencia = malloc((size_t)n * sizeof *sequencia);
    if (sequencia == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        sequencia[i] = i + 1;
    }
    return sequencia;
}

int main(void) {
    const int n = 5;
    int *v = cria_sequencia(n);
    if (v == NULL) {
        fprintf(stderr, "Nao foi possivel criar a sequencia.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%d%s", v[i], i == n - 1 ? "\n" : " ");
    }

    free(v);
    return 0;
}
