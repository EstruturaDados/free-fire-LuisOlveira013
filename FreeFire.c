#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define TAM_STRING 50

// -----------------------------------------------------------
// ESTRUTURA DO COMPONENTE
// -----------------------------------------------------------
typedef struct {
    char nome[TAM_STRING];
    char tipo[TAM_STRING];
    int prioridade;
} Componente;

// -----------------------------------------------------------
// EXIBIR COMPONENTES
// -----------------------------------------------------------
void mostrarComponentes(Componente v[], int n) {
    printf("\n--- Lista de Componentes ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Tipo: %s | Prioridade: %d\n",
               i,
               v[i].nome,
               v[i].tipo,
               v[i].prioridade);
    }
    printf("----------------------------\n");
}

// -----------------------------------------------------------
// SELECTION SORT POR PRIORIDADE (CRESCENTE)
// -----------------------------------------------------------
void selectionSortPrioridade(Componente v[], int n) {
    int min;

    for (int i = 0; i < n - 1; i++) {
        min = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j].prioridade < v[min].prioridade) {
                min = j;
            }
        }
        if (min != i) {
            Componente temp = v[i];
            v[i] = v[min];
            v[min] = temp;
        }
    }
}

// -----------------------------------------------------------
// INSERTION SORT POR TIPO (ALFABÉTICO)
// -----------------------------------------------------------
void insertionSortTipo(Componente v[], int n) {
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0 && strcmp(v[j].tipo, chave.tipo) > 0) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// -----------------------------------------------------------
// BUSCA BINÁRIA POR NOME
// (Requer vetor previamente ordenado pelo nome)
// -----------------------------------------------------------
int buscaBinariaPorNome(Componente v[], int n, char nome[]) {
    int inicio = 0, fim = n - 1, meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;

        int cmp = strcmp(nome, v[meio].nome);

        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else inicio = meio + 1;
    }

    return -1;
}

// -----------------------------------------------------------
// FUNÇÃO DE MEDIR TEMPO
// Usa ponteiro para função
// -----------------------------------------------------------
void medirTempo(void (*algoritmo)(Componente[], int), 
                Componente v[], int n, 
                const char *descricao) 
{
    clock_t inicio = clock();
    
    algoritmo(v, n);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nTempo do algoritmo %s: %.6f segundos\n", descricao, tempo);
}

// -----------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------
int main() {
    Componente vet[MAX];
    int n = 0;

    printf("Quantos componentes deseja cadastrar? ");
    scanf("%d", &n);
    getchar(); // limpar o '\n' do buffer

    for (int i = 0; i < n; i++) {
        printf("\n--- Componente %d ---\n", i + 1);

        printf("Nome: ");
        fgets(vet[i].nome, TAM_STRING, stdin);
        vet[i].nome[strcspn(vet[i].nome, "\n")] = '\0';

        printf("Tipo: ");
        fgets(vet[i].tipo, TAM_STRING, stdin);
        vet[i].tipo[strcspn(vet[i].tipo, "\n")] = '\0';

        printf("Prioridade: ");
        scanf("%d", &vet[i].prioridade);
        getchar(); // limpar enter
    }

    mostrarComponentes(vet, n);

    // ---------------- APLICAÇÃO DOS ALGORITMOS ----------------

    medirTempo(selectionSortPrioridade, vet, n, "Selection Sort por Prioridade");
    mostrarComponentes(vet, n);

    medirTempo(insertionSortTipo, vet, n, "Insertion Sort por Tipo");
    mostrarComponentes(vet, n);

    // ---------------- BUSCA BINÁRIA ----------------
    char nomeBusca[TAM_STRING];
    printf("\nDigite o nome para buscar: ");
    fgets(nomeBusca, TAM_STRING, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // É necessário ordenar por nome para busca binária
    // Aqui aproveitamos insertion sort (modificado se quiser)
    inserirOrdenado: {
        for (int i = 1; i < n; i++) {
            Componente chave = vet[i];
            int j = i - 1;
            while (j >= 0 && strcmp(vet[j].nome, chave.nome) > 0) {
                vet[j + 1] = vet[j];
                j--;
            }
            vet[j + 1] = chave;
        }
    }

    int pos = buscaBinariaPorNome(vet, n, nomeBusca);

    if (pos != -1)
        printf("\nComponente encontrado na posição %d!\n", pos);
    else
        printf("\nComponente NÃO encontrado.\n");

    mostrarComponentes(vet, n);

    return 0;
}
