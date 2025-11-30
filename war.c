//INCLUIR BIBLIOTECAS DE PRÉ-PROCESSAMENTO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//DEFINIÇÕES DE CONSTANTES
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_TERRITORIOS 5 

//ESTRUTURA PARA REPRESENTAR UM TERRITÓRIO - O MOLDE PARA A CRIAÇÃO DE TERRITÓRIOS
typedef struct{
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;


//FUNÇÃO PRINCIPAL
int main() {
    setlocale(LC_ALL,"Portuguese");
    int numTerritorios = 0;
    Territorio territorio[MAX_TERRITORIOS]; //CRIAR VETOR DE 5 POSIÇÕES PARA TERRITÓRIOS
    printf("===============================\n");
    printf("  BEM-VINDO AO JOGO DE WAR  \n");
    printf("===============================\n\n");

    //CADASTRO DE TERRITÓRIOS
    printf("Cadastro de Territórios:\n");
    do{
        //VERIFICA SE O NÚMERO MÁXIMO DE TERRITÓRIOS FOI ATINGIDO
        if (numTerritorios >= MAX_TERRITORIOS) {
            printf("Número máximo de territórios atingido.\n");
            break;
        }

        //INICIA O CADASTRO DE UM NOVO TERRITÓRIO
        printf("\nCadastro do território %d:\n", numTerritorios + 1);
        printf("-------------------------------\n");

        //CADASTRO DO NOME DO TERRITÓRIO
        printf("Nome do território: ");
        fgets(territorio[numTerritorios].nome, MAX_NOME, stdin); // lê a linha (permite espaços)
        territorio[numTerritorios].nome[strcspn(territorio[numTerritorios].nome, "\n")] = '\0'; // remove o \n final

        //CADASTRO DA COR DO TERRITÓRIO
        printf("Cor do território: ");
        fgets(territorio[numTerritorios].cor, MAX_COR, stdin); // lê a linha (permite espaços)
        territorio[numTerritorios].cor[strcspn(territorio[numTerritorios].cor, "\n")] = '\0'; // remove o \n final

        //CADASTRO DO NÚMERO DE TROPAS NO TERRITÓRIO
        printf("Número de tropas: ");
        if (scanf("%d", &territorio[numTerritorios].tropas) != 1) { // valida leitura do inteiro
            printf("Entrada inválida para tropas.\n");
            return 1; // encerra com erro
        }
        int c; while ((c = getchar()) != '\n' && c != EOF) {} // limpa buffer até fim da linha
        
        numTerritorios++; //INCREMENTA O NÚMERO DE TERRITÓRIOS CADASTRADOS

    } while (numTerritorios < MAX_TERRITORIOS);
    
    //EXIBIÇÃO DOS TERRITÓRIOS CADASTRADOS
    printf("\nTerritórios Cadastrados:\n");
    printf("-------------------------------\n");
    for (int i = 0; i < numTerritorios; i++) { // exibe somente os cadastrados
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorio[i].nome); // evita \n duplicado
        printf("Cor: %s\n", territorio[i].cor); // evita \n duplicado
        printf("Número de Tropas: %d\n", territorio[i].tropas);
        printf("-------------------------------\n");
    }

    return 0; // encerra main com sucesso
}
