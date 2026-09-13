# Biblioteca Splay Tree (C)

Implementação de uma Splay Tree (árvore binária auto-ajustável) em C.

## Estrutura

- `splay.h` — tipos
- `splay.c` — funções
- `testes.c` — testes e main
- `README.md` — este arquivo

## Decisões

- Chave do tipo `int`.
- Estratégia de splay: *bottom-up* (cada nó tem ponteiro para o pai).
- Complexidade amortizada: O(log n) por operação (Sleator & Tarjan, 1985).

## Fases implementadas

- [x] Estruturas base
- [x] Rotações + splay (zig / zig-zig / zig-zag)
- [x] Inserir / buscar / remover
- [x] Auxiliares (min, max, predecessor, sucessor, altura)
- [x] Percursos (em, pré, pós-ordem)
- [x] Validação (invariantes BST, pai/filho, estresse)
- [x] Documentação