#ifndef _ARQUIVO_H
#define _ARQUIVO_H

#include "coloracao.h"

#define NOME_ARQUIVO "grafo.txt"
#define QTD_VERTICES 500
#define BUFFER_LEITURA 1 + 2*QTD_VERTICES

Grafo* lerArquivo();
void escreveArquivo(Grafo* grafo);

#endif