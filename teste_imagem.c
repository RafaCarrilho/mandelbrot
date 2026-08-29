#include <stdio.h>
#include "escape.h"
#include "imagem.h"

int main() {
    int largura = 4, altura = 4, max_iteracoes = 50;
    int esperado[4][4] = {
        {5, 10, 10, 10},
        {5, 15, 30, 25},
        {255, 255, 255, 255},
        {5, 15, 30, 25}
    };
    int tudo_ok = 1;

    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            double cr = mapeia_x(px, largura);
            double ci = mapeia_y(py, altura);
            int iteracoes = escape(cr, ci, max_iteracoes);
            int cor = intensidade_cor(iteracoes, max_iteracoes);

            printf("%3d ", cor);
            if (cor != esperado[py][px]) tudo_ok = 0;
        }
        printf("\n");
    }

    printf(tudo_ok ? "\nBateu com o gabarito oficial.\n" : "\nDIVERGIU do gabarito oficial.\n");
    return 0;
}