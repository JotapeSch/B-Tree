#ifndef JPRINT_ARVB_H
#define JPRINT_ARVB_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "JArvB.h"


typedef struct elemento{
    Pagina *page;
    struct elemento *prox;
} elemento;

typedef struct Fila{
    elemento *fim, *inicio;
}Fila;

Fila* inicializacao(){
    Fila *queue = (Fila*)malloc(sizeof*queue);
    queue->inicio = NULL;
    queue->fim = NULL;

    return queue;
}

void InserirFila(Fila *queue, Pagina *page){
    elemento *novo = (elemento*) malloc(sizeof(elemento));
    novo->page = page;
    novo->prox = NULL;

    if(queue->fim == NULL){
        queue->inicio = novo;
        queue->fim = novo;
    } else {
        queue->fim->prox = novo;
        queue->fim = novo;
    }
}

elemento *remover(Fila *fila){
    if(fila->inicio == NULL){
        return NULL;
    }
    elemento *remo = fila->inicio;
    fila->inicio = remo->prox;

    if(fila->inicio == NULL){
        fila->fim = NULL;
    }
    remo->prox = NULL;
    return remo;

}

void Nivel(Pagina *arv, int salto_linha, int colunas){
    
    Fila *fila = inicializacao();
    InserirFila(fila, arv);
    int nivel = 0;
    
    while(fila->inicio != NULL){
    
    
        elemento *aux = fila->inicio; //auxiliar so pra nao mexer na fila diretamente
        int quantidadeNaFila = 0; // contagem do nivel

        while(aux != NULL){ //faco a contagem para cada pagina de quantos elementos tem
            quantidadeNaFila++;
            aux = aux->prox;
        }

        int linha = 2 + nivel*salto_linha;
        int passo = 20;
        int colunaI = colunas / 2 -((quantidadeNaFila - 1)*passo)/2;
        mvprintw(linha, 1, "Nivel %d:", nivel); //printo o nivel com ncurses
        
        for (int i = 0; i < quantidadeNaFila; i++){
            int coluna = colunaI+ i * passo;
            elemento *remo = remover(fila); //desenfileiro o primeiro da lista

            mvprintw(linha, coluna, "[");

            for (int k = 0; k < remo->page->m; k++) {
                if (k > 0) {
                    printw(" | ");
                }

                printw("%d", remo->page->keys[k]);//printo oq foi removido
            }

            printw("]  ");

            for (int j = 0; j <= remo->page->m; j++){
                if(remo->page->pont[j] != NULL){
                    InserirFila(fila, remo->page->pont[j]); //enfileiro o prox
                }
            }

            free(remo);
        }
        printw("\n");
        nivel++;
        
    }
}


void ImprimeArvB(ArvB*arvore){
    initscr(); // inicioo o modo ncurses...
    clear();  // limpo a tela
    int linhas, colunas;
    getmaxyx(stdscr, linhas, colunas); //esse stdscr eh a tela principal do ncurses, n entendi bem...
    int salto_linha = 1;

    if(arvore->raiz == NULL){
        //arvore vazia
        // n esta nem entrando
        mvprintw(salto_linha/2, colunas/2-7, "Arvore vazia!");
    } else {
        mvprintw(salto_linha, colunas/2-2, " ");	
        Nivel(arvore->raiz, salto_linha, colunas);
    }
    
    printw("\n\n\n\nPressione qualquer tecla...");
    refresh(); //atualizo a tlea
    getch(); // espero alguem clicar em algo
    endwin();//finaliza e volta ao terminal
}

#endif