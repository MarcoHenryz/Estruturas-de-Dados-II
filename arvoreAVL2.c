#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int tipochave;

typedef struct aux { // struct do  no da arvore
    tipochave id;
    struct aux* esq;
    struct aux* dir;
    int bf;
} NO, *PONT;

PONT criar_no(tipochave id) { // criar nós para arvore
    PONT no = (PONT) malloc(sizeof(NO));
    if (no == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
    no->id = id;
    no->esq = NULL;
    no->dir = NULL;
    no->bf = 0;
    return no;
}

int altura(PONT no) { // calcula altura dos nos recursivamente
    if (no == NULL) {
        return 0;
    }
    int alturaesq = altura(no->esq);
    int alturadir = altura(no->dir);
    if (alturaesq > alturadir) { // soma um no final porque começa um nó abaixo
        return 1 + alturaesq; 
    } else {
        return 1 + alturadir;
    }
}

void fatorbalanceamento(PONT no) { // calculo do bf
    if (no != NULL) {
        no->bf = altura(no->dir) - altura(no->esq);
    }
}

PONT rotacao_L(PONT p) { // função left
    PONT u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    fatorbalanceamento(p);
    fatorbalanceamento(u);
    return u;
}

PONT rotacao_R(PONT p) { // função right
    PONT u = p->dir;
    p->dir = u->esq;
    u->esq = p;
    fatorbalanceamento(p);
    fatorbalanceamento(u);
    return u;
}

PONT rotacaoRL(PONT p) {        // função right left
    p->dir = rotacao_L(p->dir);
    return rotacao_R(p);
}

PONT rotacaoLR(PONT p) {        // função left right
    p->esq = rotacao_R(p->esq);
    return rotacao_L(p);
}

PONT balancear(PONT raiz) { // funcao que verifica se é necessario balancear e executa o balanceamento necessario.
    fatorbalanceamento(raiz); // calcula o bf da raiz
    if (raiz->bf == -2) {
        if (raiz->esq->bf <= 0) {
            raiz = rotacao_L(raiz); //left left
        } else {
            raiz = rotacaoLR(raiz); // left right
        }
    } else if (raiz->bf == 2) {
        if (raiz->dir->bf >= 0) {
            raiz = rotacao_R(raiz); // right righ
        } else {
            raiz = rotacaoRL(raiz); // right left
        }
    }
    return raiz;
}

PONT inserir(PONT raiz, tipochave chave) { // faz um processo de busca ate identificar o local "raiz" (NULO) para realizar a inserção correta do elemento
    if (raiz == NULL) {
        return criar_no(chave); // verificar raiz nula
    }
    if (chave < raiz->id) {
        raiz->esq = inserir(raiz->esq, chave);
    } else if (chave > raiz->id) {                  // ver se a chave é maior ou menor que a chave da raiz
        raiz->dir = inserir(raiz->dir, chave);
    }
    raiz = balancear(raiz); // ver se é necessario ocorrer um balanceamento e retorna a raiz ja balanceada
    return raiz; // retorna a raiz nova da arvore balanceada ou a mesma raiz
}

void inserir_arvore(PONT* raiz, tipochave chave) {
    *raiz = inserir(*raiz, chave);
}

PONT remover(PONT raiz, tipochave chave) {
    if (raiz == NULL) {
        return NULL;
    }
    if (chave < raiz->id) {
        raiz->esq = remover(raiz->esq, chave);
    } else if (chave > raiz->id) {
        raiz->dir = remover(raiz->dir, chave);
    } else {
        if (raiz->esq == NULL) {
            PONT aux = raiz->dir;
            free(raiz);
            return aux;
        } else if (raiz->dir == NULL) {
            PONT aux = raiz->esq;
            free(raiz);
            return aux;
        } else {
            PONT aux = raiz->esq;
            while (aux->dir != NULL) {
                aux = aux->dir;
            }
            raiz->id = aux->id;
            aux->id = chave;
            raiz->esq = remover(raiz->esq, chave);
        }
    }
    return balancear(raiz); // Balanceia a árvore após a remoção
}

void exibir_em_ordem(PONT raiz) {
    if (raiz != NULL) {
        exibir_em_ordem(raiz->esq);
        printf("%d ", raiz->id);
        exibir_em_ordem(raiz->dir);
    }
}

void exibir_pos_ordem(PONT raiz) {
    if (raiz != NULL) {
        exibir_pos_ordem(raiz->esq);
        exibir_pos_ordem(raiz->dir);
        printf("%d ", raiz->id);
    }
}

void exibir_pre_ordem(PONT raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->id);
        exibir_pre_ordem(raiz->esq);
        exibir_pre_ordem(raiz->dir);
    }
}

void destruir_arvore(PONT raiz) {
    if (raiz != NULL) {
        destruir_arvore(raiz->esq);
        destruir_arvore(raiz->dir);
        free(raiz);
    }
}

void imprimir_tomas (PONT raiz, int nivel){
    int i;
    if(raiz){
        imprimir_tomas(raiz->dir, nivel + 1);
        printf("\n\n");
        for (i = 0; i < nivel; i++)
            printf("\t");
        printf("%d", raiz->id);
        imprimir_tomas(raiz->esq, nivel + 1);
    }
}

PONT buscar(PONT raiz, tipochave key){
    if(raiz == NULL){
        printf("Elemento nao encontrado!\n");
        return NULL;
    }else if(raiz->id == key){
        printf("Elemento encontrado!\n");
        return raiz;
    }else if(key < raiz-> id){
        return buscar(raiz->esq, key);
    }else{
        return buscar(raiz->dir, key);
    }
}

int main() {
    PONT raiz = NULL;
    int escolha, id;

    do {
        printf("\n1 - Inserir na arvore\n");
        printf("2 - Excluir da arvore\n");
        printf("3 - Mostrar arvore em-ordem\n");
        printf("4 - Mostrar em pos-ordem\n");
        printf("5 - Mostrar em pre-ordem\n");
        printf("6 - Destruir arvore\n");
        printf("7 - Imprimir\n");
        printf("8 - Buscar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &id);
                inserir_arvore(&raiz, id);
                break;
            case 2:
                printf("Digite o valor a ser excluido: ");
                scanf("%d", &id);
                raiz = remover(raiz, id); // Atualizar a raiz após a remoção
                break;
            case 3:
                exibir_em_ordem(raiz);
                printf("\n");
                break;
            case 4:
                exibir_pos_ordem(raiz);
                printf("\n");
                break;
            case 5:
                exibir_pre_ordem(raiz);
                printf("\n");
                break;
            case 6:
                destruir_arvore(raiz);
                raiz = NULL;
                break;
            case 7:
                imprimir_tomas(raiz, 1);
                break;
            case 8:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &id);
                buscar(raiz, id);
                break;
            case 0:
                exit(0);
            default:
                printf("Escolha invalida\n");
        }
    } while (escolha != 0);

    return 0;
}
