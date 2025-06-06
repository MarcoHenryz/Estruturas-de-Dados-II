#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int tipochave;

typedef struct aux{

    int id;
    int bf;
    struct aux* esq;
    struct aux* dir;
    struct aux* pai;
    int altura;
}NO;

typedef NO* PONTTREE;

typedef struct aux2{

    struct aux* prox;
    PONTTREE no;
}NODE;

typedef NODE* PONTPILHA;

typedef struct{

    PONTPILHA topo;
}PILHA;

void inicializar_pilha(PILHA * p){
    p->topo = NULL;
}

PONTPILHA criar_no_pilha(PONTTREE no){
    
    PONTPILHA ele = malloc(sizeof(NODE));
    ele->prox = NULL;
    ele->no = no;
    return ele;
}

bool push(PILHA * p, PONTTREE no){

    PONTPILHA ele = criar_no_pilha(no);
    if(p->topo == NULL){
        p->topo = ele;
    }else{
        ele->prox = p->topo;
        p->topo = ele;
    }
    return true;
}

PONTTREE pull(PILHA * p){
    
    if(p->topo == NULL){
        return NULL;
    }else{
        PONTPILHA apagar = p->topo;
        p->topo = p->topo->prox;
        PONTTREE aux = apagar->no;
        free(apagar);
        return aux;
    }
}

void exibir_pilha(PILHA * p){

    PONTPILHA aux = p->topo;
    while(aux != NULL){
        printf("%d", aux->no->id);
        aux = aux->prox;
    }
}

void inicializar_raiz(PONTTREE * raiz){
    *raiz = NULL;
}

PONTTREE criar_no(tipochave chave){

    PONTTREE no = malloc(sizeof(NO));
    if(no == NULL){
        printf("Erro");
        exit(1);
    }

    no->altura = 0;
    no->bf = 0;
    no->esq = NULL;
    no->dir = NULL;
    no->id = chave;
    no->pai = NULL;
    
    return no;
}

int altura();

int calculo_bf(PONTTREE no){
    if(no != NULL){
        no->bf = altura(no->dir) - altura(no->esq);
    }
}

PONTTREE rotacao_L(PONTTREE p) {
    PONTTREE u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    fatorbalanceamento(p);
    fatorbalanceamento(u);
    return u;
}

PONTTREE rotacao_R(PONTTREE p) {
    PONTTREE u = p->dir;
    p->dir = u->esq;
    u->esq = p;
    fatorbalanceamento(p);
    fatorbalanceamento(u);
    return u;
}

PONTTREE rotacaoRL(PONTTREE p) {
    p->dir = rotacao_L(p->dir);
    return rotacao_R(p);
}

PONTTREE rotacaoLR(PONTTREE p) {
    p->esq = rotacao_R(p->esq);
    return rotacao_L(p);
}

PONTTREE balancear(PONTTREE raiz){
    fatorbalanceamento(raiz);
    if (raiz->bf == -2) {
        if (raiz->esq->bf <= 0) {
            raiz = rotacao_L(raiz); 
        } else {
            raiz = rotacaoLR(raiz); 
        }
    } else if (raiz->bf == 2) {
        if (raiz->dir->bf >= 0) {
            raiz = rotacao_R(raiz); 
        } else {
            raiz = rotacaoRL(raiz); 
        }
    }
    return raiz;
}

void exibir_em_ordem();

void exibir_pos_ordem();

void exibir_pre_ordem();

PONTTREE remover_da_arvore();

PONTTREE inserir(PONTTREE raiz, tipochave key){

if (raiz == NULL){
    raiz = criar_no(key);
    return true;
}else{
    PONTTREE atual = raiz;
    while (atual){
        if (atual->id > key){
            if (atual->esq == NULL) {
            atual->esq = criar_no(key);
            return true;
            }else{
                atual = atual->esq;
            }
        }else{
            if (atual->dir == NULL) {
            atual->dir = criar_no(key);
             return true;
            }else{
            atual = atual->dir;
            }
        }
    }
}
    balancear(raiz);
    return raiz;
}

void inserir_arvore(PONTTREE * raiz, tipochave chave){
    *raiz = inserir(*raiz, chave);
}

void destruir_arvore();

int main() {
    PONTTREE raiz = NULL;
    int escolha, id;

    do {
        printf("\n1 - Inserir na arvore\n");
        printf("2 - Excluir da arvore\n");
        printf("3 - Mostrar arvore em-ordem\n");
        printf("4 - Mostrar em pos-ordem\n");
        printf("5 - Mostrar em pre-ordem\n");
        printf("6 - Destruir arvore\n");
        printf("7 - Buscar\n");
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
                raiz = remover_da_arvore(raiz, id); // Atualizar a raiz após a remoção
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