#include "imagem.h"

double mapeia_horizontal(int coluna, int largura){
    double cr;
    
    cr = -2.0 + ((double)coluna/largura) * 3.0;
    return cr;
}

double mapeia_vertical(int linha, int altura){
    double ci;
    
    ci = 1.5 - ((double)linha/altura) * 3.0;
    return ci;
}

int calcula_intensidade (int iteracoes, int max_iteracoes){
    int intensidade = (iteracoes*255)/max_iteracoes;
    return intensidade;
}

int desenha_arquivo(int *vetor_imagem, int largura, int altura, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        return -1;
    }
    
    for (int linha = 0; linha < altura; linha++) {
        for (int coluna = 0; coluna < largura; coluna++) {
            fprintf(arquivo, "%d ", vetor_imagem[linha*largura + coluna]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return 0;
}
