#include "emprestimo.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "transferencia.h"

/**
 * Cadastra novos emprestimos para um determinado usuario.
 * @param post é uma string no formato json com as seguintes informações:
 * <ol>
 *      <li>USU_ID: id do usuario </li>
 *      <li>EMP_VALOR_TOTAL: valor do emprestimo </li>
 *      <li>EMP_NUM_PARCELAS: numero de parcelas do emprestimo </li>
 *      <li>EMP_JUROS_MENSAL: valor adicionado mensalmente no valor da parcela </li>
 *      <li>EMP_DATA_INICIO: data do inicio do emprestimo </li>
 * </ol>
 * @return Uma mensagem json
 */
char * adicionar_emprestimo(char * post)
{

    ListCampo post_data = convertObj(post);

    // Insere o novo emprestimo na tabela de emprestimo
    char * query = "INSERT INTO TAB_EMPRESTIMO (USU_ID, EMP_VALOR_TOTAL, EMP_NUM_PARCELAS, EMP_JUROS_MENSAL, EMP_DATA_INICIO) VALUES (";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[2].valor);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[3].valor);
    query = concatena(query, ",\'");
    query = concatena(query, post_data.campos[4].valor);
    query = concatena(query, "\')");

    Linhas retorno = bd(query);

    // Atualiza o limite de emprestimo do usuario
    char * query02 = "UPDATE TAB_USUARIO SET USU_LIM_EMPRESTIMO = USU_LIM_EMPRESTIMO - ";
    query02 = concatena(query02, post_data.campos[1].valor);
    query02 = concatena(query02, " WHERE USU_ID = ");
    query02 = concatena(query02, post_data.campos[0].valor);
    retorno = bd(query02);

    // Faz a transação do banco para o usuario com o valor do emprestimo
    char * json = "{USU_ID_ORIGEM:1,USU_ID_DESTINO:";
    json = concatena(json,post_data.campos[0].valor);
    json = concatena(json,",VALOR:");
    json = concatena(json,post_data.campos[1].valor);
    json = concatena(json,",TIPO:EMPRESTIMO}");
    char * retornoJson = transferir(json);


    return  "{\"mensagem\":\"emprestimo cadastrado\"}";

}

/**
 * Lista os emprestimos de um determinado usuario.
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>USU_ID: id do usuario </li>
 * </ul>
 * @return Um json com diversas linhas, sendo cada uma todos os dados sobre um emprestimo
 */
char * lista_emprestimo(char * post) {

    ListCampo post_data = convertObj(post);

    // Pega todos os dados da tabela emprestimo com base no id do usuario
    char *query = "SELECT * FROM TAB_EMPRESTIMO WHERE USU_ID =";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    if (retorno.tamanho > 0) {
        char * retornoJson = convertJSON(retorno); // Constroi o json de retorno
        return retornoJson;
    } else {
        return "{\"mensagem\":\"nao possui emprestimos\"}";
    }
}



/**
 * Atualiza o valor de contas com saldo negativo e faz a cobrnaça de emprestimos, deve ser executado ao menos uma vez por dia
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>DIA: o dia atual do mês</li>
 * </ul>
 * @return Uma mensagem json
 */
char * atualiza_emprestimo(char * post){
    ListCampo post_data = convertObj(post);

    // Atualiza todos que tem saldo negativo
    char * query = "SELECT ROUND(USU_SALDO*0.0027,2)*(-1) AS VALOR, USU_ID FROM TAB_USUARIO WHERE USU_SALDO < 0 AND USU_COBRADO <>";
    query = concatena(query,post_data.campos[0].valor);

    Linhas retorno = bd(query);
    for (int i = 0; i < retorno.tamanho; i++) { // Loop para executar a transferencia de todos juros de todos que tem saldo negativo para o banco
        // Monta o json para fazer a transferncia
        char * json = "{USU_ID:";
        json = concatena(json,retorno.list_campos[i].campos[1].valor);
        json = concatena(json,",USU_ID:1,VALOR:");
        json = concatena(json,retorno.list_campos[i].campos[0].valor);
        json = concatena(json,",TIPO:JUROS}");
        char * retornoJson = transferir(json);
    }

    // Atualiza todos usuarios para não serem cobrados duas vezes no mesmo dia
    char * query4 = "UPDATE TAB_USUARIO SET USU_COBRADO = ";
    query4 = concatena(query4,post_data.campos[0].valor);
    Linhas retorno2 = bd(query4);

    if(strcmp(post_data.campos[0].valor,"15") == 0){ // Se o dia for 15 então deve combrar os emprestimos

        // Pega todos que tem algum emprestimo ativo
        char * query1 = "SELECT USU_ID, ROUND((EMP_VALOR_TOTAL/EMP_NUM_PARCELAS + EMP_JUROS_MENSAL),2) AS MENSALIDADE, EMP_ID FROM TAB_EMPRESTIMO WHERE EMP_NUM_PARCELAS_PAGAS < EMP_NUM_PARCELAS AND EMP_COBRADO <> 1";
        retorno = bd(query1);

        for (int i = 0; i < retorno.tamanho; i++) { // Loop para cobrar todos que tem algum emprestimo ativo
            // Faz a cobrança
            char * json = "{USU_ID_ORIGEM:";
            json = concatena(json,retorno.list_campos[i].campos[0].valor);
            json = concatena(json,",USU_ID_DESTINO:1,VALOR:");
            json = concatena(json,retorno.list_campos[i].campos[1].valor);
            json = concatena(json,",TIPO:EMPRESTIMO}");
            char * retornoJson = transferir(json);

            // Atualiza o emprestimo para não ser cobrado duas vezes
            char * query3 = "UPDATE TAB_EMPRESTIMO SET EMP_VALOR_PAGO = (EMP_VALOR_PAGO + EMP_VALOR_TOTAL/EMP_NUM_PARCELAS), EMP_NUM_PARCELAS_PAGAS = EMP_NUM_PARCELAS_PAGAS + 1, EMP_COBRADO = 1 WHERE EMP_ID = ";
            query3 = concatena(query3,retorno.list_campos[i].campos[2].valor);
            Linhas retorno1 = bd(query3);
        }
    } else{
        // Atualiza a data no caso de não ser dia 15
        char * query3 = "UPDATE TAB_EMPRESTIMO SET EMP_COBRADO = 0 WHERE EMP_COBRADO <> 0";
        Linhas retorno1 = bd(query3);
    }

    return "{\"mensagem\":\"ok\"}";
}
