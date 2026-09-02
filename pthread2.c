#include <stdlib.h>
#include "pthread_common.h"
#include "imagem.h"
#include "escape.h"


static int indice = 0;
thread_escritora ** criador_thread_escritora (int largura, int altura, int threads, int* vetor_imagem){
    
    indice =0;
    
    thread_escritora** vetor_threads = (thread_escritora**) malloc (threads *sizeof (thread_escritora*));
    if (vetor_threads == NULL){
        return NULL;
    }
    
    int base = altura / threads; //quantas tarefas uma thread normal recebe
    int sobra = altura % threads; //quantas threads vao receber uma tarefa a mais

    for (int t=0; t<threads; t++){
        thread_escritora* fio = (thread_escritora*) malloc (sizeof(thread_escritora));
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
                fio-> vetor_preenchido = vetor_imagem;
                fio-> texto = NULL;
                fio-> bytes = 0;
                vetor_threads[t] =fio;
                indice++;
            } else {
                fio->tarefas = base;
                fio->linha_inicial = (sobra*(base+1) + (t-sobra)*base); //aqui eu preciso calcular todas 
                fio-> largura = largura;                                //as que tiveram tarefa extra como linha base                                       //e ai multiplicar pela base mas tirando as "sobras"                    //que ja foram calculadas (t - as irregulares)
                fio-> vetor_preenchido = vetor_imagem;
                fio-> texto = NULL;
                fio-> bytes = 0;
                vetor_threads[t] =fio;
                indice++; 
            }
        }
    }
    
    return vetor_threads;
}


void *funcao_enviada_2(void *arg){
    thread_escritora *fio = (thread_escritora *) arg; //tem que colocar esse cast aqui, pra o argumento virar o que eu preciso

    if (fio->vetor_preenchido == NULL){
        return NULL;
    }
    FILE *stream = open_memstream(&fio->texto, &fio->bytes);
    if (stream == NULL){
        return NULL;
    }

    for (int linha = fio->linha_inicial; linha < fio->linha_inicial + fio->tarefas; linha++) {
        for (int coluna = 0; coluna < fio->largura; coluna++) {
            if (coluna != fio->largura-1){
                fprintf(stream, "%d ", fio->vetor_preenchido[linha*fio->largura + coluna]);
            }else{
                fprintf(stream, "%d", fio->vetor_preenchido[linha*fio->largura + coluna]);
            }
            
        }
        fprintf(stream, "\n");
    }
    fclose(stream);
    return NULL;
}

