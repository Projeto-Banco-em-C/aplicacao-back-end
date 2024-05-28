#include "extrato.h"
#include "../controller/bd.h"

#include <string.h>
char * extrato (char * post)
{
    ListCampo post_data = convertObj(post);
    /*
    char * query = "SELECT * FROM TAB_TRANSACOES WHERE USU_ID_ORIGEM=";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query, " OR USU_ID_DESTINO = ");
    query = concatena(query,post_data.campos[0].valor);
    */
    char * query = "SELECT "
                   "T.TRAN_ID,"
                   "T.USU_ID_ORIGEM,"
                   "T.USU_ID_DESTINO,"
                   "T.TRAN_VALOR,"
                   "T.TRAN_DATA,"
                   "T.TRAN_TIPO,"
                   "U1.USU_NOME,"
                   "U2.USU_NOME "
                   "FROM "
                   "TAB_TRANSACOES T,"
                   "TAB_USUARIO U1,"
                   "TAB_USUARIO U2 "
                   "WHERE "
                   "(USU_ID_ORIGEM = ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," OR USU_ID_DESTINO = ");
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query,") AND U1.USU_ID = T.USU_ID_ORIGEM AND U2.USU_ID = T.USU_ID_DESTINO");
    Linhas retorno = bd(query);
    if(retorno.tamanho > 0){
        char * json = "[";
        for (int i = 0; i< retorno.tamanho;i++)
        {
            json = concatena (json,"{\"TIPO\":\"");
            json = concatena(json,retorno.list_campos[i].campos[5].valor);
            json = concatena(json,"\",\"DATA\":\"");
            json = concatena(json,retorno.list_campos[i].campos[4].valor);
            json = concatena(json,"\",\"VALOR\":\"");
            if(strcmp(post_data.campos[0].valor,retorno.list_campos[i].campos[1].valor) == 0) // Se o id for igual ao id de origem
            {
                json = concatena(json,"-");
                json = concatena(json,retorno.list_campos[i].campos[3].valor);
                json = concatena(json,"\",\"NOME\":\"");
                json = concatena(json,retorno.list_campos[i].campos[7].valor);
            }else{
                json = concatena(json,retorno.list_campos[i].campos[3].valor);
                json = concatena(json,"\",\"NOME\":\"");
                json = concatena(json,retorno.list_campos[i].campos[6].valor);
            }
    
            json = concatena(json, "\"}");
            if (i != retorno.tamanho - 1)
            {
                json = concatena(json, ",");
            }
        }
        json = concatena(json, "]");
        return json;
    }else{
        return "{\"mensagem\":\"erro\"}";
    }
}
