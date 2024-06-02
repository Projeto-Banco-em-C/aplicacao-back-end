#include "validacaoCpf.h"
#include <stdio.h>
#include "../controller/bd.h"

/**
 * Verifica se o usuario existe
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>USU_CPF: CPF do usuario</li>
 * </ul>
 * @return Uma mensagem json indicando que o usuario existe ou não
 */
char * validacaoCpf(char * post){
    ListCampo post_data = convertObj(post);

    // Pega o usuario pelo CPF para ver se ele existe
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