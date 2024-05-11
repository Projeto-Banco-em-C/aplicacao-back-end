#include "cadastro.h"
#include "../controller/bd.h"
#include <stdio.h>


char * cadastro(char * post){ // Não precisa estar nesse arquivo
    //Exemplo
    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_USUARIO WHERE USU_ID = ";
    query = concatena(query,post_data.campos[0].valor);
    Linhas retorno = bd(query);

    printf("%s - ",retorno.list_campos[0].campos[0].key);
    printf("%s",retorno.list_campos[0].campos[0].valor);

    char * json = convertJSON(retorno);

    return json;
}
