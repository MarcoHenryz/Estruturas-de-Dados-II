#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef int codigo;
typedef int bool;
#define true 1
#define false 0

typedef struct{
    codigo IBSN;
    char titulo[50];
    char autor[50];
    int anodepub;
    bool ocupado;
} book;

typedef book* Pontbook;

int iniciar_hash(Pontbook* catalogo, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        catalogo[i] = NULL;
    }
    return 1;
}

int* transformar_int_string(codigo ibsn, int* tamanho) {
    *tamanho = 0;
    codigo ibsnaux = ibsn;

    while (ibsnaux > 0) {
        ibsnaux = ibsnaux / 10;
        (*tamanho)++;
    }

    int* numero = malloc((*tamanho) * sizeof(int));
    if (numero == NULL) {
        printf("Erro na alocacao de memoria\n");
        return NULL;
    }

    ibsnaux = ibsn;
    for (int i = *tamanho - 1; i >= 0; i--) {
        numero[i] = ibsnaux % 10;
        ibsnaux /= 10;
    }

    return numero;
}

int hashing(codigo ibsn, int tamanho) {
    int nmrodig;
    int* numero = transformar_int_string(ibsn, &nmrodig);
    int qntdobra = (nmrodig / 2) - 1;
    int j = 1;
    int i = 0;
    int result = 0;

    while (qntdobra != 0) {
        numero[j + 1] = numero[j] + numero[j + 1];
        if (numero[j + 1] >= 10) {
            numero[j + 1] %= 10;
        }
        numero[i + 3] = numero[i] + numero[i + 3];
        if (numero[i + 3] >= 10) {
            numero[i + 3] %= 10;
        }
        i = i + 2;
        j = j + 2;
        qntdobra--;
    }

    if (nmrodig % 2 == 0) {
        for (i = 0; i < nmrodig - 2; i++) {
            numero[i] = 0;
        }
    } else {
        for (i = 0; i < nmrodig - 3; i++) {
            numero[i] = 0;
        }
    }

    for (i = 0; i < nmrodig; i++) {
        result = result * 10 + numero[i];
    }

    free(numero);
    return result % tamanho;
}

int hashingcolisao(int posicao_inicial, int tentativa, int tamanho) {
    return (posicao_inicial + tentativa) % tamanho;
}

int inserir_elemento(book novo, Pontbook* catalogo, int tamanho) {
    int tentativa = 0;
    int posicao = hashing(novo.IBSN, tamanho);

    while (catalogo[posicao] != NULL && catalogo[posicao]->ocupado) {
        tentativa++;
        posicao = hashingcolisao(posicao, tentativa, tamanho);
    }

    if (catalogo[posicao] == NULL) {
        catalogo[posicao] = malloc(sizeof(book));
    }

    if (catalogo[posicao] != NULL) {
        *(catalogo[posicao]) = novo;
        catalogo[posicao]->ocupado = true;
        return 1;
    }

    return 0; // Falha ao inserir
}

int remover_elemento(codigo ibsn, Pontbook* catalogo, int tamanho) {
    int tentativa = 0;
    int posicao = hashing(ibsn, tamanho); // calcula o local do elemento

    while (catalogo[posicao] != NULL) { // procura a posicao correta
        if (catalogo[posicao]->ocupado && catalogo[posicao]->IBSN == ibsn) {
            catalogo[posicao]->ocupado = false;
            return 1;
        }
        tentativa++;
        posicao = hashingcolisao(posicao, tentativa, tamanho);
    } 

    printf("Livro nao encontrado.\n");
    return 0; // Elemento não encontrado
}

book* buscar_elemento(codigo ibsn, Pontbook* catalogo, int tamanho) {
    int tentativa = 0;
    int posicao = hashing(ibsn, tamanho);

    while (catalogo[posicao] != NULL) {
        if (catalogo[posicao]->ocupado && catalogo[posicao]->IBSN == ibsn) {
            return catalogo[posicao];
        }
        tentativa++;
        posicao = hashingcolisao(posicao, tentativa, tamanho);
    }

    return NULL; // Elemento não encontrado
}

void imprimir_livro(book* b) {
    if (b != NULL) {
        printf("IBSN: %d\n", b->IBSN);
        printf("Titulo: %s\n", b->titulo);
        printf("Autor: %s\n", b->autor);
        printf("Ano de Publicacao: %d\n", b->anodepub);
    } else {
        printf("Livro nao encontrado.\n");
    }
}

void imprimir_livros_na_mesma_posicao(int posicao, Pontbook* catalogo, int tamanho) {
    
    if (posicao < 0 || posicao >= tamanho) {
        printf("Posicao invalida!\n");
    }

    int encontrado = 0; 
    int tentativa = 0;
    int posicao_atual = posicao;

    while (tentativa < tamanho && catalogo[posicao_atual] != NULL) {
        if (catalogo[posicao_atual]->ocupado) {
            imprimir_livro(catalogo[posicao_atual]);
            encontrado = 1;
        }
        tentativa++;
        posicao_atual = hashingcolisao(posicao, tentativa, tamanho);
    }

    if (encontrado == 0) {
        printf("Nenhum livro encontrado na posicao %d.\n", posicao);
    }
}


int main() {
    int tam = 1381;
    Pontbook* tabela = malloc(tam * sizeof(Pontbook));
    iniciar_hash(tabela, tam);

    int posicao;
    int opcao;
    codigo ibsn;
    book novo;
    book* encontrado;
    int i = 0;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Inserir Livro\n");
        printf("2. Remover Livro\n");
        printf("3. Buscar Livro\n");
        printf("4. Imprimir livros na mesma posicao\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Inserir novo livro\n");
                printf("Digite o IBSN: ");
                scanf("%d", &novo.IBSN);
                getchar(); 
                printf("Insira o titulo: \n");
                fgets(novo.titulo, sizeof(novo.titulo), stdin);
                fflush(stdin);
                printf("Insira o autor: \n");
                fgets(novo.autor, sizeof(novo.autor), stdin);
                fflush(stdin);
                printf("Insira o ano de publicacao: \n"); 
                novo.ocupado = true;

                if (inserir_elemento(novo, tabela, tam)) {
                    printf("Livro inserido com sucesso.\n");
                } else {
                    printf("Erro ao inserir o livro.\n");
                }
                break;

            case 2:
                printf("Remover livro\n");
                printf("Digite o IBSN: ");
                scanf("%d", &ibsn);

                remover_elemento(ibsn, tabela, tam);
                break;

            case 3:
                printf("Buscar livro\n");
                printf("Digite o IBSN: ");
                scanf("%d", &ibsn);

                encontrado = buscar_elemento(ibsn, tabela, tam);
                imprimir_livro(encontrado);
                break;

            case 4:

                printf("Imprimir livros na mesma posicao\n");
                printf("Digite a posicao: ");
                scanf("%d", &posicao);

                imprimir_livros_na_mesma_posicao(posicao, tabela, tam);
                break;

            case 0:
                printf("Finalizado\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    for(int i = 0; i < tam; i++) {
        if (tabela[i] != NULL) {
            free(tabela[i]);
        }
    
    free(tabela);
    }

    return 0;
}