#include <stdio.h>
#include <stdlib.h>

typedef struct elementos {
    char nome[100];
    int rgm;
} t_elemento;

typedef struct no {
    struct no * esq;
    t_elemento dado;
    struct no * dir;
} t_no;

typedef t_no* t_arvore;

t_no * criar () {

    t_no * no = (t_no*) malloc(sizeof(t_no));
    if (no)
    no->esq = no->dir = NULL;
    return no;
}

int compara(t_elemento a, t_elemento b) {
    if (a.rgm < b.rgm)
        return -1;
    
    if (a.rgm > b.rgm)
        return 1;
    
    return 0;
}

int inserir (t_arvore *tree, t_elemento item) {

    int ok;
        if (*tree == NULL) {
            *tree = criar();
            
        if (*tree == NULL)
        return 0;
            
        (*tree)->dado = item;
        return 1;
    }
    
    if (compara((*tree)->dado, item)<0)
    
    ok = inserir (&((*tree)->dir), item);
    
    else
    
    if (compara((*tree)->dado, item)>0)
    
    ok = inserir (&((*tree)->esq), item);
    
    else
    
    ok = 0;
    
    return ok;
}

// Esvaziar a árvore -- Igor Marinho
void esvaziar (t_arvore *tree) {
    if (*tree) {
        esvaziar(&((*tree)->esq));
        esvaziar(&((*tree)->dir));
        free(*tree);
        *tree = NULL;
    }
}

t_no* sucessor(t_no *no) {
    while (no != NULL && no->esq != NULL)
        no = no->esq;

    return no;
}

int remover(t_arvore *tree, int rgm) {

    if (*tree == NULL)
        return 0; // não encontrou

    if (rgm < (*tree)->dado.rgm)
        return remover(&((*tree)->esq), rgm);

    if (rgm > (*tree)->dado.rgm)
        return remover(&((*tree)->dir), rgm);

        printf("Removendo:\n");
        printf("RGM: %d\n", (*tree)->dado.rgm);
        printf("Nome: %s\n", (*tree)->dado.nome);

    if ((*tree)->esq == NULL && (*tree)->dir == NULL) {
        free(*tree);
        *tree = NULL;
    }

    else if ((*tree)->esq == NULL) {
        t_no *aux = *tree;
        *tree = (*tree)->dir;
        free(aux);
    }

    else if ((*tree)->dir == NULL) {
        t_no *aux = *tree;
        *tree = (*tree)->esq;
        free(aux);
    }

    else {
        t_no *aux = sucessor((*tree)->dir);

        (*tree)->dado = aux->dado;

        remover(&((*tree)->dir), aux->dado.rgm);
    }

    return 1;
}

t_no* buscar(t_arvore tree, int rgm) {
    if (tree == NULL)
        return NULL;

    if (rgm < tree->dado.rgm)
        return buscar(tree->esq, rgm);

    if (rgm > tree->dado.rgm)
        return buscar(tree->dir, rgm);

    return tree;
}