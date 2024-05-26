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

char * transferir_ted(char * post){

    ListCampo post_data = convertObj(post);

    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char * data;
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    // Retira o saldo do remetente
    char * query = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO - ";
    query = concatena(query, post_data.campos[2].valor);
    query = concatena(query, " WHERE USU_ID = ");
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    // Insere no saldo do destinatario
    char * query1 = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO + ";
    query1 = concatena(query1, post_data.campos[2].valor);
    query1 = concatena(query1, " WHERE USU_ID = ");
    query1 = concatena(query1, post_data.campos[1].valor);
    retorno = bd(query1);

    // Insere na tabela de transação
    char * query2 = "INSERT INTO TAB_TRANSACOES (USU_ID_ORIGEM,USU_ID_DESTINO,TRAN_VALOR,TRAN_DATA,TRAN_TIPO)VALUES(";
    query2 = concatena(query2, post_data.campos[0].valor);
    query2 = concatena(query2, ",");
    query2 = concatena(query2, post_data.campos[1].valor);
    query2 = concatena(query2, ",");
    query2 = concatena(query2, post_data.campos[2].valor);
    query2 = concatena(query2, ",\'");
    query2 = concatena(query2, data);
    query2 = concatena(query2, "\',\'");
    query2 = concatena(query2, "TED");
    query2 = concatena(query2, "\')");
    retorno = bd(query2);

    return "{\"mensagem\":\"ok\"}";
}
