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
