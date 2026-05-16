// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[50];
    char cor[20];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
void limparBuffer();
void cadastrarTerritorios(Territorio* mapa, int qtd);
void exibirMapa(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Garante a aleatoriedade dos dados de ataque a cada execução
    srand(time(NULL));

    int qtdTerritorios;

    printf("=== WAR: NIVEL AVENTUREIRO ===\n\n");
    // Alocação de memória guiada pelo usuário
    printf("Informe a quantidade total de territorios no mapa: ");
    scanf("%d", &qtdTerritorios);
    limparBuffer();

    // Alocação dinâmica de memória para o vetor de territórios
    Territorio* mapa = (Territorio*)calloc(qtdTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    // Modularização: Chamada da função de cadastro
    cadastrarTerritorios(mapa, qtdTerritorios);

    int opcao, origem, destino;

    // Laço interativo de jogo
    do {
        // Modularização: Chamada da exibição pós-ataque
        exibirMapa(mapa, qtdTerritorios);

        printf("\nO que deseja fazer?\n");
        printf("1 - Atacar | 0 - Sair\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            printf("\n--- FASE DE ATAQUE ---\n");
            printf("ID do seu territorio (Atacante): ");
            scanf("%d", &origem);
            printf("ID do territorio inimigo (Defensor): ");
            scanf("%d", &destino);
            limparBuffer();

            // Validação simples para evitar acesso a índices inválidos de memória
            if (origem >= 1 && origem <= qtdTerritorios && destino >= 1 && destino <= qtdTerritorios) {
                // Passagem por referência para modificar os dados diretamente na memória alocada
                atacar(&mapa[origem - 1], &mapa[destino - 1]);
            } else {
                printf("\n[!] IDs de territorios invalidos.\n");
            }
        }
    } while (opcao != 0);

    // Gerenciamento de memória: Liberação ao final do programa
    liberarMemoria(mapa);
    printf("\nMemoria liberada. Jogo encerrado!\n");

    return 0;
}

// --- Implementação das Funções ---

// Função utilitária para limpar o buffer de entrada do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função responsável por preencher os dados iniciais do mapa alocado dinamicamente
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    printf("\n--- CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritorio [%d]:\n", i + 1);
        
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0; // Remove o \n final

        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("[%d] Territorio: %-15s | Cor: %-10s | Tropas: %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}
// Função central da lógica de combate, manipulando diretamente os ponteiros das structs
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validação: Exige no mínimo 2 tropas para atacar e dividir
    if (atacante->tropas < 2) {
        printf("\n[!] %s nao tem tropas suficientes para atacar (minimo 2).\n", atacante->nome);
        return;
    }
    
    // Validação: Impede o jogador de atacar a própria cor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[!] Fogo amigo! Voce nao pode atacar %s, pois tambem pertence a cor %s.\n", defensor->nome, atacante->cor);
        return;
    }
// Simulação com números aleatórios (1 a 6)
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\n⚔️ RESULTADO DO COMBATE: %s (%d) vs %s (%d)\n", atacante->nome, dadoAtk, defensor->nome, dadoDef);

    // Avaliação de vitória (atacante vence se tirar número maior no dado)
    if (dadoAtk > dadoDef) {
        printf("💥 Vitoria do Atacante!\n");
        
        // Atualização de campos: Muda a cor do defensor para a cor do atacante
        strcpy(defensor->cor, atacante->cor);
        
        // Atualização de campos: Transfere metade das tropas do atacante
        int metadeTropas = atacante->tropas / 2;
        defensor->tropas = metadeTropas;
        atacante->tropas -= metadeTropas;
        
        printf("🏆 TERRITORIO CONQUISTADO! %s foi dominado.\n", defensor->nome);
        printf("➡️  %d tropas de %s ocuparam o novo territorio.\n", metadeTropas, atacante->nome);
        
    } else {
        // Atualização de campos: Se o atacante perder ou empatar, perde uma tropa
        printf("🛡️ A defesa resistiu ao ataque!\n");
        atacante->tropas--;
        printf("💀 %s perdeu 1 tropa.\n", atacante->nome);
    }
}

// Função dedicada a liberar a memória previamente alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}