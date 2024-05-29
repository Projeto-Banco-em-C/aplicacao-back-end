#include "investimento.h"
#include "../controller/bd.h"
#include <stdio.h>

char * investimento(char * post) {
    ListCampo post_data = convertObj(post);

    //INSERIR DADOS NO BANCO
    char *query = "INSERT INTO TAB_INVESTIMENTO( ";
    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, post_data.campos[i].key);
        if (i != post_data.tamanho - 1) {
            query = concatena(query, ",");
        }
    }
    query = concatena(query, ") VALUES (");

    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, "\'");
        query = concatena(query, post_data.campos[i].valor);
        query = concatena(query, "\'");
        if (i != post_data.tamanho - 1) {
            query = concatena(query, ",");
        }
    }
    query = concatena(query, ")");
    Linhas retorno = bd(query);
    return "{\"mensagem\":\"aplicado\"}";
}