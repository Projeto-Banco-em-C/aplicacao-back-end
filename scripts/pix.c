
#include "pix.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
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
// Pegar o front_end o valor da transfeferencia remover da conta e acrecentar no outra
char * adicionar_chave_pix(char * post)
{








}

//consultar informaçoes do pix
char * consulta_info_pix(char * post)
{

    ListCampo post_data = convertObj(post);

    char * query = "SELECT U.USU_ID, U.USU_NOME FROM TAB_CHAVES_PIX C,TAB_USUARIO U WHERE U.USU_ID = C.USU_ID AND C.CHA_CODIGO = \'";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query,"\'");
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0)
    {

        char * json = "{\"mensagem\":\"ok\",\"USU_NOME\":\"";
        json = concatena(json, retorno.list_campos[0].campos[1].valor);
        json = concatena(json, "\"}");

        return  json;

    }
    else
    {

        return "{\"mensagem\":\"chave nao existente\"}";

    }




}


char * transferir_pix(char * post)
{








}

