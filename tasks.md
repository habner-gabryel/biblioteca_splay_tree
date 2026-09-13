# Guia de Tasks — Biblioteca Splay Tree em C

Vou dividir em **7 fases** com tasks atômicas. A ideia é você conseguir testar cada fase antes de avançar. Marque conforme for concluindo.

---

## Fase 0 — Planejamento e Setup

- [ ] **0.1** Definir o escopo da API (quais operações a biblioteca vai expor).
- [ ] **0.2** Decidir o tipo de chave: `int` fixo, ou genérico com `void*` + função de comparação (`typedef int (*cmp_fn)(const void*, const void*)`). *Recomendo começar com `int` e generalizar depois se sobrar tempo.*
- [ ] **0.3** Estrutura de arquivos:
  ```
  splay.h      -> API pública
  splay.c      -> implementação
  testes.c     -> testes unitários
  Makefile
  README.md
  ```
- [ ] **0.4** Escrever o Makefile (targets: `all`, `test`, `clean`, com `-Wall -Wextra -g`).
- [ ] **0.5** Definir a estratégia de splay: **bottom-up** (com ponteiro para pai) ou **top-down** (sem ponteiro para pai, mais elegante). *Para trabalho acadêmico, bottom-up costuma ser mais didático.*

---

## Fase 1 — Estruturas Base

- [ ] **1.1** Definir `struct Node` com: `chave`, `esq`, `dir`, `pai` (se bottom-up).
- [ ] **1.2** Definir `struct SplayTree` com: `raiz`, `tamanho`.
- [ ] **1.3** Implementar `splay_criar()` — retorna árvore vazia.
- [ ] **1.4** Implementar `splay_liberar()` — libera recursivamente todos os nós.
- [ ] **1.5** Implementar `splay_vazia()`, `splay_tamanho()`.
- [ ] **1.6** Criar função auxiliar `criar_no(chave)`.

**Teste da fase:** criar, verificar vazia, liberar. Rodar com Valgrind sem leaks.

---

## Fase 2 — Rotações e Splay (o coração)

- [ ] **2.1** `rotacao_direita(arvore, x)` — atualizar ponteiros de pai/filhos **com cuidado**.
- [ ] **2.2** `rotacao_esquerda(arvore, x)` — simétrica.
- [ ] **2.3** `splay(arvore, x)` — subir `x` até a raiz com os 3 casos:
  - **Zig:** pai é raiz → 1 rotação.
  - **Zig-zig:** x e pai ambos filhos esquerdos (ou ambos direitos) → rotaciona pai primeiro, depois x.
  - **Zig-zag:** x é filho esquerdo e pai filho direito (ou vice-versa) → rotaciona x duas vezes.
- [ ] **2.4** Atualizar `arvore->raiz` ao final do splay.

**Teste da fase:** inserir manualmente uma BST pequena (7–10 nós), chamar splay em um nó e imprimir em pré-ordem para conferir visualmente. **Essa é a parte que mais dá bug** — teste com calma.

⚠️ **Pegadinha comum:** esquecer de atualizar `pai` do nó que sobe, ou o `pai` dos filhos que mudam de subárvore.

---

## Fase 3 — Operações Principais

- [ ] **3.1** `splay_inserir(arvore, chave)`:
  - Inserção BST normal + `splay` do nó inserido.
  - Tratar chave duplicada (ignorar, atualizar ou retornar erro — decida a política).
- [ ] **3.2** `splay_buscar(arvore, chave)`:
  - Busca BST; se achou, faz splay e retorna o nó; senão, faz splay do último nó visitado (opcional) e retorna `NULL`.
- [ ] **3.3** `splay_remover(arvore, chave)`:
  - Buscar + splay do nó.
  - Remover raiz: `join(esq, dir)` onde todos da esquerda < todos da direita.
  - Implementar `join` separado (splay do máximo da subárvore esquerda, liga com a direita).

**Teste da fase:** inserir 20 valores aleatórios, buscar alguns, remover alguns. Validar com percurso in-ordem ordenado.

---

## Fase 4 — Operações Auxiliares

- [ ] **4.1** `splay_minimo()` / `splay_maximo()`.
- [ ] **4.2** `splay_predecessor(chave)` / `splay_sucessor(chave)`.
- [ ] **4.3** `splay_altura()` — recursiva ou iterativa.
- [ ] **4.4** (Opcional) `splay_contem(chave)`.

---

## Fase 5 — Percursos

- [ ] **5.1** `em_ordem(arvore, callback)` — recursivo.
- [ ] **5.2** `pre_ordem(arvore, callback)`.
- [ ] **5.3** `pos_ordem(arvore, callback)`.
- [ ] **5.4** (Bônus) Versão iterativa do em-ordem com pilha.
- [ ] **5.5** Usar uma função de callback genérica: `void (*visitar)(int chave)` — deixa a biblioteca flexível.

---

## Fase 6 — Testes e Validação

- [ ] **6.1** Testes unitários por operação (use `assert` ou um mini-framework).
- [ ] **6.2** **Invariante BST:** função que percorre e verifica se cada nó respeita a ordem.
- [ ] **6.3** **Invariante pai/filho:** `filho->pai == no` para todo filho não nulo.
- [ ] **6.4** Teste de estresse: inserir/buscar/remover 10.000+ chaves aleatórias.
- [ ] **6.5** Medir altura após sequência ordenada (1,2,3,...,N) — deve ficar pequena graças ao splay.
- [ ] **6.6** Rodar tudo com `valgrind --leak-check=full` — zero leaks, zero erros.

---

## Fase 7 — Documentação e Entrega

- [ ] **7.1** Comentários Doxygen-like no `splay.h` (pré-condições, pós-condições, complexidade).
- [ ] **7.2** `README.md` com: descrição, como compilar, como rodar testes, exemplos de uso.
- [ ] **7.3** Escrever um `main.c` de demonstração.
- [ ] **7.4** Revisar Makefile final (`all`, `test`, `clean`, `valgrind`).
- [ ] **7.5** Checklist final: compila sem warnings? testa tudo? sem leaks? README ok?

---

## Dicas gerais

1. **Faça commit após cada fase** — se quebrar algo, você volta fácil.
2. **A fase 2 é a mais crítica.** Se o `splay` estiver errado, todo o resto falha de forma misteriosa. Teste exaustivamente antes de seguir.
3. **Desenhe no papel** os casos zig, zig-zig, zig-zag antes de codar. Ajuda demais.
4. **Complexidade amortizada:** documente no README que cada operação é O(log n) *amortizado*, mesmo que uma operação isolada possa ser O(n).
5. Se travar no bottom-up, considere a versão **top-down** (Sleator & Tarjan) — não precisa de ponteiro para pai.