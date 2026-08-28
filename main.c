#include <stdio.h>
#include <stdlib.h>
#include "JArvB.h"
#include "Jprint_arvB.h"

void Inserir(ArvB** raiz, int ordem){
    int chave;
    Pagina *pt = NULL;
    Pilha *topo = NULL;
    int f = 0;
    int g = 0;
    printf("Digite a chave para INSERIR: ");
    scanf("%d", &chave);
    InsereB(chave, *raiz, &pt, &f, &g, &topo, ordem);
}



void Menu(){
    int opcao, chave, ordem;
    Pilha *topo = NULL;
    
    do{
        printf("Qual a ordem da sua arvore?\n");
        scanf("%d", &ordem);
    } while(ordem <= 0);

    ArvB *raiz = criarArvore(ordem);
    
    while(1){
        printf("===================\n");
        printf("Arvore B - Inteiros");
        printf("\n===================\n");
        
        
            ImprimeArvB(raiz);
            printf("\n");
        

        printf("\n------------------ MENU ------------------\n");
        printf("1. Buscar chave\n");
        printf("2. Insercao de chave\n");
        printf("3. Remocao de chave\n");
        printf("4. Desalocacao da arvore\n");
        printf("------------------------------------------\n");

        printf("Escolha uma opcao: ");
        if(scanf("%d", &opcao) != 1) continue;

        switch (opcao)
        {
        case 1:{
            int f = 0, g = 0;
            Pagina *ptPage;
            if (raiz == NULL) {
                    printf("A arvore esta vazia! Nao ha o que buscar.\n");
                } else {
                    printf("Digite a chave para BUSCAR: ");
                    scanf("%d", &chave);
                    BuscaArvB(chave, raiz, &ptPage, &f, &g, &topo);
                    if (f == 1) 
                        printf("Chave %d FOI encontrada na arvore!\n", chave);
                    else 
                        printf("Chave %d NAO encontrada!\n", chave);
                }
                printf("\nPressione ENTER para continuar...");
            getchar();getchar();
            limpar_tela(); 
            break;
        }
 

        case 2:{
            Inserir(&raiz, ordem);
            printf("\nPressione ENTER para continuar...");
            getchar();getchar();
            limpar_tela();
            
            break;}

        case 3:
            {   
                Pagina* ptpage;
                printf("Digite a chave para REMOVER: ");
                scanf("%d", &chave);
                JRemover(&ptpage, raiz, chave);
                printf("\nPressione ENTER para continuar...");
                getchar();getchar();
                limpar_tela();
                
                break;
            }

        case 4:
            printf("Memoria liberada. Encerrando...\n");
            exit(0);
        default:
            printf("Opcao invalida!\n");
            break;

        }
    }

}



int main(){
    Menu();

    return 0;
}