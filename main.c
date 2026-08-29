#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "imagem.h"
#include "serial.h"

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
    largura = (int)largura_long;

    long altura_long = strtol(argv[2], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    altura = (int)altura_long;

    long max_iteracoes_long = strtol(argv[3], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    max_iteracoes = (int)max_iteracoes_long;

    long max_threads_long = strtol(argv[4], &sobra, 10);
    if (*sobra != '\0') {
        fprintf(stderr, "Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    max_threads = (int)max_threads_long;

    
    if (largura <= 0) {
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
    }
    
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
    return 0;
}