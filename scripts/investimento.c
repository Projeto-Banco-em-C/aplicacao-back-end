#include "investimento.h"
#include "../controller/bd.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "transferencia.h"

char * investimento(char * post) {
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);
    char *query = "INSERT INTO TAB_INVESTIMENTO( ";
    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, post_data.campos[i].key);
        query = concatena(query, ",");
    }
    query = concatena(query, "INV_DATA_INICIO");
    query = concatena(query, ") VALUES (");

    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, "\'");
        query = concatena(query, post_data.campos[i].valor);
        query = concatena(query, "\'");
        query = concatena(query, ",");
    }
    query = concatena(query, "\'");
    query = concatena(query, data);
    query = concatena(query, "\'");
    query = concatena(query, ")");
    Linhas retorno = bd(query);
    char * json = "{USU_ID_ORIGEM:";
    json = concatena(json, post_data.campos[0].valor);
    json = concatena(json, ",USU_ID_DESTINO:1,VALOR:");
    json = concatena(json,post_data.campos[1].valor);
    json = concatena(json,",TIPO:INVESTIMENTO}");
    char * RetornoJSON = transferir(json);
    return "{\"mensagem\":\"aplicado\"}";
}
char * mostrarInvestimento(char * post)
{
    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);
    return retornoJson;
}
