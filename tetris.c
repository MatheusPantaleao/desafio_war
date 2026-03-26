#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5
#define MAX_RESERVA 3

typedef struct
{
    char nome;
    int id;
} Peca;

typedef struct
{
    Peca itens[MAX]; // array que armazena as peças
    int frente;      // indice do primeiro elemento
    int tras;        // indice de onde sera inserido a proxima peça
    int total;       // contador de quantas peças na fila
} FilaPecas;

typedef struct No
{
    Peca peca;
    struct No *proximo;
} No;

typedef struct
{
    Peca itens[MAX_RESERVA];
    int topo;
} PilhaReserva;

void inicializarPilha(PilhaReserva *p)
{
    p->topo = -1;
}

int push(PilhaReserva *p, Peca nova)
{
    if (p->topo < MAX_RESERVA - 1)
    {
        p->topo++;
        p->itens[p->topo] = nova;
        return 1;
    }
    return 0;
}

Peca pop(PilhaReserva *p)
{
    if (p->topo >= 0)
    {
        return p->itens[p->topo--];
    }
    Peca vazia = {'?', -1};
    return vazia;
}

void empilharHistorico(No **topo, Peca p)
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
    {
        printf("-- ERRO FALHA DE MEMORIA --\n");
        return;
    }
    novo->peca = p;
    novo->proximo = *topo; // esse proximo se refere ao antigo do 'novo'
    *topo = novo;          // o novo no vira o topo
}

void limparHistorico(No **topo)
{
    No *atual = *topo;
    No *proximo;
    while (atual != NULL)
    {
        proximo = atual->proximo;
        printf("liberando memoria da peca ID %d\n", atual->peca.id);
        free(atual);
        atual = proximo;
    }
    *topo = NULL;
}

void exibirReserva(PilhaReserva p)
{
    printf("-- Pecas reservadas (PILHA) --\n");

    if (p.topo == -1)
    {
        printf("A reserva esta vazia\n");
        return;
    }

    for (int i = p.topo; i >= 0; i--)
    {
        printf("[%c %d] ", p.itens[i].nome, p.itens[i].id);
    }
    printf("\n");
}

void inicializarFila(FilaPecas *f)
{
    f->frente = 0;
    f->tras = 0;
    f->total = 0;
}

Peca gerarPeca(int novoId)
{
    char tipos[] = {'I', 'Z', 'T', 'L'};
    Peca p;
    p.id = novoId;
    p.nome = tipos[rand() % 4]; // randomiza a peca
    return p;
}

int enfileirar(FilaPecas *f, int novoId)
{
    if (f->total < MAX)
    {
        Peca nova = gerarPeca(novoId); // chamando a funcao para gerar a peça
        f->itens[f->tras] = nova;      // coloca a peça na posiçao tras
        f->tras = (f->tras + 1) % MAX; // deixamos a fila como "circular"
        f->total++;
        return 1; // sucesso
    }
    return 0; // fila cheia
}

void exibirFila(FilaPecas f)
{
    printf("-- Fila de pecas --\n");
    int i, aux;
    for (i = 0; i < f.total; i++)
    {
        aux = (f.frente + i) % MAX; // calcula a posiçao correta considerando a circularidade
        printf("[%c %d] ", f.itens[aux].nome, f.itens[aux].id);
    }
    printf("\n");
}

void exibirHistorico(No *topo)
{
    printf("\nHISTORICO DE JOGADAS (Do mais recente para o antigo):\n");

    if (topo == NULL)
    {
        printf("Nenhuma peca jogada ainda.\n");
        return;
    }

    No *atual = topo; // Criamos um ponteiro auxiliar para não perder o topo original

    while (atual != NULL)
    {
        // Imprime a peça do nó atual
        printf("[%c %d] ", atual->peca.nome, atual->peca.id);

        // O "pulo do gato": move o ponteiro para o próximo nó da corrente
        atual = atual->proximo;

        if (atual != NULL)
            printf(" -> ");
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));

    FilaPecas fila;
    PilhaReserva reserva;
    inicializarFila(&fila);
    inicializarPilha(&reserva);
    No *historico = NULL; // ponteiro para o topo da pilha

    int proximoId = 0;
    int opcao;

    printf(" -- gerando fila inicial -- \n\n");
    for (int i = 0; i < MAX; i++)
    {
        enfileirar(&fila, proximoId++);
    }

    do
    {
        exibirFila(fila);
        printf("\n");
        exibirReserva(reserva);
        exibirHistorico(historico);
        printf("\n escolha uma opcao:\n");
        printf("1 - jogar pecas (dequeue)\n");
        printf("2 - inserir nova peca (enqueue)\n");
        printf("3 - Reservar peca (Fila -> Reserva)\n");
        printf("4 - Jogar peca da reserva\n");
        printf("0 - sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            if (fila.total > 0)
            {
                Peca p = fila.itens[fila.frente];
                fila.frente = (fila.frente + 1) % MAX;
                fila.total--;
                printf("\n --- PECA JOGADA : %c %d ---\n", p.nome, p.id);
                empilharHistorico(&historico, p);
                printf("\n--- VOCE JOGOU: [%c %d] ---\n", p.nome, p.id);
            }
            else
            {
                printf("\n --- ERRO, nao a pecas para jogar ---\n");
            }
            break;
        case 2:
            if (fila.total < MAX)
            {
                Peca nova = gerarPeca(proximoId++);
                fila.itens[fila.tras] = nova;
                fila.tras = (fila.tras + 1) % MAX;
                fila.total++;
                printf("\n --- PECA ADICIONADA: [%c %d] --- \n", nova.nome, nova.id);
            }
            else
            {
                printf("\n--- ERRO: A fila esta cheia, Jogue uma peca primeiro --- \n");
            }
            break;
        case 3:
            if (fila.total > 0)
            {
                if (reserva.topo < MAX_RESERVA - 1)
                {
                    Peca p = fila.itens[fila.frente];
                    fila.frente = (fila.frente + 1) % MAX;
                    fila.total--;
                    push(&reserva, p);
                    printf("\n--- PECA MOVIDA PARA RESERVA ---\n");
                }
                else
                {
                    printf("\n--- ERRO: RESERVA CHEIA ---\n");
                }
            }
            else
            {
                printf("\n--- ERRO: FILA VAZIA ---\n");
            }
            break;
        case 4:
            if (reserva.topo >= 0)
            {
                Peca p = pop(&reserva);
                empilharHistorico(&historico, p);
                printf("\n--- VOCE JOGOU DA RESERVA [%c %d] ---\n", p.nome, p.id);
            }
            else
            {
                printf("\n--- ERRO NAO HA PECAS NA RESERVA ---\n");
            }
            break;
        case 0:
            printf("Saindo do tetris stacke\n");
            break;
        default:
            printf("opcao invalida\n");
        }
    } while (opcao != 0);
    printf("\n---Encerrando e limpando memoria---\n");
    limparHistorico(&historico);
    return 0;
}