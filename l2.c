#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;

    printf("Quantas notas? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Quantidade de notas invalida.\n");
        return 1;
    }

    if ((size_t)n > SIZE_MAX / sizeof(double)) {
        fprintf(stderr, "Quantidade de notas muito grande.\n");
        return 1;
    }

    double *notas = malloc((size_t)n * sizeof *notas);
    if (notas == NULL) {
        fprintf(stderr, "Nao foi possivel alocar memoria.\n");
        return 1;
    }

    double soma = 0.0;
    printf("Digite as %d notas: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf", &notas[i]) != 1) {
            fprintf(stderr, "Nota invalida.\n");
            free(notas);
            return 1;
        }
        soma += notas[i];
    }

    printf("Media: %.2f\n", soma / n);
    free(notas);
    return 0;
}
