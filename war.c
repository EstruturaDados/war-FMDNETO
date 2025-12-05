// Jogo de batalha entre territórios com alocação dinâmica e ponteiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 32
#define MAX_COR 16

// Estrutura que representa um territorio no jogo
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Descarta caracteres remanescentes do buffer de entrada
static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Lê um inteiro com mensagem e valida minimo
static int ler_inteiro(const char *mensagem, int minimo) {
    int valor = 0;
    for (;;) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= minimo) {
            limpar_buffer();
            return valor;
        }
        printf("Entrada invalida. Tente novamente.\n");
        limpar_buffer();
    }
}

// Cadastra um territorio individual usando ponteiro
static void cadastrar_territorio(Territorio *t, int indice) {
    printf("\nCadastro do territorio %d\n", indice + 1);
    printf("-----------------------\n");

    printf("Nome do territorio: ");
    fgets(t->nome, MAX_NOME, stdin);
    t->nome[strcspn(t->nome, "\n")] = '\0';

    printf("Cor do exercito: ");
    fgets(t->cor, MAX_COR, stdin);
    t->cor[strcspn(t->cor, "\n")] = '\0';

    t->tropas = ler_inteiro("Numero de tropas (>=1): ", 1);
}

// Aloca dinamicamente o vetor de territorios e preenche via cadastro
static Territorio *cadastrar_mapa(int *quantidade) {
    *quantidade = ler_inteiro("Quantos territorios deseja cadastrar? ", 1);
    Territorio *mapa = (Territorio *)calloc((size_t)*quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Falha ao alocar memoria para os territorios.\n");
        return NULL;
    }

    for (int i = 0; i < *quantidade; i++) {
        cadastrar_territorio(&mapa[i], i);
    }
    return mapa;
}

// Exibe o estado atual de todos os territorios
static void exibir_territorios(const Territorio *mapa, int quantidade) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula um dado de seis lados (1 a 6)
static int rolar_dado(void) {
    return (rand() % 6) + 1; // valor entre 1 e 6
}

// Resolve um ataque entre territorios usando ponteiros para atualizar dados
static void atacar(Territorio *atacante, Territorio *defensor) {
    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return;
    }
    printf("\nAtaque de %s (%s) contra %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dado_atq = rolar_dado();
    int dado_def = rolar_dado();
    printf("Dado atacante: %d | Dado defensor: %d\n", dado_atq, dado_def);

    if (dado_atq > dado_def) {
        defensor->tropas -= 1;
        printf("Atacante venceu a rodada. Tropas do defensor: %d\n", defensor->tropas);
        if (defensor->tropas <= 0) {
            defensor->tropas = atacante->tropas - 1; // move tropas deixando 1
            atacante->tropas = 1;
            strcpy(defensor->cor, atacante->cor);
            printf("Territorio conquistado! Novo dono: %s. Tropas movidas: %d\n", defensor->cor, defensor->tropas);
        }
    } else {
        atacante->tropas -= 1;
        printf("Defensor segurou. Tropas do atacante: %d\n", atacante->tropas);
    }
}

// Libera memoria alocada para o vetor de territorios
static void liberar_territorios(Territorio *mapa) {
    free(mapa);
}

// Fluxo principal: cadastra, simula ataques e libera memoria
int main(void) {
    srand((unsigned)time(NULL));
    printf("===============================\n");
    printf("  BEM-VINDO AO JOGO DE WAR  \n");
    printf("===============================\n\n");

    int quantidade = 0;
    Territorio *mapa = cadastrar_mapa(&quantidade);
    if (mapa == NULL) {
        return 1;
    }

    exibir_territorios(mapa, quantidade);

    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        printf("\nEscolha o territorio atacante (1-%d): ", quantidade);
        int idx_atq = 0, idx_def = 0;
        if (scanf("%d", &idx_atq) != 1 || idx_atq < 1 || idx_atq > quantidade) {
            printf("Indice invalido.\n");
            limpar_buffer();
            continue;
        }
        printf("Escolha o territorio defensor (1-%d e diferente do atacante): ", quantidade);
        if (scanf("%d", &idx_def) != 1 || idx_def < 1 || idx_def > quantidade || idx_def == idx_atq) {
            printf("Indice invalido.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        atacar(&mapa[idx_atq - 1], &mapa[idx_def - 1]);
        exibir_territorios(mapa, quantidade);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        continuar = (char)getchar();
        limpar_buffer();
    }

    liberar_territorios(mapa);
    printf("\nFim da simulacao. Memoria liberada.\n");
    return 0;
}
