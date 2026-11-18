# Parte B - Respostas e observações dos laboratórios

## L1 - `static` na prática

A função `proximo_id()` usa uma variável local estática que começa em zero e aumenta em 1 a cada chamada. Cinco chamadas retornam:

```text
1 2 3 4 5
```

No arquivo `exemplos_com_bug/l1_sem_static.c`, o `static` foi removido. A variável local passa a ser reinicializada com zero em cada chamada, e a saída muda para `1 1 1 1 1`.

## L2 - Vetor de notas

O tamanho `n` é informado durante a execução. `malloc` cria um vetor de `n` elementos do tipo `double`, que recebe as notas. O programa soma as notas, calcula `soma / n` e executa `free(notas)` ao final. Quando `n = 4` e as notas são `7, 8, 6, 9`, a média é **7,50**.

A verificação de `malloc == NULL` evita acessar memória caso a alocação falhe.

## L3 - Sequência no heap

A função `cria_sequencia(n)` devolve um ponteiro válido para um bloco **alocado dinamicamente**, que continua existindo depois que a função retorna. Isso é diferente de devolver `&x` para uma variável local automática, cujo tempo de vida termina quando a função acaba.

**Quem deve executar `free`?** Quem recebe o ponteiro e passa a ser responsável pelo bloco — neste caso, a função `main()`.

## L4 - Vazamento de memória

**Sem ferramentas, dá para perceber o vazamento apenas rodando o programa?** Em geral, não. O programa pode encerrar normalmente mesmo sem liberar a memória. O Valgrind registra os blocos não liberados e ajuda a localizar o problema.

**Quantos bytes vazaram?** `1000 * sizeof(int)` bytes. Em plataformas onde `int` possui 4 bytes, o resultado é **4.000 bytes**.

O código com erro está em `exemplos_com_bug/l4_vazamento.c`; a solução que executa `free(bloco)` está em `parte_b/l4.c`.

## L5 - Uso após liberar

Na versão com defeito, `v[2]` é acessado **após** `free(v)`. Isso é um acesso a memória já liberada (**heap-use-after-free**), que o AddressSanitizer consegue diagnosticar.

Na versão corrigida, `v[2]` é lido enquanto o bloco ainda pertence ao programa. Em seguida, `free(v)` libera o bloco e `v` é definido como `NULL`.

**Diferença entre L4 e L5:** no L4 existe memória **não devolvida** ao sistema de gerenciamento de memória (vazamento); no L5 existe memória **já devolvida, mas acessada indevidamente** (uso após liberação).

## Discussão final - O que o compilador detecta?

- `-Wall -Wextra` podem revelar vários problemas em tempo de compilação: variáveis não usadas, conversões suspeitas e, dependendo do compilador e da otimização, possíveis acessos inválidos. Não há garantia de identificar todo bug de memória.
- Os erros que dependem da execução, como **vazamentos**, **uso após `free`** e muitos casos de **double-free**, frequentemente precisam de ferramentas em tempo de execução como **Valgrind** ou **AddressSanitizer**.
- **Regra de ouro:** cada `malloc` bem-sucedido deve ter um `free` correspondente no caminho apropriado, e nenhum acesso ao bloco pode ocorrer depois da liberação.

Consulte o `README.md` para os comandos de compilação e execução dos testes.
