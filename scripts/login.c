#include "login.h"
#include <stdio.h>
#include <string.h>
#include "../controller/bd.h"
#include <stdlib.h>

/**
 * Verifica as credencias do usuario
 * @param post é uma string no formato json com as seguintes informações:
 * <ol>
 *      <li>USU_CPF: cpf do usuario </li>
 *      <li>USU_SENHA_ACESSO: senha hash do usuario </li>
 * </ol>
 * @return Uma mensagem json informando se o usuario está correto ou não, em caso de estar correto fornece o USU_ID no mesmo json
 */
char * login(char * post){
    ListCampo post_data = convertObj(post);

    // Pega a senha, id e numero de tentativas com base no cpf
    char * query = "SELECT USU_SENHA_ACESSO, USU_ID, USU_TENTATIVAS FROM TAB_USUARIO WHERE USU_CPF = \'";
    query = concatena(query, post_data.campos[0].valor );
    query = concatena(query, "\'");
    Linhas retorno = bd(query);

    if (retorno.tamanho > 0)
    {
        if(atoi(retorno.list_campos[0].campos[2].valor) < 3) { // Verifica se não houve mais de 3 tentativas
            if (strcmp(retorno.list_campos[0].campos[0].valor, post_data.campos[1].valor) == 0) { // Verifica se a senha está correta
                // Reinicia a contagem de tentativas
                char *query1 = "UPDATE TAB_USUARIO SET USU_TENTATIVAS = 0 WHERE USU_ID = ";
                query1 = concatena(query1, retorno.list_campos[0].campos[1].valor);
                Linhas retorno1 = bd(query1);

                // Constroi o json com o id do usuario
                char *msg = "{\"mensagem\":\"ok\",\"USU_ID\":\"";
                msg = concatena(msg, retorno.list_campos[0].campos[1].valor);
                msg = concatena(msg, "\"}");
                return msg;
            } else {// No caso da senha estar errada
                // Incrementa em um a contagem de tentativas
                char *query1 = "UPDATE TAB_USUARIO SET USU_TENTATIVAS = USU_TENTATIVAS + 1 WHERE USU_ID = ";
                query1 = concatena(query1, retorno.list_campos[0].campos[1].valor);
                Linhas retorno1 = bd(query1);
                return "{\"mensagem\":\"senha incorreta\"}";
            }
        }else{
            return "{\"mensagem\":\"senha bloqueada\"}";
        }
    } else
    {
         return "{\"mensagem\":\"usuario inexistente\"}";
    }
}