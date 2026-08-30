#include <stdlib.h>
#include "pthread1.h"
#include "imagem.h"
#include "escape.h"


static int indice = 0;
thread ** criador_thread (int largura, int altura, int threads, int max_iteracoes, int* vetor_imagem){
    
    indice =0;
    
    thread** vetor_threads = (thread**) malloc (threads *sizeof (thread*));
    if (vetor_threads == NULL){
        return NULL;
    }
    
    int base = altura / threads; //quantas tarefas uma thread normal recebe
    int sobra = altura % threads; //quantas threads vao receber uma tarefa a mais

    for (int t=0; t<threads; t++){
        thread* fio = (thread*) malloc (sizeof(thread));
        if (fio == NULL){
            for (int i =0; i<indice; i++){
                free(vetor_threads[i]);
                
            }
            return NULL;
        } else {
            if (t<sobra){ //vamos dizer que sobrou 2 threads, as threads 0 e 1 são as sortudas
                fio-> tarefas = base+1; //aqui a tarefa extra
                fio-> linha_inicial = t*(base+1);
                fio-> largura = largura;
                fio-> altura = altura;
                fio-> max_iteracoes = max_iteracoes;
                fio-> vetor_imagem = vetor_imagem;
                vetor_threads[t] =fio;
                indice++;
            } else {
                fio->tarefas = base;
                fio->linha_inicial = (sobra*(base+1) + (t-sobra)*base); //aqui eu preciso calcular todas 
                fio-> largura = largura;                                //as que tiveram tarefa extra como linha base     
                fio-> altura = altura;                                  //e ai multiplicar pela base mas tirando as "sobras" 
                fio-> max_iteracoes = max_iteracoes;                    //que ja foram calculadas (t - as irregulares)
                fio-> vetor_imagem = vetor_imagem;
                vetor_threads[t] =fio;
                indice++; 
            }
        }
    }
    
    return vetor_threads;
}


void *funcao_enviada(void *arg){
    thread *fio = (thread *) arg; //tem que colocar esse cast aqui, pra o argumento virar o que eu preciso

    if (fio->vetor_imagem == NULL){
        return NULL;
    }

    for (int linha = fio->linha_inicial; linha < (fio->linha_inicial + fio-> tarefas); linha ++){
        for (int coluna = 0; coluna < fio->largura; coluna ++){ 
            double cr= mapeia_horizontal (coluna, fio->largura);
            double ci = mapeia_vertical (linha, fio->altura);

            int iteracoes = iteracoes_feitas(cr, ci, fio->max_iteracoes);
            int intensidade = calcula_intensidade (iteracoes, fio->max_iteracoes);

            fio->vetor_imagem[(linha*fio->largura)+coluna] = intensidade;
        }
    }
    return NULL;
}

