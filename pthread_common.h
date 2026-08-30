#include <stdio.h>
#ifndef PTHREAD_COMMON_H
#define PTHREAD_COMMON_H


typedef struct {
     int tarefas;
     int threads; 
     int linha_inicial;
     int largura; 
     int altura;
     int max_iteracoes; 
     int* vetor_imagem;
} thread;

thread ** criador_thread (int largura, int altura, int threads, int max_iteracoes, int* vetor_imagem);
void *funcao_enviada(void *arg);

thread ** criador_thread_2 (int largura, int altura, int threads, int max_iteracoes, int* vetor_imagem);
void *funcao_enviada_2(void *arg);

#endif