// WAR simplificado com missões estratégicas, modularização e uso de ponteiros
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 32
#define MAX_COR 16
#define MAX_MISSOES 5
#define MAX_JOGADORES 4

// Estrutura que representa um território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char dono[MAX_NOME];
    int tropas;
} Territorio;

// Estrutura que representa um jogador
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char *missao; // armazenada dinamicamente
} Jogador;

// Protótipos
void limparBufferEntrada(void);
int lerInteiro(const char *mensagem, int minimo);
int rolarDado(void);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMissao(const char *missao, const Jogador *jogador);
int verificarMissao(const char *missao, const Territorio *mapa, int tamanho, const char *corJogador);
int verificarMissaoBasica(char *missao, Territorio *mapa, int tamanho); // assinatura solicitada
void exibirMapa(const Territorio *mapa, int quantidade);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores);
void cadastrarTerritorio(Territorio *t, int indice, const Jogador *jogadores, int totalJogadores);
Territorio *cadastrarMapa(int *quantidade, const Jogador *jogadores, int totalJogadores);

// Descarta caracteres remanescentes do buffer de entrada
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Lê um inteiro com mensagem e valida mínimo
int lerInteiro(const char *mensagem, int minimo) {
    int valor = 0;
    for (;;) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= minimo) {
            limparBufferEntrada();
            return valor;
        }
        printf("Entrada inválida. Tente novamente.\n");
        limparBufferEntrada();
    }
}

// Simula um dado de seis lados (1 a 6)
int rolarDado(void) {
    return (rand() % 6) + 1;
}

// Atribui missão ao jogador, copiando de vetor de missões
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Exibe missão do jogador (chamada uma vez no início)
void exibirMissao(const char *missao, const Jogador *jogador) {
    printf("\nMissão de %s (%s): %s\n", jogador->nome, jogador->cor, missao);
}

// Extrai tipo numérico da missão a partir do prefixo "[n]"
static int obterTipoMissao(const char *missao) {
    if (missao == NULL || missao[0] != '[' || missao[2] != ']') {
        return -1;
    }
    return missao[1] - '0';
}

// Verifica se a missão do jogador foi cumprida (usa cor para saber posse)
int verificarMissao(const char *missao, const Territorio *mapa, int tamanho, const char *corJogador) {
    int tipo = obterTipoMissao(missao);
    int totalTerritorios = 0;
    int totalTropas = 0;
    int territoriosCom5 = 0;
    int territoriosCom8 = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            totalTerritorios++;
            totalTropas += mapa[i].tropas;
            if (mapa[i].tropas >= 5) {
                territoriosCom5++;
            }
            if (mapa[i].tropas >= 8) {
                territoriosCom8++;
            }
        }
    }

    switch (tipo) {
        case 1: // controlar ao menos 3 territórios
            return totalTerritorios >= 3;
        case 2: // somar 12 ou mais tropas
            return totalTropas >= 12;
        case 3: // ter 2 territórios com 5+ tropas
            return territoriosCom5 >= 2;
        case 4: // possuir 1 território com 8+ tropas
            return territoriosCom8 >= 1;
        case 5: // controlar ao menos 1 território e 15 tropas no total
            return totalTerritorios >= 1 && totalTropas >= 15;
        default:
            return 0;
    }
}

// Assinatura solicitada no enunciado; delega para a versão com cor (quando não há cor, sempre falha)
int verificarMissaoBasica(char *missao, Territorio *mapa, int tamanho) {
    (void)mapa; // evita warning se não usada
    (void)tamanho;
    (void)missao;
    return 0;
}

// Exibe o estado atual de todos os territórios
void exibirMapa(const Territorio *mapa, int quantidade) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] Nome: %s | Dono: %s | Cor: %s | Tropas: %d\n", i + 1,
               mapa[i].nome, mapa[i].dono, mapa[i].cor, mapa[i].tropas);
    }
}

// Resolve um ataque entre territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar um território aliado.\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return;
    }

    printf("\nAtaque de %s (%s) contra %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtq = rolarDado();
    int dadoDef = rolarDado();
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        // atacante vence: transfere cor, dono e metade das tropas para o defensor
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas == 0) {
            tropasTransferidas = 1;
        }
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        strcpy(defensor->cor, atacante->cor);
        strcpy(defensor->dono, atacante->dono);
        printf("Atacante venceu! Território conquistado com %d tropas. Tropas restantes no atacante: %d\n",
               defensor->tropas, atacante->tropas);
    } else {
        atacante->tropas -= 1;
        printf("Defensor segurou. Tropas do atacante: %d\n", atacante->tropas);
    }
}

// Cadastra um território e o associa a um jogador (pela cor)
void cadastrarTerritorio(Territorio *t, int indice, const Jogador *jogadores, int totalJogadores) {
    printf("\nCadastro do território %d\n", indice + 1);
    printf("-----------------------\n");

    printf("Nome do território: ");
    fgets(t->nome, MAX_NOME, stdin);
    t->nome[strcspn(t->nome, "\n")] = '\0';

    printf("Escolha o dono do território:\n");
    for (int i = 0; i < totalJogadores; i++) {
        printf("%d - %s (%s)\n", i + 1, jogadores[i].nome, jogadores[i].cor);
    }

    int dono = 0;
    do {
        dono = lerInteiro("Informe o número do jogador: ", 1);
    } while (dono < 1 || dono > totalJogadores);

    strcpy(t->cor, jogadores[dono - 1].cor);
    strcpy(t->dono, jogadores[dono - 1].nome);
    t->tropas = lerInteiro("Número de tropas (>=1): ", 1);
}

// Aloca e cadastra todos os territórios
Territorio *cadastrarMapa(int *quantidade, const Jogador *jogadores, int totalJogadores) {
    *quantidade = lerInteiro("Quantos territórios deseja cadastrar? ", 1);
    Territorio *mapa = (Territorio *)calloc((size_t)*quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Falha ao alocar memória para os territórios.\n");
        return NULL;
    }

    for (int i = 0; i < *quantidade; i++) {
        cadastrarTerritorio(&mapa[i], i, jogadores, totalJogadores);
    }
    return mapa;
}

// Libera memória alocada dinamicamente
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores) {
    for (int i = 0; i < totalJogadores; i++) {
        free(jogadores[i].missao);
    }
    free(jogadores);
    free(mapa);
}

// Fluxo principal
int main(void) {
    srand((unsigned)time(NULL));
    printf("===============================\n");
    printf("  BEM-VINDO AO JOGO DE WAR  \n");
    printf("===============================\n\n");

    // Missoes pré-definidas
    char *missoes[MAX_MISSOES] = {
        "[1] Controlar pelo menos 3 territórios.",
        "[2] Somar 12 ou mais tropas no total.",
        "[3] Controlar 2 territórios com 5+ tropas.",
        "[4] Manter um território com 8+ tropas.",
        "[5] Controlar 1 território e 15 tropas no total."
    };

    int totalJogadores = lerInteiro("Informe o número de jogadores (2 a 4): ", 2);
    if (totalJogadores > MAX_JOGADORES) {
        totalJogadores = MAX_JOGADORES;
        printf("Limitando a %d jogadores.\n", MAX_JOGADORES);
    }

    Jogador *jogadores = (Jogador *)calloc((size_t)totalJogadores, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("Falha ao alocar memória para os jogadores.\n");
        return 1;
    }

    // Cadastro de jogadores e missões
    for (int i = 0; i < totalJogadores; i++) {
        printf("\nJogador %d\n", i + 1);
        printf("Nome: ");
        fgets(jogadores[i].nome, MAX_NOME, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(jogadores[i].cor, MAX_COR, stdin);
        jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

        size_t tamBuffer = 128;
        jogadores[i].missao = (char *)malloc(tamBuffer);
        if (jogadores[i].missao == NULL) {
            printf("Falha ao alocar memória para missão.\n");
            liberarMemoria(NULL, jogadores, totalJogadores);
            return 1;
        }
        atribuirMissao(jogadores[i].missao, missoes, MAX_MISSOES);
        exibirMissao(jogadores[i].missao, &jogadores[i]);
    }

    int quantidade = 0;
    Territorio *mapa = cadastrarMapa(&quantidade, jogadores, totalJogadores);
    if (mapa == NULL) {
        liberarMemoria(NULL, jogadores, totalJogadores);
        return 1;
    }

    exibirMapa(mapa, quantidade);

    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        printf("\nEscolha o território atacante (1-%d): ", quantidade);
        int idxAtq = 0, idxDef = 0;
        if (scanf("%d", &idxAtq) != 1 || idxAtq < 1 || idxAtq > quantidade) {
            printf("Índice inválido.\n");
            limparBufferEntrada();
            continue;
        }
        printf("Escolha o território defensor (1-%d e diferente do atacante): ", quantidade);
        if (scanf("%d", &idxDef) != 1 || idxDef < 1 || idxDef > quantidade || idxDef == idxAtq) {
            printf("Índice inválido.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();

        atacar(&mapa[idxAtq - 1], &mapa[idxDef - 1]);
        exibirMapa(mapa, quantidade);

        // Verifica missões ao final do turno
        for (int i = 0; i < totalJogadores; i++) {
            if (verificarMissao(jogadores[i].missao, mapa, quantidade, jogadores[i].cor)) {
                printf("\nMissão de %s cumprida! Jogador vencedor.\n", jogadores[i].nome);
                liberarMemoria(mapa, jogadores, totalJogadores);
                return 0;
            }
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        continuar = (char)getchar();
        limparBufferEntrada();
    }

    liberarMemoria(mapa, jogadores, totalJogadores);
    printf("\nFim da simulação. Memória liberada.\n");
    return 0;
}
