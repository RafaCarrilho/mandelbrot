#include "escape.h"

int escape (double cr, double ci, int max_iteracoes){
    int iteracoes =0;
    double zr=0; 
    double zi=0;
    
    while (zr*zr + zi*zi <= 4 && iteracoes < max_iteracoes){
    
        double zr_novo = zr*zr - zi*zi + cr;    
        double zi_novo = 2*zr*zi + ci;   
        zr = zr_novo;
        zi = zi_novo; 

        iteracoes++; 
    }
    return iteracoes;
    
}