#include <stdlib.h>
#include "escape.h"
#include "imagem.h"
#include "serial.h"

int *preenche_vetor(int largura, int altura, int max_iteracoes) {
    
    int *vetor_imagem = (int*) malloc ((largura*altura)*sizeof(int));
    if (vetor_imagem == NULL){
        return NULL;
    }   
    
    for (int linha =0; linha < altura; linha ++){
        for (int coluna = 0; coluna < largura; coluna ++){
            double cr= mapeia_horizontal (coluna, largura);
            double ci = mapeia_vertical (linha, altura);

            int iteracoes = iteracoes_feitas(cr, ci, max_iteracoes);
            int intensidade = calcula_intensidade (iteracoes, max_iteracoes);

            vetor_imagem[(linha*largura)+coluna] = intensidade;




        }
    }
    return vetor_imagem;
}