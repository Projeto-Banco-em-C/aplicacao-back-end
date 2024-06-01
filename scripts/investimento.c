#include "investimento.h"
#include "../controller/bd.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "transferencia.h"
#include <stdlib.h>

char * investimento(char * post) {
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);
    char dia[3];
    sprintf(dia, "%d", data_hora->tm_mday);

    if(atoi(dia) > 28){ // Se o dia for mairo que 28 ele vai para 28 para evitar problamas em fevereiro
        strcpy(dia,"28");
    }

    ListCampo post_data = convertObj(post);
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
    char * json = "{USU_ID_ORIGEM:";
    json = concatena(json, post_data.campos[0].valor);
    json = concatena(json, ",USU_ID_DESTINO:1,VALOR:");
    json = concatena(json,post_data.campos[1].valor);
    json = concatena(json,",TIPO:INVESTIMENTO}");
    char * RetornoJSON = transferir(json);
    return "{\"mensagem\":\"aplicado\"}";
}

char * removeInvestimento(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega a lista de todos os investimentos ativos
    char * query = "SELECT INV_VALOR, INV_ID FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," AND INV_TIPO = \'");
    query = concatena(query,post_data.campos[1].valor);
    query = concatena(query," \' AND INV_DATA_FIM = \'\'");
    Linhas retorno = bd(query);

    double valor_retirado = strtod(post_data.campos[2].valor,NULL);
    for (int i = 0; i < retorno.tamanho; i++) {
        double valor = strtod(retorno.list_campos[i].campos[0].valor,NULL);
        if(valor < valor_retirado){
            valor_retirado -= valor;

            //Atualiza o valor atual do investimento e encerra ele
            char * query1 = "UPDATE TAB_INVESTIMENTO SET INV_DATA_FIM = \'";
            query1 = concatena(query1,data);
            query1 = concatena(query1," \', INV_VALOR_ATUAL = 0 WHERE INV_ID = ");
            query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
            Linhas retorno1 = bd(query1);

            //Insere o fim dele no historico
            /*
            char * query2 = "INSERT INTO TAB_HISTORICO_INVESTIMENTO (INV_ID,HIS_DATA,HIS_VALOR)VALUES(";
            query2 = concatena(query2,retorno.list_campos[i].campos[1].valor);
            query2 = concatena(query2,",\'");
            query2 = concatena(query2,data);
            query2 = concatena(query2,"\',");
            query2 = concatena(query2,"0)");
            retorno1 = bd(query2);
            */
        }else{
            //Atualiza o valor atual
            valor -= valor_retirado;
            char val[20];
            sprintf(val,"%.2f",valor);

            char * query1 = "UPDATE TAB_INVESTIMENTO SET INV_DATA_FIM = \'";
            query1 = concatena(query1,data);
            query1 = concatena(query1," \', INV_VALOR_ATUAL = ");
            query1 = concatena(query1,val);
            query1 = concatena(query1,"WHERE INV_ID = ");
            query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
            Linhas retorno1 = bd(query1);
            break;
        }
    }

    char * json = "{USU_ID_ORIGEM:1,USU_ID_DESTINO:";
    json = concatena(json,post_data.campos[0].valor);
    json = concatena(json,",VALOR:");
    json = concatena(json,post_data.campos[2].valor);
    json = concatena(json,",TIPO:INVESTIMENTO}");
    char * retornoJson = transferir(json);


    return "{\"mensagem\":\"ok\"}";
}

char * atualizaInvestimentos(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char data[11];
    sprintf(data, "%02d/%02d/%04d", data_hora->tm_mday, data_hora->tm_mon + 1, data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega a lista de investimentos que serão atualizados
    char * query = "SELECT INV_ID, ROUND(INV_VALOR_ATUAL*INV_JUROS,2) AS VALOR_NOVO FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query,post_data.campos[0].valor);
    query = concatena(query," AND INV_DIA = \'");
    query = concatena(query,post_data.campos[1].valor);
    query = concatena(query," \' AND INV_DATA_FIM = \'\' AND INV_HIS <> ");
    query = concatena(query,post_data.campos[1].valor);
    Linhas retorno = bd(query);

    for (int i = 0; i < retorno.tamanho; i++) {
        // Insere na tabela de historico a atualização do investimento
        char * query1 = "INSERT INTO TAB_HISTORICO_INVESTIMENTO (INV_ID,HIS_DATA,HIS_VALOR)VALUES(";
        query1 = concatena(query1,retorno.list_campos[i].campos[0].valor);
        query1 = concatena(query1,",\'");
        query1 = concatena(query1,data);
        query1 = concatena(query1,"\',");
        query1 = concatena(query1,retorno.list_campos[i].campos[1].valor);
        query1 = concatena(query1,")");
        Linhas retorno1 = bd(query1);

        // Atualiza o investimento para garantir que não seja atualizado de forma incorreta
        char * query2 = "UPDATE TAB_INVESTIMENTO SET INV_VALOR_ATUAL = '";
        query2 = concatena(query2,retorno.list_campos[i].campos[1].valor);
        query2 = concatena(query2,", INV_HIS = ");
        query = concatena(query,post_data.campos[1].valor);
        query2 = concatena(query2,"WHERE INV_ID = ");
        query2 = concatena(query2,retorno.list_campos[i].campos[1].valor);
        retorno1 = bd(query2);
    }

    return "{\"mensagem\":\"ok\"}";
}

char * mostraGanhos(char * post){
    time_t agora;
    time(&agora);

    struct tm * data_hora = localtime(&agora);

    char ano[5];
    sprintf(ano, "%04d", data_hora->tm_year + 1900);

    ListCampo post_data = convertObj(post);

    // Pega ganhos
    char * query = "SELECT SUM(H.HIS_VALOR), SUBSTR(H.HIS_DATA,4,2) AS MES FROM TAB_HISTORICO_INVESTIMENTO H, TAB_INVESTIMENTO I WHERE H.INV_ID = I.INV_ID AND I.USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    query = concatena(query, " AND SUBSTR(INV_DATA_FIM,7,4) = \'");
    query = concatena(query, ano);
    query = concatena(query, "\' GROUP BY MES");
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);
    return retornoJson;
}

char * mostrarInvestimento(char * post)
{
    ListCampo post_data = convertObj(post);

    char * query = "SELECT * FROM TAB_INVESTIMENTO WHERE USU_ID = ";
    query = concatena(query, post_data.campos[0].valor);
    Linhas retorno = bd(query);

    char * retornoJson = convertJSON(retorno);
    return retornoJson;
}
