#include <stdio.h>
#ifndef PTHREAD1_H
#define PTHREAD1_H

typedef struct {
     int tarefas; 
     int linha_inicial;
     int largura; 
     int altura;
     int max_iteracoes; 
     int* vetor_imagem;
} thread;


int criador_thread (int linhas, int threads, int largura, int altura, int max_iteracoes, int* vetor_imagem);

#endif