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

    

    escreveArquivo(grafo);
    // printaGrafo(grafo, 1);
    
    printf("Está corretamente colorido? %s\n", estaBemColorido(grafo) ? "sim" : "não");

    freeGrafo(grafo);

    return 0;
}