#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala
{
    char nome[30];
    char pista[30];
    struct Sala *esquerda;
    struct Sala *direita;
    struct Sala *pai;
} Sala;

typedef struct NoPista
{
    char conteudo[30];
    struct NoPista *esq;
    struct NoPista *dir;
} NoPista;

Sala *criarSala(char *nome, char *pista)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala != NULL)
    {
        strcpy(novaSala->nome, nome);
        strcpy(novaSala->pista, pista);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
        novaSala->pai = NULL;
    }
    return novaSala;
}

NoPista *inserirPista(NoPista *raiz, char *novaPista)
{
    if (raiz == NULL)
    {
        NoPista *novo = (NoPista *)malloc(sizeof(NoPista));
        strcpy(novo->conteudo, novaPista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(novaPista, raiz->conteudo) < 0)
    {
        raiz->esq = inserirPista(raiz->esq, novaPista);
    }
    else if (strcmp(novaPista, raiz->conteudo) > 0)
    {
        raiz->dir = inserirPista(raiz->dir, novaPista);
    }
    return raiz;
}

void exibirPista(NoPista *raiz)
{
    if (raiz != NULL)
    {
        exibirPista(raiz->esq);
        printf("[%s]\n", raiz->conteudo);
        exibirPista(raiz->dir);
    }
}

void explorarSalas(Sala *atual)
{
    char escolha;
    int jogando = 1;
    NoPista *minhasPistas = NULL;

    while (atual != NULL && jogando)
    {
        printf("\nVoce esta na Sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0)
        {
            printf("VOCE ENCONTROU UMA PISTA: [%s]\n", atual->pista);
            minhasPistas = inserirPista(minhasPistas, atual->pista);
            strcpy(atual->pista, "");
        }

        if (atual->esquerda == NULL && atual->direita == NULL)
        {
            printf("voce chegou em uma sala sem saidas... \n");
        }

        if (atual->esquerda != NULL)
            printf(" [e] seguir pela esquerda\n");
        if (atual->direita != NULL)
            printf(" [d] seguir pela direita\n");
        if (atual->pai != NULL)
            printf(" [s] voltar para a sala anterior \n");
        printf(" [x] fechar o jogo\n");
        scanf(" %c", &escolha);

        switch (escolha)
        {
        case 'e':
        case 'E':
            if (atual->esquerda != NULL)
            {
                atual = atual->esquerda;
            }
            else
            {
                printf("Caminho a esquerda esta bloqueado\n");
            }
            break;
        case 'd':
        case 'D':
            if (atual->direita != NULL)
            {
                atual = atual->direita;
            }
            else
            {
                printf("Caminho a direita esta bloqueado\n");
            }
            break;
        case 's':
        case 'S':
            if (atual->pai != NULL)
            {
                atual = atual->pai;
            }
            else
            {
                printf("voce ja esta na entrada da mansao\n");
            }
            break;
        case 'x':
        case 'X':
            printf("jogo encerrado\n");
            jogando = 0;
            break;

        default:
            printf("--- ERRO DE DIGITACAO ---\n\n");
            break;
        }
    }
    printf("---JOGO FINALIZADO---\n\n");
    if (minhasPistas == NULL)
    {
        printf("Nenhuma pista encontrada\n");
    }
    else
    {
        printf("Pistas encontradas:\n");
        exibirPista(minhasPistas);
    }
}

int main()
{
    // criacao das salas da mansao
    Sala *hall = criarSala("hall de entrada", "carpete rasgado");    // raiz da arvore o hall
    Sala *salaEstar = criarSala("sala de estar", "abajur quebrado"); // esta a esquerda do hall
    Sala *cozinha = criarSala("cozinha", "faca com sangue");         // esta a direita do hall
    Sala *biblioteca = criarSala("biblioteca", "livros no chao");    // esta a esquerda da sala de estar
    Sala *jardim = criarSala("jardim", "flores amassadas");          // esta a esquerda da cozinha
    Sala *sotao = criarSala("sotao abandonado", "porta joias");      // esta a direita da cozinha

    // montando conexoes
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    cozinha->esquerda = jardim;
    cozinha->direita = sotao;

    salaEstar->pai = hall;
    cozinha->pai = hall;
    biblioteca->pai = salaEstar;
    jardim->pai = cozinha;
    sotao->pai = cozinha;

    explorarSalas(hall);

    return 0;
}