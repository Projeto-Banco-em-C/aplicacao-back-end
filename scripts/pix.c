#include "pix.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

    if(retorno.tamanho > 0)
    {
        char * json = convertJSON(retorno);
        return json;
    }
    else
    {
        return "{\"mensagem\":\"erro\"}";
    }



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


// id usurio / nome pix
char * adicionar_contato_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_CONTATOS_PIX WHERE CON_CHAVE = \'";
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, "\' AND USU_ID = ");
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0){
        return "{\"mensagem\":\"contato já existente\"}";
    }else {

        char * query1 = "SELECT USU_ID FROM TAB_CHAVES_PIX WHERE CHA_CODIGO = \'";
        query1 = concatena(query1, post_data.campos[1].valor);
        query1 = concatena(query1, "\'");
        retorno = bd(query1);

        if(strcmp(retorno.list_campos[0].campos[0].valor,post_data.campos[0].valor) != 0) {
            char *query2 = "INSERT INTO TAB_CONTATOS_PIX (USU_ID, CON_CHAVE, CON_NOME) VALUES (";
            query2 = concatena(query2, post_data.campos[0].valor);
            query2 = concatena(query2, ",\'");
            query2 = concatena(query2, post_data.campos[1].valor);
            query2 = concatena(query2, "\',\'");
            query2 = concatena(query2, post_data.campos[2].valor);
            query2 = concatena(query2, "\')");

            retorno = bd(query2);

            return "{\"mensagem\":\"contato cadastrado\"}";
        }else{
            return "{\"mensagem\":\"erro de adição a si proprio\"}";
        }
    }


}

char * lista_chave_pix(char * post)
{
    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_CHAVES_PIX WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0)
    {
        char * json = convertJSON(retorno);
        return json;
    }
    else
    {
        return "{\"mensagem\":\"erro\"}";
    }


}

