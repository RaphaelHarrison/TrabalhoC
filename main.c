#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_PESSOAS 100
#define TAM_NOME 100

typedef struct {
    char nome[TAM_NOME];
    int idade;
} Pessoa;

int nome_valido(const char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ' && nome[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

void ler_nome(char *nome) {
    while (1) {
        printf("Digite seu nome: ");
        fgets(nome, TAM_NOME, stdin);
        nome[strcspn(nome, "\n")] = '\0';

        if (nome_valido(nome) && strlen(nome) > 0) {
            break;
        } else {
            printf("Nome inválido! Digite apenas letras e espaços.\n");
        }
    }
}

int ler_idade() {
    char entrada[20];
    int idade;
    while (1) {
        printf("Digite sua idade: ");
        fgets(entrada, sizeof(entrada), stdin);

        int valido = 1;
        for (int i = 0; entrada[i] != '\0' && entrada[i] != '\n'; i++) {
            if (!isdigit(entrada[i])) {
                valido = 0;
                break;
            }
        }

        if (valido) {
            idade = atoi(entrada);
            if (idade > 0) return idade;
        }

        printf("Idade inválida! Digite apenas números inteiros positivos.\n");
    }
}

int carregar_dados(Pessoa pessoas[], int *total) {
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) return 0;

    char linha[150];
    *total = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && *total < MAX_PESSOAS) {
        char *nome = strtok(linha, ";");
        char *idade_str = strtok(NULL, ";\n");

        if (nome && idade_str) {
            strncpy(pessoas[*total].nome, nome, TAM_NOME);
            pessoas[*total].idade = atoi(idade_str);
            (*total)++;
        }
    }

    fclose(arquivo);
    return 1;
}

void salvar_dados(Pessoa pessoas[], int total) {
    FILE *arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar os dados!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s;%d\n", pessoas[i].nome, pessoas[i].idade);
    }

    fclose(arquivo);
}

void exibir_lista(Pessoa pessoas[], int total) {
    if (total == 0) {
        printf("\nNenhuma pessoa cadastrada.\n");
        return;
    }
    printf("\n--- Lista de Pessoas ---\n");
    for (int i = 0; i < total; i++) {
        printf("%d. Nome: %s | Idade: %d anos\n", i + 1, pessoas[i].nome, pessoas[i].idade);
    }
}

void editar_pessoa(Pessoa pessoas[], int total) {
    int indice;
    exibir_lista(pessoas, total);
    printf("Digite o número da pessoa que deseja editar (0 para voltar): ");
    scanf("%d", &indice);
    while (getchar() != '\n');

    if (indice == 0) return;

    if (indice < 1 || indice > total) {
        printf("Índice inválido.\n");
        return;
    }

    printf("Editando pessoa %d:\n", indice);
    ler_nome(pessoas[indice - 1].nome);
    pessoas[indice - 1].idade = ler_idade();
    printf("Pessoa editada com sucesso!\n");
}

void excluir_pessoa(Pessoa pessoas[], int *total) {
    int indice;
    exibir_lista(pessoas, *total);
    printf("Digite o número da pessoa que deseja excluir (0 para voltar): ");
    scanf("%d", &indice);
    while (getchar() != '\n');

    if (indice == 0) return;

    if (indice < 1 || indice > *total) {
        printf("Índice inválido.\n");
        return;
    }

    for (int i = indice - 1; i < *total - 1; i++) {
        pessoas[i] = pessoas[i + 1];
    }

    (*total)--;
    printf("Pessoa excluída com sucesso!\n");
}

void adicionar_pessoa(Pessoa pessoas[], int *total) {
    if (*total >= MAX_PESSOAS) {
        printf("Limite máximo de pessoas atingido.\n");
        return;
    }
    ler_nome(pessoas[*total].nome);
    pessoas[*total].idade = ler_idade();
    (*total)++;
    printf("Pessoa cadastrada com sucesso!\n");
}

int main() {
    Pessoa pessoas[MAX_PESSOAS];
    int total = 0;

    carregar_dados(pessoas, &total);

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Ver lista de pessoas\n");
        printf("2. Adicionar novo cadastro\n");
        printf("3. Editar um cadastro\n");
        printf("4. Excluir um cadastro\n");
        printf("5. Finalizar programa\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                exibir_lista(pessoas, total);
                break;
            case 2:
                adicionar_pessoa(pessoas, &total);
                break;
            case 3:
                editar_pessoa(pessoas, total);
                break;
            case 4:
                excluir_pessoa(pessoas, &total);
                break;
            case 5:
                printf("Salvando dados e encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);

    salvar_dados(pessoas, total);
    return 0;
}
