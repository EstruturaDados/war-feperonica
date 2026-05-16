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
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
void liberarMemoria(Territorio* mapa, char* missaoJogador);

// Funções de interface com o usuário:
void exibirMapa(Territorio* mapa, int qtd);
void exibirMissao(const char* missao); // const simula a "passagem por valor" (somente leitura)

// Funções de lógica principal do jogo:
void cadastrarTerritorios(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

// Função utilitária:
void limparBuffer();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Garante a aleatoriedade dos dados de ataque a cada execução
    srand(time(NULL));

    char* missoes[5] = {
        "Eliminar o exercito Azul",
        "Eliminar o exercito Vermelho",
        "Ter um territorio com 10 ou mais tropas",
        "Dominar todos os territorios do mapa",
        "Todos os territorios com menos de 5 tropas" // Estratégia de enfraquecimento global
    };

    int qtdTerritorios;

    printf("=== WAR: NIVEL MESTRE ===\n\n");
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

    // Alocação dinâmica para a string da missão do jogador
    char* missaoJogador = (char*)malloc(100 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para a missao!\n");
        free(mapa);
        return 1;
    }

    // Modularização: Chamada da função de cadastro
    cadastrarTerritorios(mapa, qtdTerritorios);

    // Sorteia e exibe a missão antes do jogo começar
    atribuirMissao(missaoJogador, missoes, 5);
    exibirMissao(missaoJogador);

    int opcao, origem, destino;
    int jogoAtivo = 1;

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
                
                // Verificação Silenciosa da Missão (ocorre após o ataque)
                if (verificarMissao(missaoJogador, mapa, qtdTerritorios) == 1) {
                    printf("\n=====================================================\n");
                    printf("🎉 PARABENS! VOCE CUMPRIU SUA MISSAO E VENCEU O JOGO! 🎉\n");
                    printf("Sua missao era: %s\n", missaoJogador);
                    printf("=====================================================\n");
                    jogoAtivo = 0; // Encerra o laço
                }

            } 
            else {
                printf("\n[!] IDs de territorios invalidos.\n");
            }
        }   else if (opcao == 0) {
            jogoAtivo = 0;
        }

    } while (jogoAtivo);

    // Gerenciamento de memória: Liberação ao final do programa
    liberarMemoria(mapa, missaoJogador);
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

// Sorteia uma missão do array e copia para a memória alocada do jogador (Passagem por Referência)
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// Exibe a missão na tela. O 'const' garante que a string não será alterada (Simula Passagem por Valor)
void exibirMissao(const char* missao) {
    printf("\n========================================\n");
    printf("🎯 ATENCAO COMANDANTE! SUA MISSAO SECRETA:\n");
    printf(">> %s <<\n", missao);
    printf("Mantenha o foco. O sistema verificara seu progresso automaticamente.\n");
    printf("========================================\n");
}

// Exibe o estado atual do mapa
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

// Avalia a missão do jogador usando strcmp para identificar a string sorteada
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strcmp(missao, "Eliminar o exercito Azul") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) return 0; // Ainda existe Azul
        }
        return 1; // Azul foi eliminado
    }
    
    if (strcmp(missao, "Eliminar o exercito Vermelho") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
        }
        return 1;
    }
    
    if (strcmp(missao, "Ter um territorio com 10 ou mais tropas") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 10) return 1;
        }
        return 0;
    }
    
    if (strcmp(missao, "Dominar todos os territorios do mapa") == 0) {
        char corBase[10];
        strcpy(corBase, mapa[0].cor); // Pega a cor do primeiro território
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corBase) != 0) return 0; // Achou uma cor diferente
        }
        return 1; // Tudo da mesma cor
    }
    
    if (strcmp(missao, "Todos os territorios com menos de 5 tropas") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 5) return 0;
        }
        return 1;
    }

    return 0;
}

// Função dedicada a liberar a memória previamente alocada dinamicamente
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}