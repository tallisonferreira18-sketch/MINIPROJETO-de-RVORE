#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct elementos {
    char nome[100];
    int rgm;
} t_elemento;

typedef struct no {
    struct no *esq;
    t_elemento dado;
    struct no *dir;
} t_no;

typedef t_no *t_arvore;

t_no *criar() {
    t_no *no = (t_no *)malloc(sizeof(t_no));
    if (no)
        no->esq = no->dir = NULL;
    return no;
}

int compara(t_elemento a, t_elemento b) {
    if (a.rgm < b.rgm) return -1;
    if (a.rgm > b.rgm) return  1;
    return 0;
}

int inserir(t_arvore *tree, t_elemento item) {
    int ok;
    if (*tree == NULL) {
        *tree = criar();
        if (*tree == NULL) return 0;
        (*tree)->dado = item;
        return 1;
    }
    if (compara((*tree)->dado, item) < 0)
        ok = inserir(&((*tree)->dir), item);
    else if (compara((*tree)->dado, item) > 0)
        ok = inserir(&((*tree)->esq), item);
    else
        ok = 0;
    return ok;
}

void MostrarEmOrdem(t_arvore tree) {
    if (tree != NULL) {
        MostrarEmOrdem(tree->esq);
        printf("RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
        MostrarEmOrdem(tree->dir);
    }
}

void esvaziar(t_arvore *tree) {
    if (*tree) {
        esvaziar(&((*tree)->esq));
        esvaziar(&((*tree)->dir));
        free(*tree);
        *tree = NULL;
    }
}

t_no *sucessor(t_no *no) {
    while (no != NULL && no->esq != NULL)
        no = no->esq;
    return no;
}

int remover(t_arvore *tree, int rgm) {
    if (*tree == NULL) return 0;
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
    } else if ((*tree)->esq == NULL) {
        t_no *aux = *tree;
        *tree = (*tree)->dir;
        free(aux);
    } else if ((*tree)->dir == NULL) {
        t_no *aux = *tree;
        *tree = (*tree)->esq;
        free(aux);
    } else {
        t_no *aux = sucessor((*tree)->dir);
        (*tree)->dado = aux->dado;
        remover(&((*tree)->dir), aux->dado.rgm);
    }
    return 1;
}

t_no *buscar(t_arvore tree, int rgm) {
    if (tree == NULL)          return NULL;
    if (rgm < tree->dado.rgm)  return buscar(tree->esq, rgm);
    if (rgm > tree->dado.rgm)  return buscar(tree->dir, rgm);
    return tree;
}

int main() {
    t_arvore arvore = NULL;
    int opcao;
    t_elemento novo;
    int rgm;
    t_no *resultado;
    char confirma;

    do {
        printf("             EDITOR DE ARVORE\n");
        printf("\n 1 - INSERIR\n 2 - REMOVER UM NO\n 3 - PESQUISARr\n 4 - ESVAZIAR ARVORE\n 5 - EXIBIR A ARVORE\n 0 - SAIR\n");
        printf("-----------------------------\n");
        printf("       DIGITE SUA OPCAO: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- INSERIR ---\n");
                printf("Digite o RGM: ");
                scanf("%d", &novo.rgm);
                getchar();
                printf("Digite o nome: ");
                fgets(novo.nome, sizeof(novo.nome), stdin);
                novo.nome[strcspn(novo.nome, "\n")] = '\0';
                if (inserir(&arvore, novo))
                    printf("Aluno inserido com sucesso!\n");
                else
                    printf("Falha ao inserir (RGM ja existe ou memoria insuficiente).\n");
                break;

            case 2:
                printf("\n--- REMOVER ---\n");
                printf("Digite o RGM a remover: ");
                scanf("%d", &rgm);
                if (!remover(&arvore, rgm))
                    printf("RGM %d nao encontrado na arvore.\n", rgm);
                else
                    printf("No removido com sucesso!\n");
                break;

            case 3:
                printf("\n--- PESQUISAR ---\n");
                printf("Digite o RGM a pesquisar: ");
                scanf("%d", &rgm);
                resultado = buscar(arvore, rgm);
                if (resultado)
                    printf("Encontrado -> RGM: %d | Nome: %s\n",
                           resultado->dado.rgm, resultado->dado.nome);
                else
                    printf("RGM %d nao encontrado na arvore.\n", rgm);
                break;

            case 4:
                printf("\n--- ESVAZIAR ARVORE ---\n");
                printf("Tem certeza? Todos os dados serao perdidos. (s/n): ");
                scanf(" %c", &confirma);
                if (confirma == 's' || confirma == 'S') {
                    esvaziar(&arvore);
                    printf("Arvore esvaziada com sucesso!\n");
                } else {
                    printf("Operacao cancelada.\n");
                }
                break;

            case 5:
                printf("\n--- EXIBIR ARVORE (em ordem) ---\n");
                if (arvore == NULL)
                    printf("A arvore esta vazia.\n");
                else
                    MostrarEmOrdem(arvore);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    esvaziar(&arvore);
    return 0;
}
