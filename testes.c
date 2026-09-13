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
    assert(n != NULL);
    assert(n->chave == 42);
    assert(n->esq == NULL);
    assert(n->dir == NULL);
    assert(n->pai == NULL);
    free(n);
    printf("[OK] criar_no\n");
}

static void teste_liberar_null(void)
{
    splay_liberar(NULL);   /* não deve crashar */
    printf("[OK] liberar(NULL)\n");
}

static void teste_vazia_null(void)
{
    assert(splay_vazia(NULL) == 1);
    assert(splay_tamanho(NULL) == 0);
    printf("[OK] funcoes aceitam NULL\n");
}

/* Monta manualmente uma árvore pequena só para validar o debug/liberação.
   Não usa splay ainda — apenas linka os ponteiros na mão. */
static void teste_estrutura_manual(void)
{
    SplayTree *t = splay_criar();

    SplayNode *a = splay_criar_no(10);
    SplayNode *b = splay_criar_no(5);
    SplayNode *c = splay_criar_no(15);

    a->esq = b;  b->pai = a;
    a->dir = c;  c->pai = a;

    t->raiz    = a;
    t->tamanho = 3;

    assert(splay_tamanho(t) == 3);
    assert(splay_vazia(t) == 0);

    printf("--- estrutura manual (pre-ordem) ---\n");
    splay_debug_imprimir(t);
    printf("------------------------------------\n");

    splay_liberar(t);
    printf("[OK] estrutura manual + liberar\n");
}

int main(void)
{
    printf("=== Testes Fase 1 ===\n");
    teste_criar_vazia();
    teste_criar_no();
    teste_liberar_null();
    teste_vazia_null();
    teste_estrutura_manual();
    printf("\nTodos os testes da Fase 1 passaram.\n");
    return 0;
}