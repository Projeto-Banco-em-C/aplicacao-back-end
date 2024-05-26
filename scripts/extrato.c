#include "extrato.h"
#include "../controller/bd.h"

#include <string.h>
 char * extrato (char * post)
 {
     ListCampo post_data = convertObj(post);

     char * query = "SELECT * FROM TAB_TRANSACOES WHERE USU_ID_ORIGEM=";
     query = concatena(query,post_data.campos[0].valor);
     query = concatena(query, " OR USU_ID_DESTINO = ");
     query = concatena(query,post_data.campos[0].valor);

     Linhas retorno = bd(query);
     char * json = "[";
     for (int i = 0; i< retorno.tamanho;i++)
     {
         json = concatena (json, "{\"TIPO\":\"");
         json = concatena(json, retorno.list_campos[i].campos[5].valor);
         json = concatena(json, "\",\"DATA\":\"");
         json = concatena(json, retorno.list_campos[i].campos[4].valor);
         json = concatena(json,"\",\"VALOR\":\"");
         if(strcmp(post_data.campos[0].valor,retorno.list_campos[i].campos[1].valor) == 0)
         {
             json = concatena(json, "-");
         }
         json = concatena(json,retorno.list_campos[i].campos[3].valor);
         json = concatena(json, "\"}");
         if (i != retorno.tamanho - 1)
         {
             json = concatena(json, ",");
         }
     }
     json = concatena(json, "]");
 }