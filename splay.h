#ifndef SPLAY_H
#define SPLAY_H

#include <stddef.h>

/* ============================================================
 *  Biblioteca: Splay Tree (árvore binária auto-ajustável)
 *  Chave: int
 *  Estratégia de splay: bottom-up (nós possuem ponteiro 'pai')
 * ============================================================ */

/* ---------- Tipos ---------- */

typedef struct SplayNode {
    int                chave;
    struct SplayNode  *esq;
    struct SplayNode  *dir;
    struct SplayNode  *pai;   /* NULL para a raiz */
} SplayNode;

typedef struct {
    SplayNode *raiz;
    size_t     tamanho;
} SplayTree;

/* Callback usado nos percursos */
typedef void (*splay_visitor_fn)(int chave, void *ctx);

/* ---------- Fase 1: ciclo de vida ---------- */

SplayTree *splay_criar(void);
void       splay_liberar(SplayTree *arv);
int        splay_vazia(const SplayTree *arv);
size_t     splay_tamanho(const SplayTree *arv);
SplayNode *splay_criar_no(int chave);
void       splay_debug_imprimir(const SplayTree *arv);

/* ---------- Fase 2: rotações e splay ---------- */

void splay_rotacao_direita (SplayTree *arv, SplayNode *x);
void splay_rotacao_esquerda(SplayTree *arv, SplayNode *x);
void splay_splay           (SplayTree *arv, SplayNode *x);

/* ---------- Fase 3: operações principais ---------- */
int         splay_inserir(SplayTree *arv, int chave);
SplayNode  *splay_buscar (SplayTree *arv, int chave);
int         splay_remover(SplayTree *arv, int chave);

/* ---------- Fase 4: auxiliares ---------- */
SplayNode *splay_minimo (SplayTree *arv);
SplayNode *splay_maximo (SplayTree *arv);
SplayNode *splay_predecessor(SplayTree *arv, int chave);
SplayNode *splay_sucessor   (SplayTree *arv, int chave);
int        splay_altura (const SplayTree *arv);

/* ---------- Fase 5: percursos ---------- */
void splay_em_ordem  (const SplayTree *arv, splay_visitor_fn visit, void *ctx);
void splay_pre_ordem (const SplayTree *arv, splay_visitor_fn visit, void *ctx);
void splay_pos_ordem (const SplayTree *arv, splay_visitor_fn visit, void *ctx);

/* ---------- Utilidades internas expostas p/ testes ---------- */

/* Cria um nó isolado (sem filhos e sem pai). */
SplayNode *splay_criar_no(int chave);

/* Imprime a árvore em pré-ordem no stdout (debug). */
void splay_debug_imprimir(const SplayTree *arv);

#endif /* SPLAY_H */