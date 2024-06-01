#include "ted.h"
#include <string.h>
#include "../controller/bd.h"
#include <time.h>
#include <stdio.h>


/**
 * Pega a informações sobre um usuario com base nos dados da conta
 * @param post é uma string no formato json com a seguinte informação:
 * <ol>
 *      <li>USU_NUM_CONTA: numero da conta</li>
 *      <li>USU_NUM_AGENCIA: numero da agencia</li>
 * </ol>
 * @return Um json com as seguintes informações
 * <ol>
 *      <li>mensagem: mensagem informando se o usuario foi encontrado</li>
 *      <li>USU_ID: id do usuario dono dos dados da conta</li>
 *      <li>USU_NOME: nome do usuario dono dos dados da conta</li>
 *      <li>USU_CPF: CPF do usuario dono dos dados da conta</li>
 * </ol>
 */
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