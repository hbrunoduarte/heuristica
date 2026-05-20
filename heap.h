#ifndef _HEAP_H
#define _HEAP_H

#include "coloracao.h"

typedef struct MaxHeap {
    Adjacencias** dados; // Array de ponteiros para Adjacencias do grafo
    int capacidade;
    int tamanho;
} MaxHeap;

MaxHeap* criarHeap(int capacidade);
void inserir(MaxHeap* heap, Adjacencias* adj);
Adjacencias* extrairMax(MaxHeap* heap);
void liberarHeap(MaxHeap* heap);

#endif