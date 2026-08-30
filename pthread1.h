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


thread ** criador_thread (int largura, int altura, int threads, int max_iteracoes, int* vetor_imagem);
void *funcao_enviada(void *arg);

#endif