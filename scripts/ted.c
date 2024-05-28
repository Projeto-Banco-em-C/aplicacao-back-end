#include "ted.h"
#include <string.h>
#include "../controller/bd.h"
#include <time.h>
#include <stdio.h>

char * consulta_info_conta(char * post){

    ListCampo post_data = convertObj(post);

    char * query = "SELECT USU_ID, USU_NOME, USU_CPF FROM TAB_USUARIO WHERE USU_NUM_CONTA = \'";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, "\' AND USU_NUM_AGENCIA = \'");
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, "\'");

    Linhas retorno = bd(query);

    if (retorno.tamanho > 0){
        char * json = "{\"mensagem\":\"ok\",\"USU_ID\":\"";
        json = concatena(json,retorno.list_campos[0].campos[0].valor);
        json = concatena(json,"\",\"USU_NOME\":\"");
        json = concatena(json,retorno.list_campos[0].campos[1].valor);
        json = concatena(json,"\",\"USU_CPF\":\"");
        json = concatena(json,retorno.list_campos[0].campos[2].valor);
        json = concatena(json,"\"}");
        return json;
    }else{
        return "{\"mensagem\":\"conta inexistente\"}";
    }


}