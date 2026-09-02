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

typedef struct {
     int tarefas; //vulgo linhas, quantas linhas vai receber
     int linha_inicial;
     int largura; 
     int* vetor_preenchido; //Já preenchido
     char* texto; //A thread vai guardar o texto pronto aqui
     size_t bytes;

} thread_escritora;

thread ** criador_thread (int largura, int altura, int threads, int max_iteracoes, int* vetor_imagem);
void *funcao_enviada(void *arg);

thread_escritora ** criador_thread_escritora (int largura, int altura, int threads, int* vetor_imagem);
void *funcao_enviada_2(void *arg);

#endif