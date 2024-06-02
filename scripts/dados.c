#include "dados.h"
#include <stdio.h>
#include <string.h>
#include "../controller/bd.h"

/**
 * Pega todos dados do usuario na TAB_USUARIO
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>USU_ID: id do usuario </li>
 * </ul>
 * @return Um json com todos os dados da TAB_USUARIO do usuario como USU_ID fornecido
 */
char * dados(char * post){
    ListCampo post_data = convertObj( post);

    // Seleciona os todos os dados do usuario com base no id
    char * query = "SELECT * FROM TAB_USUARIO WHERE USU_ID = \'";

    query = concatena(query, post_data.campos[0].valor );
    query = concatena(query, "\'");
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);

    if (retorno.tamanho > 0){
        return retornoJson;
    }
    else{
        return "{\"mensagem\":\"erro\"}";
    }
}