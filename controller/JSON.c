#include "JSON.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

char *concatena(char *data1, char *data2) {
    size_t tamanho_data1 = strlen(data1);
    size_t tamanho_data2 = strlen(data2);
    size_t tamanho_total = tamanho_data1 + tamanho_data2 + 1; // tamanho das duas strings + '\0'

    char *resultado = malloc(tamanho_total);
    if (resultado == NULL) {
        // Tratamento de erro: falha na alocação de memória
        return NULL;
    }

    strcpy(resultado, data1); // Copia a primeira string para o resultado.
    strcat(resultado, data2); // Concatena a segunda string ao resultado.

    return resultado;
}

/**
 * <h3>Descrição:</h3>
 * Função responsavel por trasformar um arquivo Json em uma struct.
 * A função retorna o ponteiro para o primeiro campo da struct para ver a struct inteira é necessário uso de um loop.
 * <h3>Comandos:</h3>
 * <ul>
 *      <li><b>list_campos.tamanho</b> : comando para pegar o tamanho da struct</li>
 *      <li><b>listaCampos.campos[i].key</b> : comando para pegar a chave da struct como exemplo {Nome: pedro} ele retorna o valor "Nome"</li>
 *      <li><b>listaCampos.campos[i].valor</b> : comando para pegar o valor da struct como exemplo {Nome: pedro} ele retorna o valor "pedro"</li>
 * </ul>
 */
ListCampo convertObj(char *obj) {
    ListCampo list_campos;

    list_campos.tamanho = 0;
    list_campos.campos = (Campo *)malloc(list_campos.tamanho*sizeof(Campo));
    if (list_campos.campos == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Tokenização do objeto JSON
    char *ident;
    ident = strtok(obj, ",:{}");

    while (ident != NULL) {
        // Aloca memória dinamicamente para a chave
        char *key = (char *)malloc((strlen(ident) + 1) * sizeof(char));
        if (key == NULL) {
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
        strcpy(key, ident); // Copia a chave para a memória alocada

        // Avança para o próximo token
        ident = strtok(NULL, ",:{}");

        // Aloca memória dinamicamente para o valor
        char *valor = (char *)malloc((strlen(ident) + 1) * sizeof(char));
        if (valor == NULL) {
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
        strcpy(valor, ident); // Copia o valor para a memória alocada

        list_campos.tamanho++;

        list_campos.campos = (Campo *)realloc(list_campos.campos, list_campos.tamanho*sizeof(Campo));
        if (list_campos.campos == NULL) {
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
        list_campos.campos[list_campos.tamanho-1].key = key;
        list_campos.campos[list_campos.tamanho-1].valor = valor;

        // Avança para o próximo token
        ident = strtok(NULL, ",:{}");
    }

    return list_campos; // Retorna o ponteiro para o primeiro campo da lista
}