#include <stdio.h>
#include <stdlib.h>

typedef struct { 
    int urgencia;
    int tempo;
    int id;
} TAREFA;
/*
Paramêtros:posição na heap
Retorno: posição do pai
*/
int pai(int i) {
    return i / 2;
}
/*
Paramêtros: posição na heap
Retorno: filho esquerdo daquela posição
*/
int filho_esq(int i) {
    return 2 * i;
}
/*
Paramêtros: posição na heap
Retorno: filho direito daquela posição
*/
int filho_dir(int i) {
    return 2 * i + 1;
}
/*
Paramêtros: heapmax(urgencia), tamanho da heapmax
Função: printar a heapmax
*/
void printvetorurgencia(TAREFA *vetor, int n) {
    printf("Heap de Urgencia:\n");
    for (int i = 1; i <= n; i++) {
        printf("%d ", vetor[i].urgencia);
    }
    printf("\n");
}
/*
Paramêtros: heapmin(tempo), tamanho da heapmin
Função: printar a heapmin
*/
void printvetortempo(TAREFA *vetor, int n) {
    printf("Heap de Tempo:\n");
    for (int i = 1; i <= n; i++) {
        printf("%d ", vetor[i].tempo);
    }
    printf("\n");
}
/*
Paramêtros: heapmax, posição na heap que vai precisar subir
Função: Acha o pai daquele elemento, e vê se a urgência do elemento(i) é maior que a do pai.
Se for ele efetua a troca do pai pelo filho. Faz isso recursivamente até o elemento chegar na
posição correta.
*/
void subir_heapmax(TAREFA *heapmax, int i) {
    int j = pai(i);
    if (j > 0 && heapmax[i].urgencia > heapmax[j].urgencia) {
        TAREFA temp = heapmax[i];
        heapmax[i] = heapmax[j];
        heapmax[j] = temp;
        subir_heapmax(heapmax, j);
    }
}
/*
Paramêtros: heapmin, posição na heap que vai subir
Função: Acha o pai daquele elemento, e vê se o tempo do elemento é menor que o tempo do pai.
Se for ele efetua a troca do pai pelo filho. Faz isso recursivamente até chegar na posição correta.
*/
void subir_heapmin(TAREFA *heapmin, int i) {
    int j = pai(i);
    if (j > 0 && heapmin[i].tempo < heapmin[j].tempo) {
        TAREFA temp = heapmin[i];
        heapmin[i] = heapmin[j];
        heapmin[j] = temp;
        subir_heapmin(heapmin, j);
    }
}
/*
Paramêtros: heapmax, posição na heap, tamanho da heap
Função: cria tres auxiliares, uma para o filho esq, outra para o filho dir, outra para armazenar o
maior, se o filho esq for maior que o elemento i, armazena a posição do filho esq no maior, no 
segundo if verifica se o filho a direita é maior que o maior novamente, se for ele vira o maior
ao final dessa verificação teremos o maior elemento entre os 3, efetuamos a troca entre o elemento e 
esse filho maior e chamamos recursivamente a função até chegar na posição correta.
*/
void descer_heapmax(TAREFA *heapmax, int i, int n) {
    int e = filho_esq(i);
    int d = filho_dir(i);
    int maior = i;

    if (e <= n && heapmax[e].urgencia > heapmax[maior].urgencia) {
        maior = e;
    }
    if (d <= n && heapmax[d].urgencia > heapmax[maior].urgencia) {
        maior = d;
    }
    if (maior != i) {
        TAREFA temp = heapmax[i];
        heapmax[i] = heapmax[maior];
        heapmax[maior] = temp;
        descer_heapmax(heapmax, maior, n);
    }
}
/*
Paramêtros: heapmin, posição na heap, tamanho da heap
Função: cria tres auxiliares, uma para o filho esq, outra para o filho dir, outra para armazenar o
menor, se o filho esq for menor que o elemento i, armazena a posição do filho esq no menor, no 
segundo if verifica se o filho a direita é menor que o menor novamente, se for ele vira o menor,
ao final dessa verificação teremos o menor elemento entre os 3, efetuamos a troca entre o elemento e 
esse filho menor e chamamos recursivamente a função até chegar na posição correta.
*/
void descer_heapmin(TAREFA *heapmin, int i, int n) {
    int e = filho_esq(i);
    int d = filho_dir(i);
    int menor = i;

    if (e <= n && heapmin[e].tempo < heapmin[menor].tempo) {
        menor = e;
    }
    if (d <= n && heapmin[d].tempo < heapmin[menor].tempo) {
        menor = d;
    }
    if (menor != i) {
        TAREFA temp = heapmin[i];
        heapmin[i] = heapmin[menor];
        heapmin[menor] = temp;
        descer_heapmin(heapmin, menor, n);
    }
}
/*
Parâmetros: heapmax, heapmin, elemento a ser inserido, tamanho da heap
Função: realoca tamanho para o novo elemento nas duas heaps, insere ele na posição e chama a função
subir para organizar as heaps.
Retorna: tamanho da heap + 1
*/
int inserir(TAREFA **heapmax, TAREFA **heapmin, TAREFA novo, int n) {
    *heapmax = (TAREFA *)realloc(*heapmax, sizeof(TAREFA) * (n + 2));
    *heapmin = (TAREFA *)realloc(*heapmin, sizeof(TAREFA) * (n + 2));

    (*heapmax)[n + 1] = novo;
    (*heapmin)[n + 1] = novo;

    subir_heapmax(*heapmax, n + 1);
    subir_heapmin(*heapmin, n + 1);

    return n + 1;
}
/*
Parâmetros: heap(qualquer uma das duas), id que ta sendo buscado, tamanho da heap
retorna: Posição da heap que está o elemento com esse id, 0 caso não achar.
*/
int busca_remover(TAREFA *heap, int id, int n) {
    for (int i = 1; i <= n; i++) {
        if (heap[i].id == id) {
            return i;
        }
    }
    return 0;
}
/*
Parâmetros: heapmax, heapmin, tamanho da heap
Função: verifica se ela esta vazia, após isso vai fazer a remoção da heapmax,salva o id do elemento
a ser excluído no auxiliar indice, faz o primeiro elemento ser igual ao ultimo, e manda descer para
o ultimo elemento ir para a posição correta dele. Para remover da heapmin, a busca auxiliar é usada para
achar o elemento a ser excluido nela, apos achar o processo é pretido igual feito na heapmax.
Por fim realoca o tamanho das heaps.
Retorno: tamanho da heap - 1
*/
int remover_heapmax(TAREFA **heapmax, TAREFA **heapmin, int n) {
    if (n == 0) {
        printf("Heapmax esta vazio.\n");
        return n;
    }

    int indice = (*heapmax)[1].id;

    (*heapmax)[1] = (*heapmax)[n];
    descer_heapmax(*heapmax, 1, n - 1);

    int pos = busca_remover(*heapmin, indice, n);
    (*heapmin)[pos] = (*heapmin)[n];
    descer_heapmin(*heapmin, pos, n - 1);

    *heapmax = (TAREFA *)realloc(*heapmax, sizeof(TAREFA) * n);
    *heapmin = (TAREFA *)realloc(*heapmin, sizeof(TAREFA) * n);

    return n - 1;
}
/*
Parâmetros: heapmax, heapmin, tamanho das heaps
Função: faz a exatamente a mesma coisa da remoção heapmax, porém com as ordens invertidas
primeiro remove na heapmin e depois remove na heapmax
retorno: tamanho das heaps -1
*/
int remover_heapmin(TAREFA **heapmax, TAREFA **heapmin, int n) {
    if (n == 0) {
        printf("Heapmin esta vazio.\n");
        return n;
    }

    int indice = (*heapmin)[1].id;

    (*heapmin)[1] = (*heapmin)[n];
    descer_heapmin(*heapmin, 1, n - 1);

    int pos = busca_remover(*heapmax, indice, n);
    (*heapmax)[pos] = (*heapmax)[n];
    descer_heapmax(*heapmax, pos, n - 1);

    *heapmax = (TAREFA *)realloc(*heapmax, sizeof(TAREFA) * n);
    *heapmin = (TAREFA *)realloc(*heapmin, sizeof(TAREFA) * n);

    return n - 1;
}
/*
Parâmetros: heapmax, heapmin, endereço de um inteiro, tamanho da heap, id a ser procurado
retorno: O index na heapmax, retorna tambem o index na heapmin por referência

*/
int busca(TAREFA **pheapmax, TAREFA **pheapmin, int *indice, int n, int id) {
    int index_alvo = 0;
    *indice = 0;
    for (int i = 1; i <= n; i++) {
        if ((*pheapmax)[i].id == id) {
            index_alvo = i;
        }
        if ((*pheapmin)[i].id == id) {
            *indice = i;
        }
    }
    return index_alvo;
}
/*
Parâmetros: pheapmax, pheapmin, tamanho da heap, id
Função: atualizar a urgência de uma TAREFA, faz a busca por id na heapmax e por referência a posição na heapmin
também, se retornar 0 signific aque não foi encontrada o elemento, coloca a nova urgencia nas heaps, e organiza
novamente somente a heapmax, já que a heapminnão é afetada pela troca da urgência
*/
void alterar_heap(TAREFA **pheapmax, TAREFA **pheapmin, int n, int id) {
    int indiceheapmin;
    int urgencianova;
    int indiceheapmax = busca(pheapmax, pheapmin, &indiceheapmin, n, id);
    if (indiceheapmax == 0) {
        printf("Elemento nao encontrado\n");
        return 0;
    }

    printf("Digite a nova urgencia da tarefa: ");
    scanf("%d", &urgencianova);

    (*pheapmax)[indiceheapmax].urgencia = urgencianova;
    (*pheapmin)[indiceheapmin].urgencia = urgencianova;

    descer_heapmax(*pheapmax, indiceheapmax, n);
    subir_heapmax(*pheapmax, indiceheapmax);

}
/*
Parâmetros: heapmax, heapmin, tamanho da heap
Função: Insere a quantidade de tarefas que o usuário pedir.
*/
int inserir_varias_tarefas(TAREFA ** heapmax, TAREFA** heapmin, int n){

    free(heapmax);
    free(heapmin);
    int quantidade;
    printf("Insira o numero de elementos que deseja inserir: ");
        scanf("%d", &quantidade);
    TAREFA novo;

    for(int i = 0; i < quantidade; i++){
        printf("Insira o ID da tarefa: ");
            scanf("%d", &novo.id);
        printf("Insira a urgencia da tarefa: ");
            scanf("%d", &novo.urgencia);
        printf("Insira o tempo da tarefa: ");
            scanf("%d", &novo.tempo);
        n = inserir(heapmax, heapmin, novo, n);
    }
    return n;
}
/*
Menu para as funções da heap
*/
void menu() {
    printf("1 - Inserir elemento\n");
    printf("2 - Remover elemento mais urgente\n");
    printf("3 - Remover elemento mais rapido\n");
    printf("4 - Printar heapmax\n");
    printf("5 - Printar heapmin\n");
    printf("6 - Atualizar urgencia\n");
    printf("7 - Consulta tarefa mais urgente\n");
    printf("8 - Consulta tarefa de menor tempo\n");
    printf("9 - Inserir varias tarefas\n");
    printf("0 - Sair\n");
    printf("\nEscolha: ");
}

int main() {
    TAREFA novo;
    int id;
    int n = 0;
    int escolha;
    TAREFA *heapmax = NULL;
    TAREFA *heapmin = NULL;

    do {
        menu();
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                printf("Insira o ID da tarefa: ");
                scanf("%d", &novo.id);
                printf("Insira a urgencia da tarefa: ");
                scanf("%d", &novo.urgencia);
                printf("Insira o tempo da tarefa: ");
                scanf("%d", &novo.tempo);
                n = inserir(&heapmax, &heapmin, novo, n);
                break;

            case 2:
                n = remover_heapmax(&heapmax, &heapmin, n);
                break;

            case 3:
                n = remover_heapmin(&heapmax, &heapmin, n);
                break;

            case 4:
                if (heapmax != NULL && n > 0) {
                    printvetorurgencia(heapmax, n);
                } else {
                    printf("Heapmax esta vazio.\n");
                }
                break;

            case 5:
                if (heapmin != NULL && n > 0) {
                    printvetortempo(heapmin, n);
                } else {
                    printf("Heapmin esta vazio.\n");
                }
                break;

            case 6:
                printf("Digite o ID que deseja alterar: ");
                scanf("%d", &id);
                alterar_heap(&heapmax, &heapmin, n, id);
                break;

            case 7:
                if (heapmax != NULL && n > 0) {
                    printf("Tarefa mais urgente: %d\n", heapmax[1].urgencia);
                } else {
                    printf("Heapmax esta vazio.\n");
                }
                break;

            case 8:
                if (heapmin != NULL && n > 0) {
                    printf("Tarefa de menor tempo: %d\n", heapmin[1].tempo);
                } else {
                    printf("Heapmin está vazio.\n");
                }
                break;

            case 9:
                n = 0;
                n = inserir_varias_tarefas(&heapmax, &heapmin, n);
                break;
                
            case 0:
                exit(0);
                break;

            default:
                printf("Opção inválida\n");
                break;
        }
    } while (escolha != 0);

    free(heapmax);
    free(heapmin);
    return 0;
}
