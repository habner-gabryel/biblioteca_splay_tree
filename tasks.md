# Biblioteca Splay Tree em C — Guia Completo de Tasks (100% online)

Todo o desenvolvimento acontece **num único arquivo** colado em um compilador C online. Nada de instalação local.

**Ferramenta:** https://www.onlinegdb.com/online_c_compiler  
**Linguagem:** selecione **C** (canto superior direito).  
**Motivo do arquivo único:** a versão gratuita do OnlineGDB aceita só um arquivo por vez. Todo o código (tipos + implementação + testes + `main`) fica num único `.c`.

**Fluxo de trabalho (repete em toda fase):**
1. Editar o arquivo local `splay.c`.
2. Copiar tudo e colar no OnlineGDB.
3. Clicar em **Run**.
4. Se aparecerem os `[OK]` esperados e nenhum `Assertion failed`, fase concluída.
5. Ao final da disciplina, se quiser entregar em `.h` + `.c` separados, é só dividir — o conteúdo é o mesmo.

---

## Fase 0 — Setup

### Tasks

- [ ] **0.1** Abrir https://www.onlinegdb.com/online_c_compiler.
- [ ] **0.2** Trocar a linguagem para **C**.
- [ ] **0.3** Criar localmente o arquivo único `splay.c` que servirá de fonte da verdade.
- [ ] **0.4** Adicionar no topo do arquivo o esqueleto base:

```c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

/* ... código das fases ... */

int main(void) {
    printf("Hello splay\n");
    return 0;
}
```

- [ ] **0.5** Colar no OnlineGDB e apertar **Run**. Deve imprimir `Hello splay`. 
- [ ] **0.6** Criar localmente um `README.md` (será escrito na Fase 7).

**Teste da fase:** o OnlineGDB roda sem erros e imprime `Hello splay`.

---

## Fase 1 — Estruturas base

### Tasks

- [ ] **1.1** Definir `struct SplayNode` (chave `int`, ponteiros `esq`, `dir`, `pai`).
- [ ] **1.2** Definir `struct SplayTree` (`raiz`, `tamanho`).
- [ ] **1.3** Implementar `splay_criar()`.
- [ ] **1.4** Implementar `splay_liberar()` (recursiva, aceita `NULL`).
- [ ] **1.5** Implementar `splay_vazia()` e `splay_tamanho()` (aceitam `NULL`).
- [ ] **1.6** Implementar `splay_criar_no(chave)`.
- [ ] **1.7** Implementar `splay_debug_imprimir()` — imprime em pré-ordem com indentação.
- [ ] **1.8** Substituir o `main` pelos testes da Fase 1.

### Código completo

```c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

/* ---------------- Tipos ---------------- */
typedef struct SplayNode {
    int                chave;
    struct SplayNode  *esq;
    struct SplayNode  *dir;
    struct SplayNode  *pai;
} SplayNode;

typedef struct {
    SplayNode *raiz;
    size_t     tamanho;
} SplayTree;

typedef void (*splay_visitor_fn)(int chave, void *ctx);

/* ---------------- Protótipos ---------------- */
SplayTree *splay_criar(void);
void       splay_liberar(SplayTree *arv);
int        splay_vazia(const SplayTree *arv);
size_t     splay_tamanho(const SplayTree *arv);
SplayNode *splay_criar_no(int chave);
void       splay_debug_imprimir(const SplayTree *arv);

/* ---------------- Implementação ---------------- */

SplayNode *splay_criar_no(int chave)
{
    SplayNode *n = (SplayNode *)malloc(sizeof(SplayNode));
    if (!n) return NULL;
    n->chave = chave;
    n->esq   = NULL;
    n->dir   = NULL;
    n->pai   = NULL;
    return n;
}

SplayTree *splay_criar(void)
{
    SplayTree *arv = (SplayTree *)malloc(sizeof(SplayTree));
    if (!arv) return NULL;
    arv->raiz    = NULL;
    arv->tamanho = 0;
    return arv;
}

static void liberar_subarvore(SplayNode *no)
{
    if (!no) return;
    liberar_subarvore(no->esq);
    liberar_subarvore(no->dir);
    free(no);
}

void splay_liberar(SplayTree *arv)
{
    if (!arv) return;
    liberar_subarvore(arv->raiz);
    free(arv);
}

int splay_vazia(const SplayTree *arv)
{
    return (arv == NULL) || (arv->raiz == NULL);
}

size_t splay_tamanho(const SplayTree *arv)
{
    return arv ? arv->tamanho : 0;
}

static void imprimir_rec(const SplayNode *no, int nivel)
{
    if (!no) return;
    for (int i = 0; i < nivel; i++) printf("    ");
    printf("%d\n", no->chave);
    imprimir_rec(no->esq, nivel + 1);
    imprimir_rec(no->dir, nivel + 1);
}

void splay_debug_imprimir(const SplayTree *arv)
{
    if (!arv || !arv->raiz) { printf("(arvore vazia)\n"); return; }
    imprimir_rec(arv->raiz, 0);
}

/* ---------------- Testes Fase 1 ---------------- */

static void teste_criar_vazia(void)
{
    SplayTree *t = splay_criar();
    assert(t != NULL);
    assert(splay_vazia(t) == 1);
    assert(splay_tamanho(t) == 0);
    splay_liberar(t);
    printf("[OK] criar / vazia / tamanho\n");
}

static void teste_criar_no(void)
{
    SplayNode *n = splay_criar_no(42);
    assert(n && n->chave == 42);
    assert(n->esq == NULL && n->dir == NULL && n->pai == NULL);
    free(n);
    printf("[OK] criar_no\n");
}

static void teste_null_safe(void)
{
    splay_liberar(NULL);
    assert(splay_vazia(NULL) == 1);
    assert(splay_tamanho(NULL) == 0);
    printf("[OK] funcoes aceitam NULL\n");
}

static void teste_estrutura_manual(void)
{
    SplayTree *t = splay_criar();
    SplayNode *a = splay_criar_no(10);
    SplayNode *b = splay_criar_no(5);
    SplayNode *c = splay_criar_no(15);
    a->esq = b; b->pai = a;
    a->dir = c; c->pai = a;
    t->raiz = a; t->tamanho = 3;

    assert(splay_tamanho(t) == 3);
    printf("--- estrutura manual ---\n");
    splay_debug_imprimir(t);
    printf("------------------------\n");

    splay_liberar(t);
    printf("[OK] estrutura manual + liberar\n");
}

int main(void)
{
    printf("=== Testes Fase 1 ===\n");
    teste_criar_vazia();
    teste_criar_no();
    teste_null_safe();
    teste_estrutura_manual();
    printf("\nFase 1 concluida.\n");
    return 0;
}
```

**Saída esperada no OnlineGDB:**
```
=== Testes Fase 1 ===
[OK] criar / vazia / tamanho
[OK] criar_no
[OK] funcoes aceitam NULL
--- estrutura manual ---
10
    5
    15
------------------------
[OK] estrutura manual + liberar

Fase 1 concluida.
```

---

## Fase 2 — Rotações e Splay

### Tasks

- [ ] **2.1** `rotacao_direita(arv, x)` — o filho esquerdo de `x` sobe.
- [ ] **2.2** `rotacao_esquerda(arv, x)` — o filho direito de `x` sobe.
- [ ] **2.3** `splay(arv, x)` — sobe `x` até a raiz tratando os 4 casos: **zig**, **zig-zig**, **zig-zag**.
- [ ] **2.4** Atualizar `arv->raiz` quando necessário.
- [ ] **2.5** Testar cada caso com uma árvore montada manualmente.

### Código para adicionar

```c
/* ---------------- Fase 2: rotações ---------------- */

void rotacao_direita(SplayTree *arv, SplayNode *x)
{
    /* x é o pai; seu filho esquerdo y sobe */
    SplayNode *y = x->esq;
    if (!y) return;

    x->esq = y->dir;
    if (y->dir) y->dir->pai = x;

    y->pai = x->pai;
    if (!x->pai)                    arv->raiz = y;
    else if (x == x->pai->esq)      x->pai->esq = y;
    else                            x->pai->dir = y;

    y->dir = x;
    x->pai = y;
}

void rotacao_esquerda(SplayTree *arv, SplayNode *x)
{
    SplayNode *y = x->dir;
    if (!y) return;

    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;

    y->pai = x->pai;
    if (!x->pai)                    arv->raiz = y;
    else if (x == x->pai->esq)      x->pai->esq = y;
    else                            x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

void splay(SplayTree *arv, SplayNode *x)
{
    while (x->pai) {
        SplayNode *p = x->pai;
        SplayNode *g = p->pai;

        if (!g) {
            /* Zig */
            if (x == p->esq) rotacao_direita(arv, p);
            else             rotacao_esquerda(arv, p);
        } else if (x == p->esq && p == g->esq) {
            /* Zig-zig esquerda */
            rotacao_direita(arv, g);
            rotacao_direita(arv, p);
        } else if (x == p->dir && p == g->dir) {
            /* Zig-zig direita */
            rotacao_esquerda(arv, g);
            rotacao_esquerda(arv, p);
        } else if (x == p->dir && p == g->esq) {
            /* Zig-zag esquerda-direita */
            rotacao_esquerda(arv, p);
            rotacao_direita(arv, g);
        } else {
            /* Zig-zag direita-esquerda */
            rotacao_direita(arv, p);
            rotacao_esquerda(arv, g);
        }
    }
}
```

Adicione os protótipos no topo:

```c
void rotacao_direita (SplayTree *arv, SplayNode *x);
void rotacao_esquerda(SplayTree *arv, SplayNode *x);
void splay           (SplayTree *arv, SplayNode *x);
```

### Testes para o `main`

```c
static void teste_splay_zig(void)
{
    /* 10 -> 5 ; splay(5) deve deixar 5 como raiz com 10 à direita */
    SplayTree *t = splay_criar();
    SplayNode *a = splay_criar_no(10);
    SplayNode *b = splay_criar_no(5);
    a->esq = b; b->pai = a;
    t->raiz = a; t->tamanho = 2;

    splay(t, b);
    assert(t->raiz == b);
    assert(b->dir == a);
    assert(a->esq == NULL && a->pai == b);

    splay_liberar(t);
    printf("[OK] splay caso ZIG\n");
}

static void teste_splay_zig_zig(void)
{
    /* 30 -> 20 -> 10 (todos esquerdos). splay(10) */
    SplayTree *t = splay_criar();
    SplayNode *a = splay_criar_no(30);
    SplayNode *b = splay_criar_no(20);
    SplayNode *c = splay_criar_no(10);
    a->esq = b; b->pai = a;
    b->esq = c; c->pai = b;
    t->raiz = a; t->tamanho = 3;

    splay(t, c);
    assert(t->raiz == c);
    assert(c->esq == NULL);
    assert(c->dir == b);
    assert(b->esq == NULL);
    assert(b->dir == a);

    splay_liberar(t);
    printf("[OK] splay caso ZIG-ZIG\n");
}

static void teste_splay_zig_zag(void)
{
    /* 30 -> 20 (esq) -> 25 (dir). splay(25) */
    SplayTree *t = splay_criar();
    SplayNode *a = splay_criar_no(30);
    SplayNode *b = splay_criar_no(20);
    SplayNode *c = splay_criar_no(25);
    a->esq = b; b->pai = a;
    b->dir = c; c->pai = b;
    t->raiz = a; t->tamanho = 3;

    splay(t, c);
    assert(t->raiz == c);
    assert(c->esq == b);
    assert(c->dir == a);
    assert(b->esq == NULL && b->dir == NULL);
    assert(a->esq == NULL && a->dir == NULL);

    splay_liberar(t);
    printf("[OK] splay caso ZIG-ZAG\n");
}
```

Adicione essas três chamadas no `main` da fase.

**Saída esperada:**
```
[OK] splay caso ZIG
[OK] splay caso ZIG-ZIG
[OK] splay caso ZIG-ZAG
```

---

## Fase 3 — Inserir, Buscar, Remover

### Tasks

- [ ] **3.1** `splay_inserir(arv, chave)` — BST + splay do nó inserido. Duplicata = ignora.
- [ ] **3.2** `splay_buscar(arv, chave)` — busca + splay se encontrar.
- [ ] **3.3** `splay_remover(arv, chave)` — busca (já faz splay) + junta subárvores.

### Código para adicionar

```c
/* ---------------- Fase 3 ---------------- */

int splay_inserir(SplayTree *arv, int chave)
{
    SplayNode *n = splay_criar_no(chave);
    if (!n) return 0;

    if (!arv->raiz) {
        arv->raiz = n;
        arv->tamanho = 1;
        return 1;
    }

    SplayNode *atual = arv->raiz;
    while (1) {
        if (chave < atual->chave) {
            if (!atual->esq) { atual->esq = n; n->pai = atual; break; }
            atual = atual->esq;
        } else if (chave > atual->chave) {
            if (!atual->dir) { atual->dir = n; n->pai = atual; break; }
            atual = atual->dir;
        } else {
            free(n);
            splay(arv, atual);
            return 0;
        }
    }
    arv->tamanho++;
    splay(arv, n);
    return 1;
}

SplayNode *splay_buscar(SplayTree *arv, int chave)
{
    SplayNode *atual = arv->raiz;
    while (atual && atual->chave != chave) {
        atual = (chave < atual->chave) ? atual->esq : atual->dir;
    }
    if (atual) splay(arv, atual);
    return atual;
}

int splay_remover(SplayTree *arv, int chave)
{
    SplayNode *n = splay_buscar(arv, chave);
    if (!n) return 0;

    SplayNode *esq = n->esq;
    SplayNode *dir = n->dir;
    if (esq) esq->pai = NULL;
    if (dir) dir->pai = NULL;
    free(n);
    arv->tamanho--;

    if (!esq) {
        arv->raiz = dir;
    } else {
        arv->raiz = esq;
        SplayNode *max_esq = esq;
        while (max_esq->dir) max_esq = max_esq->dir;
        splay(arv, max_esq);
        arv->raiz->dir = dir;
        if (dir) dir->pai = arv->raiz;
    }
    return 1;
}
```

### Teste para o `main`

```c
static void teste_inserir_buscar_remover(void)
{
    SplayTree *t = splay_criar();
    int v[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) splay_inserir(t, v[i]);
    assert(splay_tamanho(t) == 7);
    assert(t->raiz->chave == 80); /* último inserido subiu */

    SplayNode *b = splay_buscar(t, 40);
    assert(b != NULL && b->chave == 40);
    assert(t->raiz == b);

    assert(splay_buscar(t, 999) == NULL);
    assert(splay_remover(t, 30) == 1);
    assert(splay_tamanho(t) == 6);
    assert(splay_remover(t, 999) == 0);

    splay_liberar(t);
    printf("[OK] inserir / buscar / remover\n");
}
```

---

## Fase 4 — Auxiliares

### Tasks

- [ ] **4.1** `splay_minimo()` / `splay_maximo()`.
- [ ] **4.2** `splay_predecessor()` / `splay_sucessor()`.
- [ ] **4.3** `splay_altura()`.

### Código

```c
/* ---------------- Fase 4 ---------------- */

SplayNode *splay_minimo(SplayTree *arv)
{
    if (!arv->raiz) return NULL;
    SplayNode *n = arv->raiz;
    while (n->esq) n = n->esq;
    splay(arv, n);
    return n;
}

SplayNode *splay_maximo(SplayTree *arv)
{
    if (!arv->raiz) return NULL;
    SplayNode *n = arv->raiz;
    while (n->dir) n = n->dir;
    splay(arv, n);
    return n;
}

SplayNode *splay_predecessor(SplayTree *arv, int chave)
{
    SplayNode *n = splay_buscar(arv, chave);
    if (!n) return NULL;
    if (n->esq) {
        SplayNode *p = n->esq;
        while (p->dir) p = p->dir;
        return p;
    }
    return NULL;
}

SplayNode *splay_sucessor(SplayTree *arv, int chave)
{
    SplayNode *n = splay_buscar(arv, chave);
    if (!n) return NULL;
    if (n->dir) {
        SplayNode *s = n->dir;
        while (s->esq) s = s->esq;
        return s;
    }
    return NULL;
}

static int altura_rec(const SplayNode *no)
{
    if (!no) return -1;
    int he = altura_rec(no->esq);
    int hd = altura_rec(no->dir);
    return 1 + (he > hd ? he : hd);
}

int splay_altura(const SplayTree *arv)
{
    return arv ? altura_rec(arv->raiz) : -1;
}
```

---

## Fase 5 — Percursos

### Tasks

- [ ] **5.1** `splay_em_ordem`, `splay_pre_ordem`, `splay_pos_ordem` com callback `(int chave, void *ctx)`.
- [ ] **5.2** Teste: em-ordem de uma árvore populada deve sair em ordem crescente.

### Código

```c
/* ---------------- Fase 5 ---------------- */

static void em_ordem_rec(const SplayNode *no, splay_visitor_fn visit, void *ctx)
{
    if (!no) return;
    em_ordem_rec(no->esq, visit, ctx);
    visit(no->chave, ctx);
    em_ordem_rec(no->dir, visit, ctx);
}

static void pre_ordem_rec(const SplayNode *no, splay_visitor_fn visit, void *ctx)
{
    if (!no) return;
    visit(no->chave, ctx);
    pre_ordem_rec(no->esq, visit, ctx);
    pre_ordem_rec(no->dir, visit, ctx);
}

static void pos_ordem_rec(const SplayNode *no, splay_visitor_fn visit, void *ctx)
{
    if (!no) return;
    pos_ordem_rec(no->esq, visit, ctx);
    pos_ordem_rec(no->dir, visit, ctx);
    visit(no->chave, ctx);
}

void splay_em_ordem (const SplayTree *arv, splay_visitor_fn v, void *ctx)
{ if (arv) em_ordem_rec (arv->raiz, v, ctx); }

void splay_pre_ordem(const SplayTree *arv, splay_visitor_fn v, void *ctx)
{ if (arv) pre_ordem_rec(arv->raiz, v, ctx); }

void splay_pos_ordem(const SplayTree *arv, splay_visitor_fn v, void *ctx)
{ if (arv) pos_ordem_rec(arv->raiz, v, ctx); }
```

### Teste

```c
static void imprimir_chave(int chave, void *ctx)
{
    (void)ctx;
    printf("%d ", chave);
}

static void teste_percursos(void)
{
    SplayTree *t = splay_criar();
    int v[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) splay_inserir(t, v[i]);

    printf("em ordem : "); splay_em_ordem (t, imprimir_chave, NULL); printf("\n");
    printf("pre ordem: "); splay_pre_ordem(t, imprimir_chave, NULL); printf("\n");
    printf("pos ordem: "); splay_pos_ordem(t, imprimir_chave, NULL); printf("\n");

    splay_liberar(t);
    printf("[OK] percursos\n");
}
```

O **em-ordem** deve imprimir `20 30 40 50 60 70 80`.

---

## Fase 6 — Testes e Validação

### Tasks

- [ ] **6.1** Verificar invariante BST em toda a árvore.
- [ ] **6.2** Verificar invariante `filho->pai == pai`.
- [ ] **6.3** Verificar altura após inserir 1..N ordenado (deve ficar pequena).
- [ ] **6.4** Estresse com 10.000 inserções/buscas/remoções aleatórias.
- [ ] **6.5** Contador de alocação (valgrind caseiro) para detectar leak.

### Código

```c
/* ---------------- Fase 6 ---------------- */

static int checar_bst_rec(const SplayNode *no, long min, long max, int tem_min, int tem_max)
{
    if (!no) return 1;
    if (tem_min && no->chave <= min) return 0;
    if (tem_max && no->chave >= max) return 0;
    return checar_bst_rec(no->esq, min, no->chave, tem_min, 1)
        && checar_bst_rec(no->dir, no->chave, max, 1, tem_max);
}

int splay_checar_bst(const SplayTree *arv)
{
    if (!arv) return 1;
    return checar_bst_rec(arv->raiz, 0, 0, 0, 0);
}

static int checar_pais_rec(const SplayNode *no)
{
    if (!no) return 1;
    if (no->esq && no->esq->pai != no) return 0;
    if (no->dir && no->dir->pai != no) return 0;
    return checar_pais_rec(no->esq) && checar_pais_rec(no->dir);
}

int splay_checar_pais(const SplayTree *arv)
{
    if (!arv) return 1;
    if (arv->raiz && arv->raiz->pai != NULL) return 0;
    return checar_pais_rec(arv->raiz);
}

/* Contador de alocação (valgrind caseiro) */
static long alocacoes_ativas = 0;

static void teste_estresse(void)
{
    SplayTree *t = splay_criar();
    srand(42);

    for (int i = 0; i < 10000; i++) {
        splay_inserir(t, rand() % 100000);
        assert(splay_checar_bst(t));
        assert(splay_checar_pais(t));
    }
    for (int i = 0; i < 5000; i++) {
        splay_buscar(t, rand() % 100000);
    }
    for (int i = 0; i < 5000; i++) {
        splay_remover(t, rand() % 100000);
        assert(splay_checar_bst(t));
        assert(splay_checar_pais(t));
    }

    printf("[OK] estresse (tamanho final = %zu, altura = %d)\n",
           splay_tamanho(t), splay_altura(t));
    splay_liberar(t);
}

static void teste_ordenado_nao_degenera(void)
{
    SplayTree *t = splay_criar();
    for (int i = 1; i <= 1000; i++) splay_inserir(t, i);
    int h = splay_altura(t);
    assert(h < 100); /* sem splay, seria 999 */
    printf("[OK] inserir ordenado: altura = %d (esperado < 100)\n", h);
    splay_liberar(t);
}
```

> **Sobre leaks:** o OnlineGDB **não roda valgrind**. Estratégia: instrumentar `malloc`/`free`.

```c
/* Substitua malloc/free por wrappers para contar alocações */
static long g_aloc = 0;
static void *meu_malloc(size_t n) { g_aloc++; return malloc(n); }
static void  meu_free(void *p)    { if (p) { g_aloc--; free(p); } }
/* e use #define malloc(n) meu_malloc(n) / #define free(p) meu_free(p) no topo */
```

Ao final do `main`, imprimir `g_aloc`. Deve ser **0**.

---

## Fase 7 — Documentação e Entrega

### Tasks

- [ ] **7.1** Comentários Doxygen-like em cada função pública (pré/pós-condições, complexidade).
- [ ] **7.2** Escrever `README.md`.
- [ ] **7.3** Dividir o arquivo único em `splay.h` + `splay.c` + `testes.c` para entrega formal.
- [ ] **7.4** Checklist final:
  - Compila sem warnings no OnlineGDB? 
  - Todos os `[OK]` aparecem?
  - Nenhum `Assertion failed`?
  - Contador de alocação termina em 0?
  - README explica como rodar?

### Modelo de `README.md`

```markdown
# Biblioteca Splay Tree (C)

Implementação de uma Splay Tree (árvore binária auto-ajustável) em C.

## Como executar

Todo o código está em `splay.c` (arquivo único, tipos + implementação + testes).
Para rodar:

1. Acesse https://www.onlinegdb.com/online_c_compiler
2. Selecione a linguagem **C**.
3. Cole o conteúdo de `splay.c` no editor.
4. Clique em **Run**.

## Estrutura

- `splay.c` — código completo (tipos, funções, testes, main)
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
```

---

## Ciclo resumido (repete em toda fase)

```
1. Editar o arquivo único no seu editor local
2. Colar tudo no OnlineGDB (linguagem C)
3. Run
4. Todos os [OK]? Nenhum assert falhou? → fase concluída
5. Avançar para a próxima fase
```

**Ordem de execução recomendada:** Fase 1 → 2 → 3 → 4 → 5 → 6 → 7, **sem pular**. A Fase 2 (splay) é a mais crítica; se algo ali estiver errado, todas as fases seguintes falham de forma confusa.