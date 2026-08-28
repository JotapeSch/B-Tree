#include "JArvB.h"

/*
raizArvB          → estrutura ArvB inteira
raizArvB->d       → ordem d
raizArvB->raiz    → ponteiro para a página raiz
*/


void limpar_tela(void) {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}   

//Funcao que cria uma pagina, recebe apenas a ordem para fazer a inicializacao das chaves q tem na pagina
Pagina *criarPagina(int d){
    Pagina *page = malloc(sizeof(Pagina));
    page->m = 0;
    page->keys = malloc((2*d+1) * sizeof*page->keys);
    page->pont = malloc((2*d+2) * sizeof*page->pont);

    for(int i = 0; i <= 2*d+1; i++){
        page->pont[i] = NULL;
    }

    return page;
}

//funcao que cria a raiz da arvore e retorna ela
ArvB *criarArvore(int ordem){
    ArvB *raizArvB = malloc(sizeof(ArvB));
    raizArvB->d = ordem;
    raizArvB->raiz = NULL;

    return raizArvB;

}

//procedimento que vai buscar a chave em tal pagina e retorna por refencia se tem ou nao, tipo a BST
//se f = 1 -> encontrei x em ponteiroArvore.chaves[g]
//senao f = 0 nao contrei e o ponteiro aponta para a ultima pagina analisada, que eh uma folha! g eh onde x deve ser inserida
//g eh o indice que vai andar ate achar ou nao o x... tudo esta por referencia pois preciso retornar muitas coisas
void BuscaArvB(int x, ArvB *arvB, Pagina **ponteiro, int *f, int *g, Pilha **topo){
    *ponteiro = (*arvB).raiz;
    if (*ponteiro == NULL) {
        
        *f = 0;
        *g = 0;
        return;
    }

    while(1){
        Empilhar(topo, *ponteiro);
        *g = 0;
        while (*g < (*ponteiro)->m && x > (*ponteiro)->keys[*g]){
            (*g)++;
        }

        if(*g < (*ponteiro)->m && x == (*ponteiro)->keys[*g]){
            *f = 1;
            return;
        }

        if((*ponteiro)->pont[*g] == NULL){
            *f = 0;
            return;
        }
        (*ponteiro) = (*ponteiro)->pont[*g];
    }

}

void Cisao(Pilha** topo, Pagina **ponteiro, ArvB *arvore, int d){
    //p(esquerda fica com as chaves de 0 ate d-1)
    //q fica com d+1 ate 2d
    // d - meio sobre
    
    Pagina *P = *ponteiro, *Q = criarPagina(d);
    int meio = P->keys[d];

    int j = 0;
    for(int i = d+1; i <= 2*d; i++){
        Q->keys[j] = P->keys[i];
        j++;
    }
    j =0;
    for(int i = d+1; i<= 2*d + 1; i++){
        Q->pont[j] = P->pont[i];
        j++;
    }
    P->m = d;
    Q->m = d;
    Desempilhar(topo);
    
    if(*topo == NULL){
        // senao tem pai, vou criar uma page nova chamada R
        Pagina *R = criarPagina(d);
        R->keys[0] = meio;
        R->pont[0] = P;
        R->pont[1] = Q;
        R->m = 1;
        arvore->raiz = R;
    }  else { // se tiver pai, so chamar a funcao que insere em uma pagina nao folha
        Pagina *pai = (*topo)->caminho;
        int gPai = 0;
        while(gPai < pai->m && meio > pai->keys[gPai]){
            gPai++;
        }
        InsereBnonVazia(pai, gPai, meio, Q);
        
        if(pai->m == 2*d+1){ // se estourar novamente, chama a cisao dnv
            Cisao(topo, &pai, arvore, d);
        }
    }

}

//procedimento que vai inserir quando a pagina que queremos colocar chave nao esta vazia
//assim realocamos o jeito que esta o vetor com as chaves para colocar a chave
void InsereBnonVazia(Pagina *pt, int g, int x, Pagina *ptDir){
    for(int i = pt->m; i > g; i--){
        pt->keys[i] = pt->keys[i-1]; //aqui basicamente oq estava ah esquerda vai pra direita... pois precisamos colocar no g
        //ex: 10 20 40, queremos colocar 30, 40 vai pra direita e 30 fica no lugar de 40
        pt->pont[i+1] = pt->pont[i];
    }
    pt->keys[g] = x;
    pt->pont[g+1] = ptDir;
    pt->m = pt->m +1;
}

void InsereB(int x, ArvB *arv,Pagina **pt,int *f,int *g,Pilha **topo, int ordem){
    BuscaArvB(x, arv, pt, f, g, topo);
    if(*f == 1){
        printf("!!! Chave ja inserida!\n");
        return;
    }
    

    if (*pt == NULL){
        Pagina *raiz = criarPagina((*arv).d);
        raiz->keys[0] = x;
        raiz->m = 1;
        arv->raiz = raiz;
        return;
    }


    //pt[*g] = x; //parcialmente errada, pois preciso andar no ptfolha, pois as vezes a pagina nao esta vazia
    InsereBnonVazia(*pt, *g, x, NULL);

    if((*pt)->m == 2*(*arv).d + 1){
        Cisao(topo, pt, arv, ordem);
    }


}

/*
Aqui comeca uma estrutura auxiliar para guardar o caminho para fazer a cisao...
Pensei em fazer recursivamente o caminho e tals, porem nao consegui tirar nada, ai vou ir andando e guardando o caminho

Bom, se a busca der certo, supostamente tem que terminar assim:
topo → folha → pai → avô → raiz → NULL

*/

void JRemover(Pagina** ponteiro, ArvB *arvore, int x){
    Pilha *topo = NULL;
    int f = 0, g = 0;

    BuscaArvB(x, arvore, ponteiro, &f, &g, &topo);
    if (f == 0){
        printf("ARVORE ESTA VAZIA\n");
        return;
    }

    if((*ponteiro)->pont[0] == NULL){
        //estou em uma pagina folha
        for (int i = g; i < (*ponteiro)->m - 1; i++){
            (*ponteiro)->keys[i] = (*ponteiro)->keys[i+1];
        }
        (*ponteiro)->m--;

        /*
        if(soma < 2d){
            concatenacao()
        } else {
            redistribuicao()
        }
        */
    } else {
        //pagina interna... faz o sucessor
        Pagina *PageOriginal = *ponteiro; //guardo pois vou empilhar ate chegar no sucessor
        int gOriginal = g;

        Pagina *ptInterna = PageOriginal->pont[g+1]; //passo para achar o sucessor
        Empilhar(&topo, ptInterna); //comeco a empilhar

        while(ptInterna->pont[0] != NULL){
            ptInterna = ptInterna->pont[0];//anda ate chegar no sucessor 
            Empilhar(&topo, ptInterna);
        }
        int sucessor = ptInterna->keys[0]; //achamos o sucessor
        PageOriginal->keys[gOriginal] = sucessor; //na onde o elemento que queremos retirar estava localizado
        //eh trocado pelo sucessor 
        for (int i = g; i < (*ponteiro)->m - 1; i++){
            ptInterna->keys[i] = ptInterna->keys[i+1]; //como retiramos... apenas povo ele pra um indice anterior
        }
        ptInterna->m--;

        /*
        if(soma < 2d){
            concatenacao()
        } else {
            redistribuicao()
        }
        */
    }
}

void Empilhar(Pilha **topo, Pagina *pt){
    Pilha *novo = malloc(sizeof(Pilha));
    novo->caminho = pt;
    novo->prox = *topo;
    *topo = novo;
}

Pagina* Desempilhar(Pilha **topo){
    
    if(*topo == NULL){
        return NULL;
    }
    Pilha *remo = *topo;
    Pagina *pageRemovida = remo->caminho;

    (*topo) = remo->prox;
    free(remo);
    return pageRemovida;

}

/*
int main (){
    
    ArvB *tree = criarArvore(2);
    Pagina *ptPage = NULL;
    int f,g;

    //InsereB(2, tree, &ptPage,&f, &g);
    printf("%d\n", tree->raiz->keys[0]);
    return 0;
    
}
    */