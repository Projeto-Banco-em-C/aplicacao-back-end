#include "dados.h"
#include <stdio.h>
#include <string.h>
#include "../controller/bd.h"

char * dados(char * post){
    ListCampo post_data = convertObj( post);

    char * query = "SELECT * FROM TAB_USUARIO WHERE USU_ID = \'";

    query = concatena(query, post_data.campos[0].valor );
    query = concatena(query, "\'");
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);

    if (retorno.tamanho > 0){
        //return "{\"mensagem\":\"ok\"}";
        return retornoJson;
    }
    else{
        return "{\"mensagem\":\"erro\"}";
    }
}