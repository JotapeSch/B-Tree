#include <stdio.h>
#include <stdlib.h>

void limpar_tela(void);

typedef struct pagina{
    int m;              //quantidade de chaves atual na pagina
    int *keys;          //capacidade max para 2d+1 
    struct pagina **pont;   //ponteiro

} Pagina;

typedef struct ArvB{
    int d;
    Pagina *raiz; //inicio de tudo aqui

} ArvB;

typedef struct pilha{
    Pagina *caminho;
    struct pilha *prox; 
} Pilha;

Pagina *criarPagina(int d);
ArvB *criarArvore(int ordem);
void BuscaArvB(int x, ArvB *arvB, Pagina **ponteiro, int *f, int *g, Pilha **topo);
void InsereBnonVazia(Pagina *pt, int g, int x, Pagina *ptDir);
void InsereB(int x, ArvB *raiz,Pagina **pt,int *f,int *g,Pilha **topo, int ordem);
void Inserir(ArvB** raiz, int ordem);


//auxiliares
void Empilhar(Pilha **topo, Pagina *pt);
Pagina* Desempilhar(Pilha **topo);