#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "imagem.h"
#include "serial.h"
#include "openmp.h"
#include "pthread_common.h"

#define LARGURA_MAX 5000
#define ALTURA_MAX 5000
#define ITERACOES_MAX 1000
#define THREADS_MAX 40

static pthread_t vetor_pthreads[THREADS_MAX];

int grava_tempo (double tempo, const char *arquivo, const char *operacao){
    FILE *file = fopen(arquivo, "a");
    if (file == NULL) {
        return -1;
    }
    fprintf (file, "%s: %.6lfs\n",operacao, tempo);
    fclose(file);
    return 0;
}

int reseta_arquivo (){
    FILE *limpa = fopen("times.txt", "w");
    if (limpa == NULL) {
        return -1;
    }
    fclose(limpa);
    return 0;
}

int main(int argc, char *argv[]) {
    struct timespec inicio, fim;

    if (reseta_arquivo() == -1){
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo times.txt\n");
        return -1;
    }
    
    
    if (argc != 5) {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }

    char *sobra;
    int largura, altura, max_iteracoes, max_threads;

    long largura_long = strtol(argv[1], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    if (largura_long > LARGURA_MAX || largura_long <= 0){
        fprintf(stderr, "Erro: Número de largura fora da faixa permitida\n");
        return -1;
    }
    largura = (int)largura_long;

    

    long altura_long = strtol(argv[2], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    if (altura_long > ALTURA_MAX || altura_long <= 0){
        fprintf(stderr, "Erro: Número de altura fora da faixa permitida\n");
        return -1;
    }
    altura = (int)altura_long;


    long max_iteracoes_long = strtol(argv[3], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    if (max_iteracoes_long > ITERACOES_MAX || max_iteracoes_long <= 0){
        fprintf(stderr, "Erro: Número de iterações fora da faixa permitida\n");
        return -1;
    }
    max_iteracoes = (int)max_iteracoes_long;


    long max_threads_long = strtol(argv[4], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    if (max_threads_long > THREADS_MAX || max_threads_long <= 0){
        fprintf(stderr, "Erro: Número de threads fora da faixa permitida\n");
        return -1;
    }
    max_threads = (int)max_threads_long;

    
/*     if (largura <= 0) {
        fprintf(stderr, "Erro: largura deve ser um numero inteiro positivo\n");
        return -1;
    }
    if (altura <= 0) {
        fprintf(stderr, "Erro: altura deve ser um numero inteiro positivo\n");
        return -1;
    }
    if (max_iteracoes <= 0) {
        fprintf(stderr, "Erro: max_iteracoes deve ser um numero inteiro positivo\n");
        return -1;
    }
    if (max_threads <= 0) {
        fprintf(stderr, "Erro: num_threads deve ser um numero inteiro positivo\n");
        return -1;
    } */
    

    //Execução serial
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    int *vetor_imagem = preenche_vetor(largura, altura, max_iteracoes);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    if (vetor_imagem == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem\n");
        return -1;
    }

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    
    int arquivo_desenhado = desenha_arquivo(vetor_imagem, largura, altura, "mandelbrot_rac4_serial.pgm");
    if (arquivo_desenhado == -1){
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo mandelbrot_rac4_serial.pgm\n");
        return -1;
    }
    grava_tempo(tempo, "times.txt", "Serial");
    free(vetor_imagem);

    
    //execução OpenMP
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    vetor_imagem = preenche_vetor_mp(largura, altura, max_iteracoes, max_threads);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    if (vetor_imagem == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem (OpenMP)\n");
        return -1;
    }

    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    arquivo_desenhado = desenha_arquivo(vetor_imagem, largura, altura, "mandelbrot_rac4_openmp.pgm");
    if (arquivo_desenhado == -1){
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo mandelbrot_rac4_openmp.pgm\n");
        return -1;
    }
    grava_tempo(tempo, "times.txt", "OpenMP");
    free(vetor_imagem);

    //execução pthreads1
    int *vetor_imagem_pthread = (int*) malloc ((largura*altura)*sizeof(int));
    if (vetor_imagem_pthread == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem (Pthread1)\n");
        return -1;
    } 
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    thread** vetor_threads = criador_thread (largura, altura, max_threads, max_iteracoes, vetor_imagem_pthread);
    if (vetor_threads == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para criação de struct thread\n");
        return -1;
    }
    

    for (int i=0; i< max_threads; i++){
        if (pthread_create(&vetor_pthreads[i], NULL, funcao_enviada, vetor_threads[i])!=0){
            for (int p =0; p<i; p++){
                pthread_join (vetor_pthreads[p], NULL); 
            }
            
            free(vetor_imagem_pthread);
            for (int j =0; j<max_threads; j++){
                free (vetor_threads[j]);   //liberamos cada struct
            }
            free(vetor_threads); // liberamos o vetor por fim (feito matriz :D)
            fprintf(stderr, "Erro: falha na criação de pthread\n");
            return -1;
        }
        //Pthread_create precisa de alguns ingredientes, um vetor de pthread_t* (ou só 1), quase 
        //sempre um NULL, a funcao que ele vai executar, a struct que contem os ingredientes DESSA função

    }
    for (int i=0; i< max_threads; i++){ //agora vamos waitar cada thread antes de desenhar o nosso arquivo
        pthread_join (vetor_pthreads[i], NULL);     
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    arquivo_desenhado = desenha_arquivo(vetor_imagem_pthread, largura, altura, "mandelbrot_rac4_pthreads1.pgm");
    if (arquivo_desenhado == -1){
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo mandelbrot_rac4_pthreads1.pgm\n");
        return -1;
    }
    grava_tempo(tempo, "times.txt", "Pthreads1");
    
    
    free(vetor_imagem_pthread);
    for (int i =0; i<max_threads; i++){
        free (vetor_threads[i]);   //liberamos cada struct
    }
    free(vetor_threads); // liberamos o vetor por fim (feito matriz :D)

    
    //execução de pthreads2
    
   clock_gettime(CLOCK_MONOTONIC, &inicio);
    vetor_imagem = preenche_vetor(largura, altura, max_iteracoes);
    if (vetor_imagem == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem\n");
        return -1;
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);


    thread_escritora** vetor_escritora = criador_thread_escritora (largura, altura, max_threads, vetor_imagem);
    if (vetor_escritora == NULL){
        fprintf(stderr, "Erro: falha ao alocar memoria para criação de struct thread\n");
        return -1;
    }
    

    for (int i=0; i< max_threads; i++){
        if (pthread_create(&vetor_pthreads[i], NULL, funcao_enviada_2, vetor_escritora[i])!=0){
            for (int p =0; p<i; p++){
                pthread_join (vetor_pthreads[p], NULL); 
            }
            
            free(vetor_imagem);
            for (int j =0; j<max_threads; j++){
                free (vetor_escritora[j]);   //liberamos cada struct
            }
            free(vetor_escritora); // liberamos o vetor por fim (feito matriz :D)
            fprintf(stderr, "Erro: falha na criação de pthread\n");
            return -1;
        }
        //Pthread_create precisa de alguns ingredientes, um vetor de pthread_t* (ou só 1), quase 
        //sempre um NULL, a funcao que ele vai executar, a struct que contem os ingredientes DESSA função

    }
    for (int i=0; i< max_threads; i++){ //agora vamos waitar cada thread antes de desenhar o nosso arquivo
        pthread_join (vetor_pthreads[i], NULL);     
    }
    FILE *arquivo = fopen("mandelbrot_rac4_pthreads2.pgm", "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: Abertura de arquivo sem sucesso (Pthreads2)");
        return -1;
    }

    for (int i =0; i< max_threads; i++){
        fwrite(vetor_escritora[i]->texto, 1, vetor_escritora[i]->bytes, arquivo);
        free(vetor_escritora[i]->texto);
    }
    fclose(arquivo);
    
    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    grava_tempo(tempo, "times.txt", "Pthreads2");
    
    
    free(vetor_imagem);
    for (int i =0; i<max_threads; i++){
        free (vetor_escritora[i]);   //liberamos cada struct
    }
    free(vetor_escritora); // liberamos o vetor por fim (feito matriz :D)
    return 0;
}