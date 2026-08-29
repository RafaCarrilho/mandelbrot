#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "serial.h"

int main(int argc, char *argv[]) {

    if (argc != 5) {
        printf("Uso: (claude completa a mensagem de erro)");
        return -1;
    }

    char *sobra;
    int largura, altura, max_iteracoes, max_threads;

    long largura_long = strtol(argv[1], &sobra, 10);
    if (*sobra != '\0') {
        printf("Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    largura = (int)largura_long;

    long altura_long = strtol(argv[2], &sobra, 10);
    if (*sobra != '\0') {
        printf("Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    altura = (int)altura_long;

    long max_iteracoes_long = strtol(argv[3], &sobra, 10);
    if (*sobra != '\0') {
        printf("Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    max_iteracoes = (int)max_iteracoes_long;

    long max_threads_long = strtol(argv[4], &sobra, 10);
    if (*sobra != '\0') {
        printf("Uso: ./mandelbrot largura altura max_iteracoes num_threads\n");
        return -1;
    }
    max_threads = (int)max_threads_long;

    
    if (largura <= 0) {
        printf("Uso: (claude completa a mensagem de erro)");
        return -1;
    }
    if (altura <= 0) {
        printf("Uso: (claude completa a mensagem de erro)");
        return -1;
    }
    if (max_iteracoes <= 0) {
        printf("Uso: (claude completa a mensagem de erro)");
        return -1;
    }
    if (max_threads <= 0) {
        printf("Uso: (claude completa a mensagem de erro)");
        return -1;
    }

    int *vetor_imagem = preenche_vetor(largura, altura, max_iteracoes);
    int arquivo_desenhado = desenha_arquivo(vetor_imagem, largura, altura, "mandelbrot_rac4_serial.pgm");

    return 0;
}