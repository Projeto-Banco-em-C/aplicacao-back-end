#include "login.h"
#include <stdio.h>
#include <string.h>
#include "../controller/bd.h"
char * login(char * post){
    ListCampo post_data = convertObj(post);
    char * query = "SELECT USU_SENHA FROM TAB_USUARIO WHERE USU_CPF = \'";
    query = concatena(query, post_data.campos[0].valor );
    query = concatena(query, "\'");
    Linhas retorno = bd(query);

    if (retorno.tamanho > 0)
    {
        if (strcmp(retorno.list_campos[0].campos[0].valor,post_data.campos[1].valor) ==0 )
        {
            return "{\"mensagem\":\"ok\"}";
        } else
        {
            return "{\"mensagem\":\"senha incorreta\"}";
        }
    } else
    {
         return "{\"mensagem\":\"usuario inexistente\"}";
    }
}