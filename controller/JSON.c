#include "JSON.h"
#include <string.h>
#include <stdlib.h>

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