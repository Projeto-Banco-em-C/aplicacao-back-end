#include "extrato.h"
#include "../controller/bd.h"
#include <string.h>

/**
 * Pega informações necessarias para montagem do extrato
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>USU_ID: id do usuario </li>
 * </ul>
 * @return Um json com diversas linhas do banco de dados, cada um das linhas possui as seguintes informações:
 * <ol>
 *      <li>TIPO: tipo da transação </li>
 *      <li>DATA: data da transação </li>
 *      <li>VALOR: valor da transação (positivo quando o usuario recebeu e negativo quando o usuario enviou) </li>
 *      <li>NOME: nome do usuario que participou da transação junto com usuario que teve seu id fornecido</li>
 * </ol>
 */
char * extrato (char * post)
{
    ListCampo post_data = convertObj(post);

    // Seleciona dados relevantes para o extrato
    char * query = "SELECT "
                   "T.TRAN_ID,"
                   "T.USU_ID_ORIGEM,"
                   "T.USU_ID_DESTINO,"
                   "T.TRAN_VALOR,"
                   "T.TRAN_DATA,"
                   "T.TRAN_TIPO,"
                   "U1.USU_NOME,"
                   "U2.USU_NOME "
                   "FROM "
                   "TAB_TRANSACOES T,"
                   "TAB_USUARIO U1,"
                   "TAB_USUARIO U2 "
                   "WHERE "
                   "(USU_ID_ORIGEM = ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," OR USU_ID_DESTINO = ");
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query,") AND U1.USU_ID = T.USU_ID_ORIGEM AND U2.USU_ID = T.USU_ID_DESTINO ORDER BY TRAN_ID DESC");
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0){
        // Constroi json
        char * json = "[";
        for (int i = 0; i< retorno.tamanho;i++) // Loop para construir as linhas do extrato no json
        {
            json = concatena (json,"{\"TIPO\":\"");
            json = concatena(json,retorno.list_campos[i].campos[5].valor);
            json = concatena(json,"\",\"DATA\":\"");
            json = concatena(json,retorno.list_campos[i].campos[4].valor);
            json = concatena(json,"\",\"VALOR\":\"");
            if(strcmp(post_data.campos[0].valor,retorno.list_campos[i].campos[1].valor) == 0) // Se o id for igual ao id de origem então se trata de um valor enviado, portanto negativo
            {
                json = concatena(json,"-");
                json = concatena(json,retorno.list_campos[i].campos[3].valor);
                json = concatena(json,"\",\"NOME\":\"");
                json = concatena(json,retorno.list_campos[i].campos[7].valor);
            }else{ // Caso o id seja diferente do id de origem então o valor foi recebido, portanto o valor é positivo
                json = concatena(json,retorno.list_campos[i].campos[3].valor);
                json = concatena(json,"\",\"NOME\":\"");
                json = concatena(json,retorno.list_campos[i].campos[6].valor);
            }
    
            json = concatena(json, "\"}");
            if (i != retorno.tamanho - 1)
            {
                json = concatena(json, ",");
            }
        }
        json = concatena(json, "]");
        return json;
    }else{
        return "{\"mensagem\":\"erro\"}";
    }
}