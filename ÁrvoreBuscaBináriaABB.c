#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_TXT "arvore.txt"

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

/* ========================= FUNCOES BASICAS DA ARVORE ========================= */

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

/* ========================= EXIBICAO NO CONSOLE ========================= */

void MostrarEmOrdem(t_arvore tree) {
    if (tree != NULL) {
        MostrarEmOrdem(tree->esq);
        printf("RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
        MostrarEmOrdem(tree->dir);
    }
}

void MostrarPreOrdem(t_arvore tree) {
    if (tree != NULL) {
        printf("RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
        MostrarPreOrdem(tree->esq);
        MostrarPreOrdem(tree->dir);
    }
}

void MostrarPosOrdem(t_arvore tree) {
    if (tree != NULL) {
        MostrarPosOrdem(tree->esq);
        MostrarPosOrdem(tree->dir);
        printf("RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
    }
}

/* ========================= GERACAO DO ARQUIVO TXT ========================= */

void escreverArvoreVisual(FILE *f, t_arvore tree, int nivel) {
    int i;
    if (tree == NULL) return;

    escreverArvoreVisual(f, tree->dir, nivel + 1);

    for (i = 0; i < nivel; i++)
        fprintf(f, "    ");

    fprintf(f, "[RGM:%d | %s]\n", tree->dado.rgm, tree->dado.nome);

    escreverArvoreVisual(f, tree->esq, nivel + 1);
}

void gravarPreOrdem(FILE *f, t_arvore tree) {
    if (tree == NULL) return;
    fprintf(f, "  RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
    gravarPreOrdem(f, tree->esq);
    gravarPreOrdem(f, tree->dir);
}

void gravarEmOrdem(FILE *f, t_arvore tree) {
    if (tree == NULL) return;
    gravarEmOrdem(f, tree->esq);
    fprintf(f, "  RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
    gravarEmOrdem(f, tree->dir);
}

void gravarPosOrdem(FILE *f, t_arvore tree) {
    if (tree == NULL) return;
    gravarPosOrdem(f, tree->esq);
    gravarPosOrdem(f, tree->dir);
    fprintf(f, "  RGM: %d | Nome: %s\n", tree->dado.rgm, tree->dado.nome);
}

void salvarArvoreNoArquivo(t_arvore tree) {
    FILE *f = fopen(ARQUIVO_TXT, "w");
    if (f == NULL) {
        printf("AVISO: nao foi possivel abrir/criar o arquivo '%s'.\n", ARQUIVO_TXT);
        return;
    }

    fprintf(f, "========================================\n");
    fprintf(f, "         ARVORE BINARIA DE BUSCA        \n");
    fprintf(f, "========================================\n\n");

    if (tree == NULL) {
        fprintf(f, "  (arvore vazia)\n");
    } else {
        fprintf(f, "--- ESTRUTURA VISUAL (raiz = menor recuo) ---\n");
        fprintf(f, "    (subarvore DIR aparece ACIMA da raiz,\n");
        fprintf(f, "     subarvore ESQ aparece ABAIXO)\n\n");
        escreverArvoreVisual(f, tree, 0);

        fprintf(f, "\n--- PRE-ORDEM (Raiz -> Esq -> Dir) ---\n");
        gravarPreOrdem(f, tree);

        fprintf(f, "\n--- EM ORDEM / IN-ORDEM (Esq -> Raiz -> Dir) ---\n");
        gravarEmOrdem(f, tree);

        fprintf(f, "\n--- POS-ORDEM (Esq -> Dir -> Raiz) ---\n");
        gravarPosOrdem(f, tree);
    }

    fprintf(f, "\n========================================\n");
    fclose(f);
    printf("Arquivo '%s' atualizado com sucesso.\n", ARQUIVO_TXT);
}

/* ========================= LEITURA DO ARQUIVO TXT ========================= */

/*
 * Le a secao "EM ORDEM / IN-ORDEM" do arquivo TXT e reconstroi a arvore
 * em memoria. Cada linha de dado tem o formato:
 *   "  RGM: <valor> | Nome: <nome>"
 * A leitura para ao encontrar outra secao ("---") ou o fim do arquivo.
 * Inserir a partir da sequencia em ordem reconstroi uma arvore valida,
 * mas ela ficara degenerada (lista encadeada) pois os valores chegam
 * ordenados. Para preservar a forma original, a funcao usa a secao
 * PRE-ORDEM, que garante a mesma estrutura da arvore salva.
 */
void carregarDoArquivo(t_arvore *tree) {
    FILE *f;
    char linha[200];
    t_elemento elem;
    int dentro_secao;

    f = fopen(ARQUIVO_TXT, "r");
    if (f == NULL) return; /* arquivo ainda nao existe — tudo bem */

    dentro_secao = 0;

    while (fgets(linha, sizeof(linha), f)) {
        /* Detecta o inicio da secao pre-ordem */
        if (strstr(linha, "PRE-ORDEM") != NULL && dentro_secao == 0) {
            dentro_secao = 1;
            continue;
        }

        /* Detecta inicio de outra secao: encerra leitura */
        if (dentro_secao && strstr(linha, "---") != NULL)
            break;

        if (dentro_secao) {
            /* Linha de dado: "  RGM: 42 | Nome: Fulano de Tal" */
            if (sscanf(linha, "  RGM: %d | Nome: %99[^\n]",
                       &elem.rgm, elem.nome) == 2) {
                inserir(tree, elem);
            }
        }
    }

    fclose(f);
}

/* ========================= MAIN ========================= */

int main() {
    t_arvore arvore = NULL;
    int opcao, subOpcao;
    t_elemento novo;
    int rgm;
    t_no *resultado;
    char confirma;

    /* Tenta recuperar dados salvos de uma sessao anterior */
    carregarDoArquivo(&arvore);
    if (arvore != NULL)
        printf("Dados carregados do arquivo '%s'.\n", ARQUIVO_TXT);

    do {
        printf("\n             EDITOR DE ARVORE\n");
        printf("\n 1 - INSERIR\n 2 - REMOVER UM NO\n 3 - PESQUISAR\n"
               " 4 - ESVAZIAR ARVORE\n 5 - EXIBIR A ARVORE\n 0 - SAIR\n");
        printf("-----------------------------\n");
        printf("       DIGITE SUA OPCAO: ");
        scanf("%d", &opcao);

        switch (opcao) {

            /* ---------- INSERIR ---------- */
            case 1:
                printf("\n--- INSERIR ---\n");
                printf("Digite o RGM: ");
                scanf("%d", &novo.rgm);
                getchar();
                printf("Digite o nome: ");
                fgets(novo.nome, sizeof(novo.nome), stdin);
                novo.nome[strcspn(novo.nome, "\n")] = '\0';

                if (inserir(&arvore, novo)) {
                    printf("Aluno inserido com sucesso!\n");
                    salvarArvoreNoArquivo(arvore);
                } else {
                    printf("Falha ao inserir (RGM ja existe ou memoria insuficiente).\n");
                }
                break;

            /* ---------- REMOVER ---------- */
            case 2:
                printf("\n--- REMOVER ---\n");
                printf("Digite o RGM a remover: ");
                scanf("%d", &rgm);
                if (!remover(&arvore, rgm)) {
                    printf("RGM %d nao encontrado na arvore.\n", rgm);
                } else {
                    printf("No removido com sucesso!\n");
                    salvarArvoreNoArquivo(arvore);
                }
                break;

            /* ---------- PESQUISAR ---------- */
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

            /* ---------- ESVAZIAR ---------- */
            case 4:
                printf("\n--- ESVAZIAR ARVORE ---\n");
                printf("Tem certeza? Todos os dados serao perdidos. (s/n): ");
                scanf(" %c", &confirma);
                if (confirma == 's' || confirma == 'S') {
                    esvaziar(&arvore);
                    salvarArvoreNoArquivo(arvore);
                    printf("Arvore esvaziada com sucesso!\n");
                } else {
                    printf("Operacao cancelada.\n");
                }
                break;

            /* ---------- EXIBIR ---------- */
            case 5:
                printf("\n--- EXIBIR ARVORE ---\n");

                /* Se a arvore em memoria esta vazia, tenta ler do arquivo */
                if (arvore == NULL) {
                    carregarDoArquivo(&arvore);
                    if (arvore != NULL)
                        printf("(Dados recarregados do arquivo '%s')\n", ARQUIVO_TXT);
                }

                if (arvore == NULL) {
                    printf("A arvore esta vazia e nao ha dados salvos no arquivo.\n");
                    break;
                }

                printf("\n  1 - PRE-ORDEM  (Raiz -> Esq -> Dir)\n");
                printf("  2 - IN-ORDEM   (Esq  -> Raiz -> Dir)\n");
                printf("  3 - POS-ORDEM  (Esq  -> Dir  -> Raiz)\n");
                printf("  DIGITE A OPCAO DE EXIBICAO: ");
                scanf("%d", &subOpcao);

                switch (subOpcao) {
                    case 1:
                        printf("\n[PRE-ORDEM]\n");
                        MostrarPreOrdem(arvore);
                        break;
                    case 2:
                        printf("\n[IN-ORDEM]\n");
                        MostrarEmOrdem(arvore);
                        break;
                    case 3:
                        printf("\n[POS-ORDEM]\n");
                        MostrarPosOrdem(arvore);
                        break;
                    default:
                        printf("Opcao invalida.\n");
                }
                break;

            /* ---------- SAIR ---------- */
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
