#include <stdio.h>
#include "rotas.h"
#include <string.h>
#include "bd.h"
#include <stdlib.h>
#include "../scripts/cadastro.h"
#include "../scripts/validacaoCpf.h"
#include "../scripts/login.h"
#include "../scripts/ted.h"
#include "../scripts/pix.h"
#include "../scripts/dados.h"

char * end_point_1(char * post){ // Não precisa estar nesse arquivo
    //Exemplo
    char * query = "SELECT * FROM TAB_USUARIO";
    Linhas retorno = bd(query);

    printf("%s - ",retorno.list_campos[0].campos[0].key);
    printf("%s",retorno.list_campos[0].campos[0].valor);

    char * json = convertJSON(retorno);

    return json;
}

char * end_point_2(char * post){ // Não precisa estar nesse arquivo
    //Exemplo
    return "{\"status\":\"ok\"}";
}

/**
 * Função que recebe os dados da API e trata eles e os direciona para as devidas funções
 * @param url caminho para o qual deve ser enviado as informações do JSON
 * @param post_data JSON com as informações que devem ser enviadas para a devida função
 * @return
 */
char * criaRotas(const char * url, char * post_data){
    char * page1;
    ListCampo post;
    if(post_data != NULL) {
        int i = 0;
        while(post_data[i] != '}'){
            i++;
        }

        post_data[i+1] = '\0'; // Adiciona o caracter de fimd e string no JSON

        removeChar(post_data,'\"'); // Remove as aspas
        //post = convertObj(post_data); // converte o JSON para ListCampos
        if (strcmp(url, "/login") == 0) { // Cada end point tem um if
            page1 = login(post_data);
        } else if (strcmp(url, "/cadastro") == 0) {
            page1 = cadastro(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/ted") == 0) {
            page1 = end_point_1(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/pagamento") == 0) {
            page1 = end_point_1(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/extrato") == 0) {
            page1 = end_point_1(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/investimento") == 0) {
            page1 = end_point_1(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/emprestimo") == 0) {
            page1 = end_point_1(post_data); // Esse é um exemplo
        } else if (strcmp(url, "/validacaoCpf") == 0) {
            page1 = validacaoCpf(post_data);
        } else if (strcmp(url, "/consulta_info_conta") == 0) {
            page1 = consulta_info_conta(post_data);
        } else if (strcmp(url, "/transferir_ted") == 0) {
            page1 = transferir_ted(post_data);
        } else if (strcmp(url, "/verifica_saldo") == 0) {
            page1 = verifica_saldo(post_data);
        } else if (strcmp(url, "/consulta_info_pix") == 0) {
            page1 = consulta_info_pix(post_data);
        } else if (strcmp(url, "/list_historico_pix") == 0) {
            page1 = list_historico_pix(post_data);
        } else if (strcmp(url, "/adicionar_chave_pix") == 0) {
            page1 = adicionar_chave_pix(post_data);
        }else if (strcmp(url, "/transferir_pix") == 0) {
            page1 = transferir_pix(post_data);
        } else if (strcmp(url, "/dados") == 0) {
            page1 = dados(post_data);
        } else {
            page1 = "{\"erro\": \"caminho não encontrado\"}";
        }
    }
    return page1;
}

