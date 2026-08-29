#include <stdio.h>
#ifndef IMAGEM_H
#define IMAGEM_H


double mapeia_horizontal(int coluna, int largura);
double mapeia_vertical(int linha, int altura);
int calcula_intensidade (int iteracoes, int max_iteracoes);
int desenha_arquivo(int *vetor_imagem, int largura, int altura, const char *nome_arquivo);

#endif