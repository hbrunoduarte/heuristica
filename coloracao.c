#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heap.h"
#include "coloracao.h"
#include "arquivo.h"

void resetaCores(Grafo* grafo) {
    for(int i = 0; i < grafo->numVertices; i++) {
        grafo->listaAdj[i].cor = 0;
    }
    grafo->verticesColoridos = 0;
}

int temVerticeSemCor(Grafo *g) {
    return g->verticesColoridos != g->numVertices;
}

void atualizarSaturacao(Grafo *g, MaxHeap *heap, Adjacencias *v) {

    int *cores = calloc(g->numVertices, sizeof(int));
    
    Vertice *vizinho = v->head;
    int qtdVizinhosColoridos = 0;
    while (vizinho != NULL) {
        int cor = g->listaAdj[vizinho->indice].cor;
        if (cor != 0) {
            qtdVizinhosColoridos++;
            cores[cor-1] = 1;
        }
        vizinho = vizinho->proximo;
    }

    int satAntiga = v->sat;
    int novaSaturacao = 0;
    for (int i = 0; i < g->numVertices && novaSaturacao < qtdVizinhosColoridos; i++)
        if (cores[i] == 1)
            novaSaturacao++;
    
    v->sat = novaSaturacao;
    free(cores);

    if (novaSaturacao != satAntiga)
        atualizarPrioridade(heap, v);

}

int DSatur(Grafo *grafo) {

    MaxHeap *heapVertices = criarHeap(grafo->numVertices);
    for (int i = 0; i < grafo->numVertices; i++)
        inserir(heapVertices, grafo->listaAdj + i);
    
    int *cores = calloc(grafo->numVertices, sizeof(int));

    while (!estaVazio(heapVertices)) {

        Adjacencias *v = extrairMax(heapVertices);

        Vertice *aux = v->head;
        while (aux != NULL) {
            int cor = grafo->listaAdj[aux->indice].cor;
            if (cor != 0) cores[cor-1] = 1;
            grafo->listaAdj[aux->indice].grau--;
            aux = aux->proximo;
        }

        int i = 0;
        for (; i < grafo->numVertices; i++)
            if (cores[i] == 0)
                break;
            else cores[i] = 0;
        
        v->cor = i+1;

        for (; i < grafo->numVertices; i++)
            cores[i] = 0;

        aux = v->head;
        while (aux != NULL) {
            atualizarSaturacao(grafo, heapVertices, &grafo->listaAdj[aux->indice]);
            aux = aux->proximo;
        }
    }

    int numCores = 0;
    for (int i = 0; i < grafo->numVertices; i++) {
        int posCor = grafo->listaAdj[i].cor - 1;
        if (cores[posCor] == 0) {
            cores[posCor] = 1;
            numCores++;
        }
    }

    free(cores);
    liberarHeap(heapVertices);

    return numCores;
}

void printaGrafo(Grafo* grafo, int printaCor) {
    printf("Lista de Adjacências: \n");

    for(int i = 0; i < grafo->numVertices; i++) {

        if(printaCor) {
            printf("[ (COR %d) %d:", grafo->listaAdj[i].cor, i);
        } else {
            printf("[ %d:", i);    
        }
        
        Vertice* aux = grafo->listaAdj[i].head;

        while(aux != NULL) {
            printf(" -> %d", aux->indice);
            aux = aux->proximo;
        }

        printf(" ]\n");
    }
}

Grafo* criaGrafo(int n) {
    Grafo* grafo = malloc(sizeof(Grafo));
    grafo->numVertices = n;
    grafo->verticesColoridos = 0;
    grafo->listaAdj = malloc(sizeof(Adjacencias)*n);

    for(int i = 0; i < n; i++) {
        grafo->listaAdj[i].head = NULL;
        grafo->listaAdj[i].cor = 0;
        grafo->listaAdj[i].grau = 0;
        grafo->listaAdj[i].sat = 0;
    }
        
    return grafo;
}

int estaBemColorido(Grafo *g) {
    for (int i = 0; i < g->numVertices; i++) {
        Vertice *vAdj = g->listaAdj[i].head;
        while (vAdj) {
            if (g->listaAdj[vAdj->indice].cor == g->listaAdj[i].cor) {
                printf("\n[%i -> %i]\n", i, vAdj->indice);
                return 0;
            }
            vAdj = vAdj->proximo;
        }
    }
    return 1;
}

void freeGrafo(Grafo *g) {

    if (!g || g->numVertices == 0 || !g->listaAdj) return;

    for (int i = 0; i < g->numVertices; i++) {
        Vertice *vAdj = g->listaAdj[i].head;
        while (vAdj) {
            Vertice *aux = vAdj;
            vAdj = vAdj->proximo;
            free(aux);
        }
    }
    free(g->listaAdj);
    g->numVertices = 0;
    g->verticesColoridos = 0;
}

int main(int argc, char *argv[]) {
    
    srand(time(NULL));

    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    Grafo *grafo = lerArquivo();

    int numCores = DSatur(grafo);

    printf("\nGrafo colorido com %d cores\n\n", numCores);
    printf("Está corretamente colorido? %s\n\n", estaBemColorido(grafo) ? "sim" : "não");

    escreveArquivo(grafo);
    // printaGrafo(grafo, 1);

    freeGrafo(grafo);

    return 0;
}