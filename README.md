# Unidade 6 - Escopo, tempo de vida e memória em C

Atividade de **escopo e tempo de vida: caça ao bug e teste de mesa**, organizada para estudo e entrega via Git/GitHub.

O material acompanha os exercícios propostos na Unidade 6, divididos em:

- **Parte A:** respostas comentadas de 12 questões de teste de mesa e identificação de bugs.
- **Parte B:** soluções dos laboratórios L1 a L5, com código em C.
- **Exemplos com bug:** versões intencionalmente incorretas para observar os erros solicitados na atividade (sem usá-las como soluções finais).

## Organização dos arquivos

```text
atividade-u6-c/
├── README.md
├── PARTE_A.md
├── RESPOSTAS_LAB.md
├── Makefile
├── .gitignore
├── parte_b/
│   ├── l1.c                # Contador de IDs com static
│   ├── l2.c                # Média de notas com malloc e free
│   ├── l3.c                # Sequência 1..n criada no heap
│   ├── l4.c                # Vazamento corrigido
│   └── l5.c                # Uso após free corrigido
└── exemplos_com_bug/
    ├── l1_sem_static.c     # Mostra o que acontece ao tirar static
    ├── l4_vazamento.c      # Vazamento de memória intencional
    └── l5_uso_apos_free.c  # Use-after-free intencional
```

## Pré-requisitos

- Compilador GCC com suporte a C11.
- Terminal Linux ou WSL no Windows para usar as ferramentas de memória como proposto no enunciado.
- `make` (opcional, para compilar todos de uma vez).
- Valgrind (para investigar o exercício L4, quando disponível).
- AddressSanitizer, incluído em instalações comuns do GCC (para investigar L5).

## Compilar e executar

Na pasta principal do projeto, rode:

```bash
make
```

Isso gera os executáveis em `build/`. Para executar cada laboratório:

```bash
./build/l1
./build/l2
./build/l3
./build/l4
./build/l5
```

Sem `make`, você pode compilar individualmente. Exemplo com L1:

```bash
gcc -std=c11 -Wall -Wextra parte_b/l1.c -o l1
./l1
```

Para apagar os executáveis gerados pelo Makefile:

```bash
make clean
```

## Parte B: o que cada programa faz

| Arquivo | Objetivo | Resultado principal |
| --- | --- | --- |
| `l1.c` | Gerar IDs com variável `static` | `1 2 3 4 5` |
| `l2.c` | Alocar notas em tempo de execução | Exemplo: notas 7, 8, 6, 9 → `Media: 7.50` |
| `l3.c` | Devolver um vetor alocado no heap | `1 2 3 4 5` |
| `l4.c` | Liberar corretamente o bloco alocado | Sem vazamento de memória |
| `l5.c` | Acessar o vetor antes do `free` | Sem acesso após liberar memória |

Os programas corrigidos verificam se `malloc` falhou antes de usar o ponteiro. O L2 também verifica a entrada e libera o vetor após o cálculo.

## Testar os bugs pedidos pelo enunciado

**Atenção:** os exemplos da pasta `exemplos_com_bug/` contêm erros **de propósito**. Não são versões para usar em um programa real.

### L1 - Sem `static`

```bash
gcc -std=c11 -Wall -Wextra exemplos_com_bug/l1_sem_static.c -o l1_sem_static
./l1_sem_static
```

Saída: `1 1 1 1 1`, pois a variável local `n` começa em zero em cada chamada.

### L4 - Vazamento de memória (Valgrind)

```bash
gcc -std=c11 -Wall -Wextra -g exemplos_com_bug/l4_vazamento.c -o l4_vazamento
valgrind --leak-check=full ./l4_vazamento
```

O programa aloca 1.000 inteiros e não executa `free`. Em uma plataforma com `sizeof(int) == 4`, são **4.000 bytes** perdidos. Para comparar com a versão corrigida:

```bash
gcc -std=c11 -Wall -Wextra -g parte_b/l4.c -o l4_corrigido
valgrind --leak-check=full ./l4_corrigido
```

O relatório da versão corrigida deve indicar que todos os blocos alocados foram liberados.

### L5 - Uso após liberar (AddressSanitizer)

```bash
gcc -std=c11 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer exemplos_com_bug/l5_uso_apos_free.c -o l5_bug
./l5_bug
```

A versão com bug deve produzir diagnóstico de **`heap-use-after-free`**, mostrando o acesso a `v[2]` depois de `free(v)`. Compare com:

```bash
gcc -std=c11 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer parte_b/l5.c -o l5_corrigido
./l5_corrigido
```

Na versão corrigida, `v[2]` é lido **antes** de `free(v)`; depois da liberação, o ponteiro recebe `NULL`.

> A execução de código com comportamento indefinido pode variar. O sanitizer é uma ferramenta de diagnóstico: ele torna o problema observável, mas o resultado de uma execução sem o sanitizer não é confiável.

## Respostas e explicações

- [Parte A: questões 1 a 12](PARTE_A.md)
- [Parte B: perguntas dos laboratórios e discussão final](RESPOSTAS_LAB.md)

## Enviar o projeto para o GitHub

Crie um repositório vazio no GitHub e, no terminal, dentro desta pasta, execute:

```bash
git init
git add .
git commit -m "Resolve atividade da Unidade 6 em C"
git branch -M main
git remote add origin https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git
git push -u origin main
```

Substitua `SEU-USUARIO/SEU-REPOSITORIO` pelo endereço real do seu repositório. Se ele já existir localmente, basta copiar os arquivos, executar `git add .`, `git commit` e `git push`.

## Conceitos-chave

- **Escopo:** região do código na qual um identificador está visível.
- **Tempo de vida:** intervalo durante o qual o objeto existe.
- **`static`:** mantém o objeto durante toda a execução do programa, mesmo quando seu nome só é visível dentro de uma função.
- **Pilha (stack):** armazenamento usual de variáveis locais automáticas enquanto a função/bloco está em execução.
- **Heap:** memória dinâmica obtida com `malloc`, cuja liberação é responsabilidade do programa.
- **Boa prática:** cada alocação bem-sucedida deve ter uma liberação correspondente, e a memória não pode ser acessada depois de liberada.

---

Atividade baseada no PDF fornecido para a **Unidade 6 - Escopo e tempo de vida: caça ao bug e teste de mesa**.
