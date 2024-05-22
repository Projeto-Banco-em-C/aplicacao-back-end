
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
char * list_historico(char * post)
{







}

// Pegar a chave pix do front_end enviar para o banco de dados
char * adicionar_chavePix(char * post)
{








}

// Pegar o front_end o valor da transfeferencia remover da conta e acrecentar no outra
char * transferir_pix(char * post)
{








}

