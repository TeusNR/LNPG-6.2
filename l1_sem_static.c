#include <stdio.h>

/* Exemplo didatico: sem static, o ID nao avanca entre chamadas. */
int proximo_id(void) {
    int id = 0;
    id++;
    return id;
}

int main(void) {
    for (int i = 0; i < 5; i++) {
        printf("%d%s", proximo_id(), i == 4 ? "\n" : " ");
    }
    return 0;
}
