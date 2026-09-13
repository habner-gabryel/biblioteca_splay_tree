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

/* Liberação recursiva (pós-ordem) a partir de um nó. */
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

/* ============================================================
 *  Debug — imprime em pré-ordem com indentação
 *  (útil já na Fase 1 para conferir a estrutura depois)
 * ============================================================ */

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
    if (!arv || !arv->raiz) {
        printf("(arvore vazia)\n");
        return;
    }
    imprimir_rec(arv->raiz, 0);
}

/* ============================================================
 *  Fase 2+ ficará aqui embaixo (rotações, splay, inserir, ...)
 * ============================================================ */