#include "validacaoCpf.h"
#include <stdio.h>
#include "../controller/bd.h"
char * validacaoCpf(char * post){
    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_USUARIO WHERE USU_CPF = \'";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, "\'");
    Linhas retorno = bd(query);


    if (retorno.tamanho > 0)
    {
        return "{\"mensagem\":\"ok\"}";
    } else
    {
        return "{\"mensagem\":\"erro\"}";
    }
}