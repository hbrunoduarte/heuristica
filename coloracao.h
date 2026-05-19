#ifndef _COLORACAO_H
#define _COLORACAO_H

#ifdef _WIN32
#include <windows.h>
#endif

#define MIN_VERTICES 5
#define RANGE_ADD_VERTICES 15

typedef struct Vertice {
    int indice;
    struct Vertice* proximo;
} Vertice;

typedef struct Adjacencias {
    int cor;
    Vertice* head;
} Adjacencias;

typedef struct Grafo {
    int numVertices;
    int verticesColoridos;
    Adjacencias* listaAdj;
} Grafo;

void printaGrafo(Grafo* grafo, int printaCor);
void resetaCores(Grafo* grafo);

// Algoritmo de coloração sobre o grafo
int coloreGuloso(Grafo *grafo, int vInicial);

// Verificações sobre o grafo
int temVerticeSemCor(Grafo *g);
int estaBemColorido(Grafo *g);

// Cria ou destrói o grafo
Grafo* criaGrafo(int n);
void freeGrafo(Grafo *g);

#endif