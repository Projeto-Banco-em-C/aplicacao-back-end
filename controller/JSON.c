#include "JSON.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

/**
 * Função que remove um caracter especifico de uma string qualquer
 * @param str String que
 * @param charToRemove O caracter que será removido
 * @return Retorna a string sem o caracter
 */
void removeChar(char *str, char charToRemove) {
    char *src = str; // Ponteiro para percorrer a string
    char *dst = str; // Ponteiro para onde os caracteres devem ser movidos

    while (*src) {
        if (*src != charToRemove) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0'; // Termina a string após a remoção
}

/**
 * Função que concatena duas strings
 * @param data1 String será concatenada a esquerda
 * @param data2 String será concatenada a direita
 * @return Retorna a string resultante da concatenação
 */
char * concatena(char *str1, char *str2) {
    size_t tamanho_data1 = strlen(str1);
    if(str2 != NULL) {
        size_t tamanho_data2 = strlen(str2);
        size_t tamanho_total = tamanho_data1 + tamanho_data2 + 1; // tamanho das duas strings + '\0'
    
        char *resultado = malloc(tamanho_total);
        if (resultado == NULL) {
            // Falha na alocação de memória
            return NULL;
        }
    
        strcpy(resultado, str1); // Copia a primeira string para o resultado.
        strcat(resultado, str2); // Concatena a segunda string ao resultado.
    
        return resultado;
    }else{
        return str1;
    }
}

/**
 * Função que gera um JSON apartir de uma variavel do tipo Linhas
 * @param linhas_json Variavel do tipo linha que será convertida para JSON
 * @return Retorna a string com formato JSON
 */
char * convertJSON(Linhas linhas_json){
    char * json;
    if(linhas_json.tamanho == 1){
        json = "";
    }else{
        json = "[";
    }
    for (int i = 0; i < linhas_json.tamanho; i++) {
        json = concatena(json,"{\"");
        for (int j = 0; j < linhas_json.list_campos[i].tamanho; j++) {
            json = concatena(json,linhas_json.list_campos[i].campos[j].key);
            json = concatena(json,"\" : \"");
            json = concatena(json,linhas_json.list_campos[i].campos[j].valor);
            json = concatena(json,"\"");
            if(j < linhas_json.list_campos[i].tamanho-1){
                json = concatena(json,",\"");
            }
        }
        json = concatena(json,"}");
        if(i < linhas_json.tamanho-1){
            json = concatena(json,",");
        }
    }
    if(linhas_json.tamanho != 1){
        json = concatena(json,"]");
    }

    return json;
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
        char * valor;
        if(ident == NULL){
             valor = '\0';
        }else {
            valor = (char *) malloc((strlen(ident) + 1) * sizeof(char));
            if (valor == NULL) {
                printf("Erro ao alocar memória.\n");
                exit(1);
            }
            strcpy(valor, ident); // Copia o valor para a memória alocada
        }

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
