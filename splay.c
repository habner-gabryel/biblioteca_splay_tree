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