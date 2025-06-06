#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Definições das estruturas
typedef struct Node {
    int pr;
    struct Node* prox;
} Node;

typedef struct {
    Node* inicio;
} ListaAdj;

typedef struct {
    int vertices;
    ListaAdj* lista;
} Grafo;

// Função para criar um novo nó
Node* criar_no(int pr) {
    Node* novo = malloc(sizeof(Node));
    novo->pr = pr;
    novo->prox = NULL;
    return novo;
}

// Função para criar um grafo
Grafo* criar_grafo(int vertices) {
    Grafo* grafo = malloc(sizeof(Grafo));
    grafo->vertices = vertices;
    grafo->lista = malloc(vertices * sizeof(ListaAdj));

    for (int i = 0; i < vertices; i++) {
        grafo->lista[i].inicio = NULL;
    }
    return grafo;
}

// Função para adicionar uma aresta ao grafo
void adicionar_aresta(Grafo* grafo, int origem, int destino) {
    Node* novo = criar_no(destino);
    novo->prox = grafo->lista[origem].inicio;
    grafo->lista[origem].inicio = novo;

    novo = criar_no(origem);
    novo->prox = grafo->lista[destino].inicio;
    grafo->lista[destino].inicio = novo;
}

// Função DFS para encontrar o menor caminho
void dfs(Grafo* grafo, int atual, int destino, int* visitado, int profundidade, int* menorCaminho) {
    if (atual == destino) {
        if (profundidade < *menorCaminho) {
            *menorCaminho = profundidade;
        }
        return;
    }

    visitado[atual] = 1;

    Node* temp = grafo->lista[atual].inicio;
    while (temp != NULL) {
        if (!visitado[temp->pr]) {
            dfs(grafo, temp->pr, destino, visitado, profundidade + 1, menorCaminho);
        }
        temp = temp->prox;
    }

    visitado[atual] = 0; // Backtracking
}

// Função para encontrar o índice de um ponto dado o nome
int encontrar_indice(char pontos[][50], int n, char* nome) {
    for (int i = 0; i < n; i++) {
        if (strcmp(pontos[i], nome) == 0) return i;
    }
    return -1;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    Grafo* grafo = criar_grafo(n);

    char pontos[n][50];
    int entrada = -1, saida = -1, queijo = -1;
    int total_pontos = 0;

    // Lendo as arestas
    for (int i = 0; i < m; i++) {
        char origem[50], destino[50];
        scanf("%s %s", origem, destino);

        // Verificar e adicionar novos pontos ao array
        int idx_origem = encontrar_indice(pontos, total_pontos, origem);
        if (idx_origem == -1) {
            strcpy(pontos[total_pontos], origem);
            idx_origem = total_pontos;
            total_pontos++;
        }

        int idx_destino = encontrar_indice(pontos, total_pontos, destino);
        if (idx_destino == -1) {
            strcpy(pontos[total_pontos], destino);
            idx_destino = total_pontos;
            total_pontos++;
        }

        if (strcmp(origem, "Entrada") == 0) entrada = idx_origem;
        if (strcmp(destino, "Entrada") == 0) entrada = idx_destino;
        if (strcmp(origem, "Saida") == 0) saida = idx_origem;
        if (strcmp(destino, "Saida") == 0) saida = idx_destino;
        if (strcmp(origem, "*") == 0) queijo = idx_origem;
        if (strcmp(destino, "*") == 0) queijo = idx_destino;

        adicionar_aresta(grafo, idx_origem, idx_destino);
    }

    // Verifica se os pontos de interesse foram encontrados
    if (entrada == -1 || saida == -1 || queijo == -1) {
        printf("-1\n"); // Erro: Não foram encontrados todos os pontos de interesse
        return 1;
    }

    // Encontrando o caminho mínimo usando DFS
    int* visitado = calloc(n, sizeof(int));
    int menorCaminhoEntradaQueijo = INT_MAX;
    int menorCaminhoQueijoSaida = INT_MAX;

    // Busca do ponto de entrada até o queijo
    dfs(grafo, entrada, queijo, visitado, 0, &menorCaminhoEntradaQueijo);

    // Busca do queijo até a saída
    memset(visitado, 0, n * sizeof(int)); // Resetar vetor de visitados para fazer dfs do queijo ate a saida
    dfs(grafo, queijo, saida, visitado, 0, &menorCaminhoQueijoSaida);

    if (menorCaminhoEntradaQueijo != INT_MAX && menorCaminhoQueijoSaida != INT_MAX) {
        printf("%d\n", menorCaminhoEntradaQueijo + menorCaminhoQueijoSaida);
    } else {
        printf("-1\n"); // Impossível alcançar o queijo ou a saída
    }

    free(visitado);
    free(grafo->lista);
    free(grafo);

    return 0;
}

