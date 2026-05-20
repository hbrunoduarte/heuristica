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

void coloreGulosoAuxiliar(Grafo *grafo, int verticeAtual, int *paletaCores) {

    Vertice *vAdj = grafo->listaAdj[verticeAtual].head;
    while (vAdj) {
        int corVizinho = grafo->listaAdj[vAdj->indice].cor;
        if (corVizinho != 0) paletaCores[corVizinho-1] = 0;
        vAdj = vAdj->proximo;
    }

    int corEscolhida = 1;
    for (; corEscolhida < grafo->numVertices; corEscolhida++)
        if (paletaCores[corEscolhida-1])
            break;

    grafo->listaAdj[verticeAtual].cor = corEscolhida;

    vAdj = grafo->listaAdj[verticeAtual].head;
    while (vAdj) {
        int corVizinho = grafo->listaAdj[vAdj->indice].cor;
        if (corVizinho != 0) paletaCores[corVizinho-1] = 1;
        vAdj = vAdj->proximo;
    }

    grafo->verticesColoridos++;

    Vertice *proxVertice = grafo->listaAdj[verticeAtual].head;
    while (temVerticeSemCor(grafo) && proxVertice != NULL) {
        if (grafo->listaAdj[proxVertice->indice].cor == 0)
            coloreGulosoAuxiliar(grafo, proxVertice->indice, paletaCores);
        proxVertice = proxVertice->proximo;
    }
}

int coloreGuloso(Grafo *grafo, int vInicial) {

    int *paletaCores = malloc(sizeof(int) * grafo->numVertices);
    for (int i = 0; i < grafo->numVertices; i++)
        paletaCores[i] = 1;

    for (int i = (vInicial + 1) % grafo->numVertices; i != vInicial || temVerticeSemCor(grafo); i = (i+1) % grafo->numVertices)
        if (grafo->listaAdj[i].cor == 0) {
            coloreGulosoAuxiliar(grafo, i, paletaCores);
            if (i == vInicial) break;
        }
        
        
    int numCores = 0;

    for (int i = 0; i < grafo->numVertices && numCores < grafo->numVertices; i++) {
        if (paletaCores[grafo->listaAdj[i].cor-1]) {
            numCores++;
            paletaCores[grafo->listaAdj[i].cor-1] = 0;
        }
    }

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

    Grafo *grafo;
    grafo = lerArquivo();

    int numCores = grafo->numVertices;

    // testa todos os vértices
    for (int i = 0; i < grafo->numVertices && numCores != 1; i++) {
        resetaCores(grafo);
        int temp = coloreGuloso(grafo, i);
        numCores = temp < numCores ? temp : numCores;
    }

    printf("\n-----------------------------------\n");
    printf("Guloso: Grafo colorido com %d cores\n", numCores);
    printf("-----------------------------------\n\n");

    escreveArquivo(grafo);
    // printaGrafo(grafo, 1);
    
    printf("Está corretamente colorido? %s\n", estaBemColorido(grafo) ? "sim" : "não");

    freeGrafo(grafo);

    return 0;
}