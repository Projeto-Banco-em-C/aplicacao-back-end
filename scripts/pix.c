
#include "pix.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>

// verificar se a salto o sufuciente antes de proceguir
char * verifica_saldo(char * post) {

    ListCampo post_data = convertObj(post);

    char * query = "SELECT USU_SALDO FROM TAB_USUARIO WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);
    double saldo = strtod(retorno.list_campos[0].campos[0].valor, NULL);
    double valor = strtod(post_data.campos[1].valor, NULL);



    if (saldo >= valor){

        return "{\"mensagem\":\"ok\"}";


    }else{
        return "{\"mensagem\":\"saldo insuficiente\"}";
    }

}

// Enviar historio para o front-end
char * list_historico_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_CONTATOS_PIX WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    char * json = convertJSON(retorno);

    return json;

}

// Pegar a chave pix do front_end enviar para o banco de dados
char * adicionar_chave_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_CHAVES_PIX WHERE CHA_CODIGO = \'";
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, "\'");
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0)
    {

        return "{\"mensagem\":\"pix ja existente\"}";

    }
    else
    {

        char * query02 = "INSERT INTO TAB_CHAVES_PIX VALUES (";
        query02 = concatena(query02, post_data.campos[0].valor);
        query02 = concatena(query02, ",");
        query02 = concatena(query02, "'");
        query02 = concatena(query02, post_data.campos[1].valor);
        query02 = concatena(query02, "'");
        query02 = concatena(query02, ")");
        Linhas retorno02 = bd(query02);

        return "{\"mensagem\":\"chave pix adicionada\"}";

    }

}

//consultar informaçoes do pix
char * consulta_info_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT U.USU_ID, U.USU_NOME, U.USU_CPF FROM TAB_CHAVES_PIX C,TAB_USUARIO U WHERE U.USU_ID = C.USU_ID AND C.CHA_CODIGO = \'";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query,"\'");
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0)
    {

        char * json = "{\"mensagem\":\"ok\",\"USU_ID\":\"";
        json = concatena(json, retorno.list_campos[0].campos[0].valor);
        json = concatena(json, "\",\"USU_NOME\":\"");
        json = concatena(json, retorno.list_campos[0].campos[1].valor);
        json = concatena(json, "\",\"USU_CPF\":\"");
        json = concatena(json, retorno.list_campos[0].campos[2].valor);
        json = concatena(json, "\"}");

        return  json;

    }
    else
    {

        return "{\"mensagem\":\"chave nao existente\"}";

    }




}

// Pegar o front_end o valor da transfeferencia remover da conta e acrecentar no outra
char * transferir_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT USU_SALDO FROM TAB_USUARIO WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);
    char * saldo = retorno.list_campos[0].campos[0].valor;
    char * valor = post_data.campos[2].valor;


    char * query02 = "UPDATE TAB_USUARIO SET USU_SALDO = ";
    query02 = concatena(query02,  saldo);
    query02 = concatena(query02,  " - ");
    query02 = concatena(query02,  valor);
    query02 = concatena(query02,  " WHERE USU_ID = ");
    query02 = concatena(query02,  post_data.campos[0].valor);

    retorno = bd(query02);

    //-------------------------------------------

    char * query03 = "SELECT USU_SALDO FROM TAB_USUARIO WHERE USU_ID = ";
    query03 = concatena(query03, post_data.campos[1].valor);
    retorno = bd(query03);
    char * saldo02 = retorno.list_campos[0].campos[0].valor;

    char * query04 = "UPDATE TAB_USUARIO SET USU_SALDO = ";
    query04 = concatena(query04,  saldo02);
    query04 = concatena(query04,  " + ");
    query04 = concatena(query04,  valor);
    query04 = concatena(query04,  " WHERE USU_ID = ");
    query04 = concatena(query04,  post_data.campos[1].valor);

    retorno = bd(query04);

    //--------------------------------

    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char * data;
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);


    char * query05 = "INSERT INTO TAB_TRANSACOES (USU_ID_ORIGEM,USU_ID_DESTINO,TRAN_VALOR,TRAN_DATA,TRAN_TIPO) VALUES (";
    query05 = concatena(query05,post_data.campos[0].valor);
    query05 = concatena(query05,",");
    query05 = concatena(query05,post_data.campos[1].valor);
    query05 = concatena(query05,",");
    query05 = concatena(query05,post_data.campos[2].valor);
    query05 = concatena(query05,",\'");
    query05 = concatena(query05,data);
    query05 = concatena(query05,"\',\'PIX\')");

    retorno = bd(query05);

    return "{\"mensagem\":\"ok\"}";

}

