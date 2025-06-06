#include<stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int codigo;
typedef int bool;

#define true 1
#define false 0

typedef struct aux {
    codigo IBSN;
    char titulo[50];
    char autor[50];
    int anodepub;
    struct aux* prox;
} book;

typedef book* Pontbook;

typedef struct {
    Pontbook* lista;
    int tamanho;
} TabelaHash;

int iniciar_hash(TabelaHash* tabela, int tamanho) {
    tabela->tamanho = tamanho;
    tabela->lista = malloc(tamanho * sizeof(Pontbook));
    if (tabela->lista == NULL) {
        return 0;
    }

    for (int i = 0; i < tamanho; i++) {
        tabela->lista[i] = NULL;
    }
    return 1;
}

int calcular_hash(char* titulo, int tamanho) {
    int hash = 0;
    for (int i = 0; titulo[i] != '\0'; i++) {
        hash += titulo[i];
    }
    return hash % tamanho;
}

bool inserir_elemento(TabelaHash* tabela, book novo) {
    int indice = calcular_hash(novo.titulo, tabela->tamanho);
    
    
    Pontbook novoLivro = (Pontbook)malloc(sizeof(book));
    if (novoLivro == NULL) {
        return false; // Falha ao alocar memória
    }

    *novoLivro = novo;
    novoLivro->prox = tabela->lista[indice];
    tabela->lista[indice] = novoLivro;
    return true;
}

Pontbook buscar_elemento(TabelaHash* tabela, char* titulo) {
    int indice = calcular_hash(titulo, tabela->tamanho);
    
    Pontbook atual = tabela->lista[indice];

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL; 
}

bool remover_elemento(TabelaHash* tabela, char* titulo) {
    int indice = calcular_hash(titulo, tabela->tamanho);
    
    Pontbook atual = tabela->lista[indice];
    Pontbook anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            if (anterior == NULL) {
                tabela->lista[indice] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return false; // Elemento não encontrado
}

void imprimir_livro(Pontbook b) {
    if (b != NULL) {
        printf("IBSN: %d\n", b->IBSN);
        printf("Titulo: %s\n", b->titulo);
        printf("Autor: %s\n", b->autor);
        printf("Ano de Publicacao: %d\n", b->anodepub);
    }
}

void imprimir_livros_na_posicao(TabelaHash* tabela, int posicao) {
    if (posicao < 0 || posicao >= tabela->tamanho) {
        printf("Posicao invalida!\n");
        return;
    }

    Pontbook atual = tabela->lista[posicao];
    if (atual == NULL) {
        printf("Nenhum livro encontrado na posicao %d.\n", posicao);
        return;
    }

    printf("Livros na posicao %d:\n", posicao);
    while (atual != NULL) {
        imprimir_livro(atual);
        atual = atual->prox;
        printf("\n");
    }
}

int main() {
    int tam = 10; // Tamanho da tabela hash
    TabelaHash tabela;

    if (!iniciar_hash(&tabela, tam)) {
        printf("Erro ao iniciar a tabela hash.\n");
        return 1;
    }

    int opcao;
    char titulo[50];
    book novo;
    Pontbook encontrado;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Inserir Livro\n");
        printf("2. Remover Livro\n");
        printf("3. Buscar Livro\n");
        printf("4. Imprimir Livros na Mesma Posicao\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Inserir novo livro\n");
                printf("Digite o IBSN: ");
                scanf("%d", &novo.IBSN);
                fflush(stdin);
                printf("Insira o titulo:");
                fgets(novo.titulo, sizeof(novo.titulo), stdin);
                if (novo.titulo[strlen(novo.titulo) - 1] == '\n') {
                    novo.titulo[strlen(novo.titulo) - 1] = '\0';
                }
                fflush(stdin);
                printf("Insira o autor: \n");
                fgets(novo.autor, sizeof(novo.autor), stdin);
                if (novo.autor[strlen(novo.autor) - 1] == '\n') {
                    novo.autor[strlen(novo.autor) - 1] = '\0';
                }
                printf("Insira o ano de publicacao: \n");
                scanf("%d", &novo.anodepub);
                fflush(stdin);

                if (inserir_elemento(&tabela, novo)) {
                    printf("Livro inserido com sucesso.\n");
                } else {
                    printf("Erro ao inserir o livro.\n");
                }
                break;

            case 2:
                printf("Remover livro\n");
                getchar(); 
                printf("Insira o titulo:");
                fgets(titulo, sizeof(titulo), stdin);
                if (titulo[strlen(titulo) - 1] == '\n') {
                    titulo[strlen(titulo) - 1] = '\0';
                }

                if (remover_elemento(&tabela, titulo)) {
                    printf("Livro removido com sucesso.\n");
                } else {
                    printf("Erro ao remover o livro ou livro nao encontrado.\n");
                }
                break;

            case 3:
                printf("Buscar livro\n");
                getchar(); 
                printf("Insira o titulo: ");
                fgets(titulo, sizeof(titulo), stdin);
                if (titulo[strlen(titulo) - 1] == '\n') {
                    titulo[strlen(titulo) - 1] = '\0';
                }

                encontrado = buscar_elemento(&tabela, titulo);
                if (encontrado != NULL) {
                    imprimir_livro(encontrado);
                } else {
                    printf("Livro nao encontrado.\n");
                }
                break;

            case 4:
                printf("Imprimir livros na mesma posicao\n");
                printf("Digite a posicao desejada: ");
                int posicao;
                scanf("%d", &posicao);

                imprimir_livros_na_posicao(&tabela, posicao);
                break;

            case 0:
                printf("Saindo\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

        for(int i = 0; i < tam;) {
        Pontbook atual = tabela.lista[i];
        while (atual != NULL) {
            Pontbook prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(tabela.lista);

    return 0;
}
