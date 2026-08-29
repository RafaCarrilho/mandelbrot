#include "pthread1.h"


static thread* vetor_threads[MAX_THREADS];
static int indice = 0;

int criador_thread (int linhas, int threads, int largura, int altura, int max_iteracoes, int* vetor_imagem){
    int base = linhas / threads;
    int sobra = linhas % threads;

    for (int t=0; t<threads; t++){
        thread* fio = (thread*) malloc (sizeof(thread));
        if (fio == NULL){
            for (int i =0; i<indice; i++){
                free(vetor_threads[i]);
                
            }
            return -1;
        } else {
            if (t<sobra){
                fio-> tarefas = base+1;
                fio-> linha_inicial = t*(base+1);
                fio-> largura = largura;
                fio-> altura = altura;
                fio-> max_iteracoes = max_iteracoes;
                fio-> vetor_imagem = vetor_imagem;
                vetor_threads[t] =fio;
                indice++;
            } else {
                fio->tarefas = base;
                fio->linha_inicial = (sobra*(base+1) + (t-sobra)*base);
                fio-> largura = largura;
                fio-> altura = altura;
                fio-> max_iteracoes = max_iteracoes;
                fio-> vetor_imagem = vetor_imagem;
                vetor_threads[t] =fio;
                indice++; 
            }
        }
    }

    return 0;
}


