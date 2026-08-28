#include "imagem.h"

double mapeia_x(int px, int largura){
    double cr;
    
    cr = -2.0 + ((double)px/largura) * 3.0;
    return cr;
}

double mapeia_y(int py, int altura){
    double ci;
    
    ci = 1.5 - ((double)py/altura) * 3.0;
    return ci;
}

int intensidade_cor (int iteracoes, int max_iteracoes){
    int intensidade = (iteracoes*255)/max_iteracoes;
    return intensidade;
}