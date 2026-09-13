#include "splay.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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