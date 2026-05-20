#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

typedef struct MaxHeap {
    Adjacencias** dados; // Array de ponteiros para Adjacencias do grafo
    int capacidade;
    int tamanho;
} MaxHeap;

// Inicializa o Heap
MaxHeap* criarHeap(int capacidade) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    heap->dados = (Adjacencias**)malloc(capacidade * sizeof(Adjacencias*));
    return heap;
}

// Define o critério de prioridade (Retorna 1 se 'a' tem mais prioridade que 'b')
int temMaiorPrioridade(Adjacencias* a, Adjacencias* b) {
    if (a->sat != b->sat) {
        return a->sat > b->sat;
    }
    return a->grau > b->grau;
}

void trocar(Adjacencias** a, Adjacencias** b) {
    Adjacencias* temp = *a;
    *a = *b;
    *b = temp;
}

// Reorganiza o Heap de baixo para cima (usado na Inserção)
void heapifyUp(MaxHeap* heap, int indice) {
    int pai = (indice - 1) / 2;

    // Se o nó atual tem maior prioridade que o pai, eles trocam de lugar
    if (indice > 0 && temMaiorPrioridade(heap->dados[indice], heap->dados[pai])) {
        trocar(&heap->dados[indice], &heap->dados[pai]);
        heapifyUp(heap, pai); // Continua subindo recursivamente
    }
}

// Insere um novo elemento no Heap
void inserir(MaxHeap* heap, Adjacencias* adj) {
    if (heap->tamanho == heap->capacidade) {
        printf("Erro: Heap cheio!\n");
        return;
    }

    // Insere no final e ajusta subindo
    heap->dados[heap->tamanho] = adj;
    heapifyUp(heap, heap->tamanho);
    heap->tamanho++;
}

// Reorganiza o Heap de cima para baixo (usado na Extração)
void heapifyDown(MaxHeap* heap, int indice) {
    int maior = indice;
    int esq = 2 * indice + 1;
    int dir = 2 * indice + 2;

    // Verifica se o filho esquerdo é maior que a raiz atual
    if (esq < heap->tamanho && temMaiorPrioridade(heap->dados[esq], heap->dados[maior])) {
        maior = esq;
    }

    // Verifica se o filho direito é maior que o maior atual
    if (dir < heap->tamanho && temMaiorPrioridade(heap->dados[dir], heap->dados[maior])) {
        maior = dir;
    }

    // Se o maior não for a raiz, troca e continua descendo
    if (maior != indice) {
        trocar(&heap->dados[indice], &heap->dados[maior]);
        heapifyDown(heap, maior);
    }
}

// Extrai e retorna o elemento de maior prioridade
Adjacencias* extrairMax(MaxHeap* heap) {
    if (heap->tamanho <= 0) {
        return NULL;
    }
    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->dados[0];
    }

    // O maior elemento está sempre na raiz (índice 0)
    Adjacencias* raiz = heap->dados[0];
    
    // Substitui a raiz pelo último elemento e diminui o tamanho
    heap->dados[0] = heap->dados[heap->tamanho - 1];
    heap->tamanho--;
    
    // Ajusta o heap descendo o novo elemento na raiz
    heapifyDown(heap, 0);

    return raiz;
}

void liberarHeap(MaxHeap* heap) {
    if (heap != NULL) {
        free(heap->dados);
        free(heap);
    }
}