#include "emprestimo.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>

char * adicionar_emprestimo(char * post)
{

    ListCampo post_data = convertObj(post);
    char * query = "INSERT INTO TAB_EMPRESTIMO (USU_ID, EMP_VALOR_TOTAL, EMP_NUM_PARCELAS, EMP_JURUS_MENSAL, EMP_DATA_INICIO) VALUES (";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[2].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[3].valor);
    query = concatena(query, ",\'");
    query = concatena(query, post_data.campos[4].valor);
    query = concatena(query, "\')");

    Linhas retorno = bd(query);

    char * query02 = "UPDATE TAB_USUARIO SET USU_LIM_EMPRESTIMO = USU_LIM_EMPRESTIMO - ";
    query02 = concatena(query02, post_data.campos[1].valor);
    query02 = concatena(query02, " WHERE USU_ID = ");
    query02 = concatena(query02, post_data.campos[0].valor);
    retorno = bd(query02);
    

    char * query03 = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO + ";
    query03 = concatena(query03, post_data.campos[1].valor);
    query03 = concatena(query03, " WHERE USU_ID = ");
    query03 = concatena(query03, post_data.campos[0].valor);
    retorno = bd(query03);

    char * query04 = "SELECT "
                   "USU_ID,"
                   "EMP_VALOR_TOTAL,"
                   "EMP_VALOR_PAGO,"
                   "EMP_NUM_PARCELAS,"
                   "EMP_JURUS_MENSAL,"
                   "EMP_DATA_INICIO,"
                   "EMP_COBRADO "
                   "FROM "
                   "TAB_EMPRESTIMO "
                   "WHERE "
                   "(USU_ID = ";
    query04  = concatena(query04,post_data.campos[0].valor);
    query04 = concatena(query04,")");
    retorno = bd(query04);
    char * json = "[";
    for (int i = 0; i< retorno.tamanho;i++)
    {

        json = concatena (json,"{\"USU_ID\":\"");
        json = concatena(json,retorno.list_campos[i].campos[0].valor);
        json = concatena(json,"\",\"EMP_VALOR_TOTAL\":\"");
        json = concatena(json,retorno.list_campos[i].campos[1].valor);
        json = concatena(json,"\",\"EMP_VALOR_PAGO\":\"");
        json = concatena(json,retorno.list_campos[i].campos[2].valor);
        json = concatena(json,"\",\"EMP_NUM_PARCELAS\":\"");
        json = concatena(json,retorno.list_campos[i].campos[3].valor);
        json = concatena(json,"\",\"EMP_JURUS_MENSAL\":\"");
        json = concatena(json,retorno.list_campos[i].campos[4].valor);
        json = concatena(json,"\",\"EMP_DATA_INICIO\":\"");
        json = concatena(json,retorno.list_campos[i].campos[5].valor);
        json = concatena(json,"\",\"T.EMP_COBRADO\":\"");
        json = concatena(json,retorno.list_campos[i].campos[6].valor);
        json = concatena(json, "\"}");

    }
    json = concatena(json, "]");

    return json;

    //return  "{\"mensagem\":\"emprestimo cadastrado\"}";

}