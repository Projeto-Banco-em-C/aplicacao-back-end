#include "investimento.h"
#include "../controller/bd.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "transferencia.h"
#include <stdlib.h>

/**
 * Cadastra novos investimentos para um determinado usuario.
 * @param post é uma string no formato json com as seguintes informações:
 * <ol>
 *      <li>USU_ID: id do usuario </li>
 *      <li>INV_VALOR: valor investido </li>
 *      <li>INV_TIPO: tipo do investimento </li>
 *      <li>INV_JUROS: juros do investimento </li>
 * </ol>
 * @return Uma mensagem json
 */
char * investimento(char * post) {
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);
    char dia[3];
    sprintf(dia, "%d", data_hora->tm_mday);

    if(atoi(dia) > 28){ // Se o dia for maior que 28 ele vai para 28 para evitar problamas em fevereiro
        strcpy(dia,"28");
    }

    ListCampo post_data = convertObj(post);

    // Insere o novo investimento na tabela de investimento
    char *query = "INSERT INTO TAB_INVESTIMENTO( ";
    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, post_data.campos[i].key);
        query = concatena(query, ",");
    }
    query = concatena(query, "INV_DATA_INICIO, INV_DIA, INV_VALOR_ATUAL, INV_HIS");
    query = concatena(query, ") VALUES (");

    for (int i = 0; i < post_data.tamanho; i++) {
        query = concatena(query, "\'");
        query = concatena(query, post_data.campos[i].valor);
        query = concatena(query, "\'");
        query = concatena(query, ",");
    }
    query = concatena(query, "\'");
    query = concatena(query, data);
    query = concatena(query, "\',");
    query = concatena(query, dia);
    query = concatena(query, ",");
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, ",");
    query = concatena(query, dia);
    query = concatena(query, ")");
    Linhas retorno = bd(query);

    // Faz a transação para colocar o dinheiro do investimento na conta do banco
    char * json = "{USU_ID_ORIGEM:";
    json = concatena(json, post_data.campos[0].valor);
    json = concatena(json, ",USU_ID_DESTINO:1,VALOR:");
    json = concatena(json,post_data.campos[1].valor);
    json = concatena(json,",TIPO:INVESTIMENTO}");
    char * RetornoJSON = transferir(json);
    return "{\"mensagem\":\"aplicado\"}";
}

/**
 * Retira um valor de um determinado tipo de investimento.
 * @param post é uma string no formato json com as seguintes informações:
 * <ol>
 *      <li>USU_ID: id do usuario </li>
 *      <li>INV_TIPO: tipo do investimento </li>
 *      <li>INV_VALOR: valor investido </li>
 * </ol>
 * @return Uma mensagem json
 */
char * removeInvestimento(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega a lista de todos os investimentos ativos do usuario
    char * query = "SELECT INV_VALOR_ATUAL, INV_ID FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," AND INV_TIPO = \'");
    query = concatena(query,post_data.campos[1].valor);
    query = concatena(query,"\' AND INV_DATA_FIM = \'\'");
    Linhas retorno = bd(query);

    double valor_retirado = strtod(post_data.campos[2].valor,NULL);
    for (int i = 0; i < retorno.tamanho; i++) { // Loop para passar em todos os investimentos ativos, retirando o valor deles até atingir o valor desejado pelo usuario
        double valor = strtod(retorno.list_campos[i].campos[0].valor,NULL);
        if(valor <= valor_retirado){ // Se o valor desse investimento for menor ou igual ao valor retirado, então o investimento deve ser encerrado
            valor_retirado -= valor;

            //Atualiza o valor atual do investimento e encerra ele
            char * query1 = "UPDATE TAB_INVESTIMENTO SET INV_DATA_FIM = \'";
            query1 = concatena(query1,data);
            query1 = concatena(query1," \', INV_VALOR_ATUAL = 0 WHERE INV_ID = ");
            query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
            Linhas retorno1 = bd(query1);

        }else{ // No caso do valor do investimento ser maior que o valor que será retirado, o valor é apenas atualizado
            valor -= valor_retirado;
            char val[20];
            sprintf(val,"%.2f",valor);

            //Atualiza o valor atual
            char * query1 = "UPDATE TAB_INVESTIMENTO SET INV_VALOR_ATUAL = ";
            query1 = concatena(query1,val);
            query1 = concatena(query1," WHERE INV_ID = ");
            query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
            Linhas retorno1 = bd(query1);
            break;
        }
    }

    // Faz a transação da conta do banco para a conta do usuario com o valor que foi retirado do investimento
    char * json = "{USU_ID_ORIGEM:1,USU_ID_DESTINO:";
    json = concatena(json,post_data.campos[0].valor);
    json = concatena(json,",VALOR:");
    json = concatena(json,post_data.campos[2].valor);
    json = concatena(json,",TIPO:INVESTIMENTO}");
    char * retornoJson = transferir(json);


    return "{\"mensagem\":\"ok\"}";
}

/**
 * Atualiza o valor dos investimentos, deve ser executado ao menos uma vez por dia
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>DIA: o dia atual do mês</li>
 * </ul>
 * @return Uma mensagem json
 */
char * atualizaInvestimentos(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega a lista de investimentos que serão atualizados no dia atual
    char * query = "SELECT INV_ID, ROUND(INV_VALOR_ATUAL*INV_JUROS,2), INV_HIS AS VALOR_NOVO FROM TAB_INVESTIMENTO WHERE INV_DIA =  ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," AND INV_DATA_FIM = \'\' AND INV_HIS <> ");
    query = concatena(query,post_data.campos[0].valor);
    Linhas retorno = bd(query);

    for (int i = 0; i < retorno.tamanho; i++) { // Loop para aplicar o rendimento nos investimentos que serão atualizados

        // Insere na tabela de historico o ganho do investimento
        char * query1 = "INSERT INTO TAB_HISTORICO_INVESTIMENTO (INV_ID,HIS_DATA,HIS_VALOR)VALUES(";
        query1 = concatena(query1,retorno.list_campos[i].campos[0].valor);
        query1 = concatena(query1,",\'");
        query1 = concatena(query1,data);
        query1 = concatena(query1,"\',");
        query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
        query1 = concatena(query1,")");
        Linhas retorno1 = bd(query1);

        // Atualiza o investimento para garantir que não seja atualizado duas vezes e atualiza o valor atual do investimento
        char * query2 = "UPDATE TAB_INVESTIMENTO SET INV_VALOR_ATUAL = INV_VALOR_ATUAL + ";
        query2 = concatena(query2,retorno.list_campos[i].campos[1].valor);
        query2 = concatena(query2,", INV_HIS = ");
        query2 = concatena(query2,post_data.campos[0].valor);
        query2 = concatena(query2," WHERE INV_ID = ");
        query2 = concatena(query2,retorno.list_campos[i].campos[0].valor);
        retorno1 = bd(query2);
    }

    // O dia de todos os investimentos para que sejam atualizados no dia correto
    char * query3 = "UPDATE TAB_INVESTIMENTO SET INV_HIS = ";
    query3 = concatena(query3,post_data.campos[0].valor);
    Linhas retorno2 = bd(query3);

    return "{\"mensagem\":\"ok\"}";
}

/**
 * Pega os ganhos de cada mês do ano atual de um dado usuario
 * @param post é uma string no formato json com a seguinte informação:
 * <ul>
 *      <li>USU_ID: id do usuario que se deja obter os ganhos</li>
 * </ul>
 * @return Um json com diversas linhas, sendo cada uma delas composta pelos seguintes campos:
 * <ol>
 *      <li>VALOR: valor ganho em um mês</li>
 *      <li>MES: mês do ano</li>
 * </ol>
 */
char * mostraGanhos(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char ano[5];
    sprintf(ano, "%04d", data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega ganhos
    char * query = "SELECT SUM(H.HIS_VALOR) AS VALOR, SUBSTR(H.HIS_DATA,4,2) AS MES FROM TAB_HISTORICO_INVESTIMENTO H, TAB_INVESTIMENTO I WHERE H.INV_ID = I.INV_ID AND I.USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, " AND SUBSTR(H.HIS_DATA,7,4) = \'");
    query = concatena(query, ano);
    query = concatena(query, "\' GROUP BY MES");
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);
    return retornoJson;
}

/**
 * Pega o total investido em um dado tipo de investimento de um dado usuario
 * @param post é uma string no formato json com as seguintes informações:
 * <ul>
 *      <li>USU_ID: id do usuario que se deja obter os ganhos</li>
 *      <li>INV_TIPO: tipo do investimento</li>
 * </ul>
 * @return Um json com as seguintes informações:
 * <ul>
 *      <li>USU_ID: id do usuario </li>
 *      <li>VALOR: valor disponivel para saque </li>
 * </ul>
 */
char * mostrarInvestimento(char * post)
{
    ListCampo post_data = convertObj(post);

    // Pega todas as informações sobre todos os investimentos com base no id do usuario
    char * query = "SELECT USU_ID, SUM(INV_VALOR_ATUAL) AS VALOR FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, " AND INV_TIPO = \'");
    query = concatena(query, post_data.campos[1].valor);
    query = concatena(query, "\' AND INV_DATA_FIM = \'\' GROUP BY USU_ID");
    Linhas retorno = bd(query);

    if(retorno.tamanho > 0) {
        char *retornoJson = convertJSON(retorno);
        return retornoJson;
    }else{
        return "{\"mensagem\":\"erro\"}";
    }
}
