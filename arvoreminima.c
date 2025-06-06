#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int u, v;
    double peso;
} Aresta;


int compararArestas(Aresta *a, Aresta *b) {
    if (a->peso < b->peso) return -1;
    if (a->peso > b->peso) return 1;
    return 0;
}


void ordenarArestas(Aresta arestas[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (compararArestas(&arestas[j], &arestas[j + 1]) > 0) {
                Aresta temp = arestas[j];
                arestas[j] = arestas[j + 1];
                arestas[j + 1] = temp;
            }
        }
    }
}


int encontrar(int pai[], int i) {
    if (pai[i] == i) return i;
    return pai[i] = encontrar(pai, pai[i]);
}


void unirConjuntos(int pai[], int rank[], int x, int y) {
    int raizX = encontrar(pai, x);
    int raizY = encontrar(pai, y);
    
    if (raizX != raizY) {
        if (rank[raizX] < rank[raizY]) {
            pai[raizX] = raizY;
        } else if (rank[raizX] > rank[raizY]) {
            pai[raizY] = raizX;
        } else {
            pai[raizY] = raizX;
            rank[raizX]++;
        }
    }
}


double calcular_peso_duas_arestas(int x, int y, int a, int b) {
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

int main() {
    int casosDeTeste;
    scanf("%d", &casosDeTeste);

    while (casosDeTeste--) {
        int numeroDePessoas;
        scanf("%d", &numeroDePessoas);
        
        int vectorx[500], vectory[500];
        Aresta arestas[124750]; 
        int contadorArestas = 0;

        
        for (int i = 0; i < numeroDePessoas; i++) {
            scanf("%d %d", &vectorx[i], &vectory[i]);
        }

        
        for (int i = 0; i < numeroDePessoas; i++) {
            for (int j = i + 1; j < numeroDePessoas; j++) {
                arestas[contadorArestas].u = i;
                arestas[contadorArestas].v = j;
                arestas[contadorArestas].peso = calcular_peso_duas_arestas(vectorx[i], vectory[i], vectorx[j], vectory[j]);
                contadorArestas++;
            }
        }

        
        ordenarArestas(arestas, contadorArestas);

        
        int pai[500], rank[500];
        for (int i = 0; i < numeroDePessoas; i++) {
            pai[i] = i;
            rank[i] = 0;
        }

        double pesoMST = 0.0;
        int arestasNaMST = 0;


        for (int i = 0; i < contadorArestas && arestasNaMST < numeroDePessoas - 1; i++) {
            int u = arestas[i].u;
            int v = arestas[i].v;

            int conjuntoU = encontrar(pai, u);
            int conjuntoV = encontrar(pai, v);

            if (conjuntoU != conjuntoV) {
                pesoMST += arestas[i].peso;
                unirConjuntos(pai, rank, conjuntoU, conjuntoV);
                arestasNaMST++;
            }
        }

        
        printf("%.2lf\n", pesoMST/100);
    }

    return 0;
}
