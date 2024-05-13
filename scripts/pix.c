
#include "pix.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>

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


char * list_hitorico(char * post){







}


