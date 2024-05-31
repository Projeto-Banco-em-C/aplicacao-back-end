#include "emprestimo.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "transferencia.h"

char * adicionar_emprestimo(char * post)
{

    ListCampo post_data = convertObj(post);
    char * query = "INSERT INTO TAB_EMPRESTIMO (USU_ID, EMP_VALOR_TOTAL, EMP_NUM_PARCELAS, EMP_JURUS_MENSAL, EMP_DATA_INICIO) VALUES (";
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

    char * query02 = "UPDATE TAB_USUARIO SET USU_LIM_EMPRESTIMO = USU_LIM_EMPRESTIMO - ";
    query02 = concatena(query02, post_data.campos[1].valor);
    query02 = concatena(query02, " WHERE USU_ID = ");
    query02 = concatena(query02, post_data.campos[0].valor);
    retorno = bd(query02);
    

    char * query03 = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO + ";
    query03 = concatena(query03, post_data.campos[1].valor);
    query03 = concatena(query03, " WHERE USU_ID = ");
    query03 = concatena(query03, post_data.campos[0].valor);
    retorno = bd(query03);


    return  "{\"mensagem\":\"emprestimo cadastrado\"}";

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
    char * query = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO*1.0027 WHERE USU_SALDO < 0";
    Linhas retorno = bd(query);

    // Pega todos que tem algum emprestimo ativo
    char * query1 = "SELECT USU_ID, (EMP_VALOR_TOTAL/EMP_NUM_PARCELAS + EMP_JUROS_MENSAL) AS MENSALIDADE, EMP_ID FROM TAB_EMPRESTIMO WHERE EMP_NUM_PARCELAS_PAGAS < EMP_NUM_PARCELAS AND EMP_COBRADO <> ";
    query1 = concatena(query1,post_data.campos[0].valor);
    retorno = bd(query1);

    if(strcmp(post_data.campos[0].valor,"15") == 0){
        for (int i = 0; i < retorno.tamanho; i++) {
            // Faz a cobrança
            /*
            char * query2 = "UPDATE TAB_USUARIO SET USU_SALDO = USU_SALDO - ";
            query2 = concatena(query2,retorno.list_campos[i].campos[1].valor);
            query2 = concatena(query2," WHERE USU_ID = ");
            query2 = concatena(query2,retorno.list_campos[i].campos[0].valor);
            Linhas retorno1 = bd(query2);
            */
            // Faz a cobrança
            char * json = "{\"USU_ID_ORIGEM\":\"";
            json = concatena(json,retorno.list_campos[i].campos[0].valor);
            json = concatena(json,"\",\"USU_ID_DESTINO\":\"1\",\"VALOR\":\"");
            json = concatena(json,retorno.list_campos[i].campos[1].valor);
            json = concatena(json,"\",\"TIPO\":\"EMPRESTIMO\"}");
            transferir(json);


            // Atualiza o emprestimo
            char * query3 = "UPDATE TAB_EMPRESTIMO SET EMP_VALOR_PAGO = (EMP_VALOR_PAGO + EMP_VALOR_TOTAL/EMP_NUM_PARCELAS), EMP_NUM_PARCELAS_PAGAS = EMP_NUM_PARCELAS_PAGAS + 1 WHERE EMP_ID = ";
            query3 = concatena(query3,retorno.list_campos[i].campos[2].valor);
            Linhas retorno1 = bd(query3);
        }
    }

    return "{\"mensagem\":\"ok\"}";
}