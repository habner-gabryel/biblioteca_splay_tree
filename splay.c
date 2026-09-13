#include "splay.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 *  Fase 1 — Estruturas base e ciclo de vida
 * ============================================================ */

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