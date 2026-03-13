#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Territorio
{
    char nome[30];
    char corEx[10];
    int tropas;
};

//funçao para cadastrar os territorios
void cadastrarTerritorios(struct Territorio *mapa, int qtd)
{
    int i;
    for (i = 0; i < qtd; i++)
    {
        printf("NOME DO PAIS %d:\n", i + 1);
        scanf("%s", mapa[i].nome);

        printf("PREENCHA A COR DO TERRITORIO %d:\n", i + 1);
        scanf("%s", mapa[i].corEx);

        printf("PREENCHA A QUANTIDADE DE tropas %d:\n", i + 1);
        scanf("%d", &mapa[i].tropas);
    }
}

//funçao para exibir os territorios cadastrados
void exibirTerritorio(struct Territorio *mapa, int qtd)
{
    int i;
    printf("---OS DADOS DOS TERRITORIOS---\n\n");
    for (i = 0; i < qtd; i++)
    {
        printf("TERRITORIO  %d\n", i + 1);
        printf("NOME: %s\n", mapa[i].nome);
        printf("COR: %s\n", mapa[i].corEx);
        printf("tropas: %d\n\n", mapa[i].tropas);
    }
}

//funçao para verificar se as missoes foram cumpridas
int verificarMissao(char *missao, struct Territorio *mapa, int qtd_territorios)
{
    int i, j;
    // Eliminar todos os outros exercitos
    if (strcmp(missao, "Conquistar o primeiro territorio") == 0)
    {
        for (i = 0; i < qtd_territorios; i++)
        {
            int contador = 0;
            for (j = 0; j < qtd_territorios; j++)
            {
                if (strcmp(mapa[i].corEx, mapa[j].corEx) == 0)
                {
                    contador++;
                }
            }
            if (contador >= 2)
            {
                return 1;
            }
        }
    }
    return 0;
}

//funçao usada para decidir o vencedor das batalhas
void atacar(struct Territorio *atacante, struct Territorio *defensor)
{
    int dado_atacante = (rand() % 6) + 1;
    int dado_defensor = (rand() % 6) + 1;

    if (dado_atacante > dado_defensor)
    {
        printf("O ATACANTE VENCEU A BATALHA\n");
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas = atacante->tropas / 2;
        strcpy(defensor->corEx, atacante->corEx);
    }
    else
    {
        printf("O ATACANTE PERDEU A BATALHA/EMPATOU\n");
        atacante->tropas = atacante->tropas - 1;
    }
}

//funçao usada para fazer a interaçao entre os territorios
void guerra(struct Territorio *mapa, int qtd_territorios, char *missaoJogador)
{
    int id_atacante, id_defensor;
    int rep;

    do
    {
        printf("DESEJA ENTRAR EM GUERRA?\n");
        printf("SIM = 1 NAO = 0\n");
        scanf("%d", &rep);
        if (rep == 0)
        {
            break;
        }
        printf("QUAL O TERRITORIO VAI ATACAR\n");
        scanf("%d", &id_atacante);
        printf("QUAL TERRITORIO VAI DEFENDER\n");
        scanf("%d", &id_defensor);
        if (id_atacante == id_defensor)
        {
            printf("VOCE NAO PODE ATACAR A SI MESMO\n");
        }
        else if (strcmp(mapa[id_atacante - 1].corEx, mapa[id_defensor - 1].corEx) == 0)
        {
            printf("ATAQUE INVALIDO, OS TERRITORIOS PERTECEM AO MESMO PAIS");
        }
        else
        {
            atacar(&mapa[id_atacante - 1], &mapa[id_defensor - 1]);
            printf("\n--- RESULTADO DA BATALHA ---\n");
            exibirTerritorio(mapa, qtd_territorios);
            int venceu = verificarMissao(missaoJogador, mapa, qtd_territorios);
            if (venceu == 1)
            {
                printf("\n================================================\n");
                printf("VITORIA!!! VOCE COMPLETOU SUA MISSAO SECRETA!\n");
                printf("Objetivo concluido: %s\n", missaoJogador);
                printf("================================================\n");

                break;
            }
        }
    } while (rep == 1);
}

//funçao que sorteia a missao da partida
void atribuirMissao(char *destino, char *missoes[], int totalMissoes)
{
    // int indice_sorteado = rand() % totalMissoes;
    int indice_sorteado = 2; //utilizado apenas para teste
    strcpy(destino, missoes[indice_sorteado]);
}

//funçao para liberar memoria
void liberarMemoria(struct Territorio *mapa, char *missaoJogador)
{
    free(mapa);
    free(missaoJogador);
}


int main()
{
    char *lista_missoes[5] = {
        "...",
        "...",
        "Conquistar o primeiro territorio",
        "...",
        "..."}; //falta fazer as outras missoes
    char *missaoJogador = (char *)malloc(100 * sizeof(char));

    srand(time(NULL)); // aleatoriedade
    int qtd_territorios;

    printf("QUANTOS TERRITORIOS VAI TER ?\n");
    scanf("%d", &qtd_territorios);
    struct Territorio *mapa = (struct Territorio *)calloc(qtd_territorios, sizeof(struct Territorio));

    cadastrarTerritorios(mapa, qtd_territorios);
    exibirTerritorio(mapa, qtd_territorios);
    atribuirMissao(missaoJogador, lista_missoes, 5);
    printf("\n--- SUA MISSAO ---\n");
    printf("%s\n", missaoJogador);
    printf("--------------------------\n\n");
    guerra(mapa, qtd_territorios, missaoJogador);
    printf("\n--- RESULTADO DA BATALHA ---\n");
    exibirTerritorio(mapa, qtd_territorios);
    liberarMemoria(mapa, missaoJogador);

    return 0;
}