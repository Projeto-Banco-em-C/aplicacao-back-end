#include "cadastro.h"
#include "../controller/bd.h"
#include <stdio.h>
#include <string.h>

char * cadastro(char * post){
    ListCampo post_data = convertObj(post);

    //INSERIR DADOS NO BANCO
    char * query = "INSERT INTO TAB_USUARIO( ";
    for (int i = 0; i < post_data.tamanho; i++)
    {
        query = concatena(query,post_data.campos[i].key);
        if (i != post_data.tamanho - 1)
        {
            query = concatena(query,",");
        }
    }
    query = concatena(query,") VALUES (");

    for (int i = 0; i < post_data.tamanho; i++)
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

    //seleciona a linha pelo cpf para colocar o numero da conta
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
