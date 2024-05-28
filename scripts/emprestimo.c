#include "emprestimo.h"
#include <stdio.h>
#include "../controller/bd.h"
#include <stdlib.h>
#include <time.h>

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

    return  "{\"mensagem\":\"emprestimo cadastrado\"}";

}