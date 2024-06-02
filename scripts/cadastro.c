#include "cadastro.h"
#include "../controller/bd.h"
#include <stdio.h>
#include <string.h>


/**
 * Cadastra um novo usuario
 * @param post é uma string no formato json com as seguintes informações:
 * <ol>
 *      <li>USU_EMAIL: email do usuario </li>
 *      <li>USU_SENHA_ACESSO: senha hash do usuario</li>
 *      <li>USU_CPF: cpf do usuario com mascara</li>
 *      <li>USU_NOME: nome do usuario</li>
 *      <li>USU_DATA_NASC: data de nascimento do usuario formato dd/mm/aaaa</li>
 *      <li>USU_TELEFONE: telefone do usuario com mascara</li>
 *      <li>USU_CEP: cep da residencia do usuario com mascara</li>
 *      <li>USU_LOGRADOURO: logradouro da residencia do usuario</li>
 *      <li>USU_NUM_ENDERECO: numero da residencia do usuario</li>
 *      <li>USU_BAIRRO: bairro da residencia do usuario</li>
 *      <li>USU_CIDADE: cidade da residencia do usuario</li>
 *      <li>USU_UF: estado da residencia do usuario</li>
 *      <li>USU_COMPLEMENTO: complemento se necessario para o endereço da residencia do usario</li>
 * </ol>
 * @return Uma mensagem json
 */
char * cadastro(char * post){
    ListCampo post_data = convertObj(post);

    //Insere no banco os dados do usuario
    char * query = "INSERT INTO TAB_USUARIO( ";
    for (int i = 0; i < post_data.tamanho; i++) // Loop para pegar todos os nomes das colunas recebidos no json
    {
        query = concatena(query,post_data.campos[i].key);
        if (i != post_data.tamanho - 1)
        {
            query = concatena(query,",");
        }
    }
    query = concatena(query,") VALUES (");

    for (int i = 0; i < post_data.tamanho; i++) // Loop para pegar todos os valores recebidos no json
    {
        query = concatena(query,"\'");
        query = concatena(query,post_data.campos[i].valor);
        query = concatena(query,"\'");
        if (i != post_data.tamanho - 1)
        {
            query = concatena(query,",");
        }
    }
    query = concatena(query, ")");
    Linhas retorno = bd(query);

    //Seleciona o usuario pelo cpf para colocar o numero da conta
    char * query2 = "SELECT USU_ID FROM TAB_USUARIO WHERE USU_CPF = \'";
    for (int i = 0; i < post_data.tamanho; i++)
    {
        if (strcmp(post_data.campos [i].key,"USU_CPF") == 0 )
        {
            query2 = concatena(query2,post_data.campos[i].valor);
            break;
        }
    }

    query2 = concatena(query2, "\'");
    Linhas retorno2 = bd(query2);
    char * numConta = retorno2.list_campos[0].campos[0].valor;
    numConta = concatena(numConta, "\0");
    int tamanho = strlen(numConta);
    for (int i = tamanho; i < 5; i++ )
    {
        numConta = concatena("0", numConta);
    }
    //atualiza os dados do banco com os numeros da conta
    char * query3 = "UPDATE TAB_USUARIO SET USU_NUM_CONTA = \'";
    query3 = concatena(query3, numConta);
    query3 = concatena(query3, "\' WHERE USU_ID = \'");
    query3 = concatena(query3, retorno2.list_campos[0].campos[0].valor);
    query3 = concatena(query3,"\'");
    Linhas retorno3 = bd(query3);
    return "{\"mensagem\":\"ok\"}";



}
