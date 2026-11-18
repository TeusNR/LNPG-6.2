# Parte A - Respostas do teste de mesa e caça ao bug

## Nível 1 - Fácil

### 1. Contador com `static`

**Saída:**

```text
1 2 3
```

A variável `n` é inicializada uma única vez. Como é `static`, conserva o valor entre chamadas: 1 na primeira, 2 na segunda e 3 na terceira.

### 2. O numerador que não anda

**Saída real:**

```text
ID 1
ID 1
ID 1
```

`n` é uma variável local automática: ela é criada e inicializada com zero em cada chamada. Por isso, sempre imprime 1.

**Correção:** mudar a declaração para `static int n = 0;`, mantendo o restante da função. Assim as próximas chamadas imprimem `ID 1`, `ID 2` e `ID 3`.

### 3. Variável global

**Saída:**

```text
15
```

A variável global `total` começa em zero e recebe `10` e depois `5`, resultando em `15`.

## Nível 2 - Médio

### 4. A função que devolve o endereço errado

A função devolve `&x`, endereço de uma variável local automática. Assim que `cria()` termina, `x` deixa de existir e o ponteiro recebido por `main()` fica **pendente** (*dangling pointer*). Tentar acessar `*p` tem **comportamento indefinido**: não é correto afirmar que sempre imprimirá 42 nem que sempre ocorrerá falha.

**Uma correção:** fazer a função alocar um inteiro no heap, verificar a alocação e devolver seu ponteiro. A função `main()` deverá executar `free(p)` ao terminar de usá-lo.

```c
#include <stdio.h>
#include <stdlib.h>

int *cria(void) {
    int *p = malloc(sizeof *p);
    if (p != NULL) {
        *p = 42;
    }
    return p;
}

int main(void) {
    int *p = cria();
    if (p == NULL) {
        return 1;
    }
    printf("%d\n", *p);
    free(p);
    return 0;
}
```

Outra correção possível é devolver o **valor** `int`, sem ponteiro, caso não seja necessário devolver um endereço.

### 5. Sombreamento (três `x`)

**Saída:**

```text
3 2
```

Dentro do bloco mais interno, a variável `x = 3` esconde a `x = 2` de `main()`. Após o fechamento do bloco, a `x = 2` volta a ser a variável visível. A variável global `x = 1` permanece sombreada dentro de `main()`.

### 6. Usando depois de liberar

O programa executa `free(v)` e depois tenta acessar `v[0]`. Depois do `free`, o bloco não pode mais ser usado; portanto, `printf("%d\n", v[0]);` produz **comportamento indefinido**. Esse erro se chama **use-after-free**, mais especificamente *heap-use-after-free*.

**Correção:** imprimir `v[0]` antes de executar `free(v)`, e opcionalmente atribuir `v = NULL` depois da liberação. Também é importante verificar se `malloc` retornou `NULL`.

## Nível 3 - Difícil

### 7. `static` contra local comum

**Saída:**

```text
s=2 a=2
s=4 a=2
s=6 a=2
```

`static int s` é inicializado apenas uma vez e continua existindo entre chamadas. Já `int a = 0` é inicializado novamente sempre que a função é executada. Em cada chamada, `a` passa de zero para dois.

### 8. O bloco perdido

Foram **2 blocos alocados** e apenas **1 bloco liberado**. Quando o segundo `malloc` substitui o endereço armazenado em `v`, perde-se a referência para o primeiro bloco, que não pode mais ser liberado por esse ponteiro. Isso é um **vazamento de memória** (*memory leak*).

**Correção:** liberar o primeiro bloco antes de fazer outra alocação, ou usar ponteiros separados. Exemplo com ponteiros separados:

```c
#include <stdlib.h>

int main(void) {
    int *primeiro = malloc(100 * sizeof *primeiro);
    int *segundo = malloc(50 * sizeof *segundo);

    if (primeiro == NULL || segundo == NULL) {
        free(primeiro);
        free(segundo);
        return 1;
    }

    free(primeiro);
    free(segundo);
    return 0;
}
```

### 9. Em que região mora cada uma?

| Item | Região da memória | Tempo de vida |
| --- | --- | --- |
| `g` (global) | Dados estáticos | Todo o programa |
| `s` (`static` local) | Dados estáticos | Todo o programa |
| `a` (local automática) | Pilha, no modelo didático | Durante a execução do bloco/função `h` |
| `p` (variável ponteiro) | Pilha, no modelo didático | Durante a execução do bloco/função `h` |
| Bloco de 16 bytes de `malloc` | Heap | Desde a alocação até `free` (ou até o encerramento do processo, se não for liberado) |

**Observação:** o ponteiro `p` é uma variável local; o **bloco apontado por ele** fica no heap. No código do enunciado não há `free(p)`, então há vazamento de memória se a alocação tiver sucesso. Em C, a colocação física exata de variáveis automáticas na pilha depende da implementação/otimizações, mas essa é a classificação esperada na atividade.

## Nível 4 - Muito difícil

### 10. Inicializa uma vez só

**Saída:**

```text
10
11
12
```

`static int n = 10` é inicializado apenas uma vez. O operador `n++` devolve o valor **anterior** à soma, por isso a primeira chamada devolve 10, a segunda 11 e a terceira 12.

### 11. Liberar duas vezes

Há um **double-free**: o programa executa `free(v)` duas vezes para o mesmo endereço, sem uma nova alocação válida entre as chamadas. Isso causa comportamento indefinido e pode resultar em erro da biblioteca de memória ou corrupção do gerenciamento do heap.

**Correção:** executar `free(v)` uma única vez e, opcionalmente, fazer `v = NULL;` em seguida. `free(NULL)` é permitido pela linguagem, mas não substitui a necessidade de controlar corretamente a propriedade da memória.

### 12. Escopo de bloco

**Saída:**

```text
12
```

O laço acumula `2 + 4 + 6 = 12` em `soma`.

Depois do laço, **nenhum** dos comandos `printf("%d", i);` ou `printf("%d", dobro);` compilaria como está no exercício, pois os identificadores já estão fora de seus escopos: `i` foi declarado no cabeçalho do `for` e `dobro` dentro do corpo do laço.
