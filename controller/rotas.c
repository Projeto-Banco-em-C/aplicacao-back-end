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
#include "../scripts/extrato.h"
#include "../scripts/transferencia.h"
#include "../scripts/emprestimo.h"
#include "../scripts/investimento.h"

/**
 * Função que recebe os dados da API e trata eles e os direciona para as devidas funções
 * @param url caminho para o qual deve ser enviado as informações do JSON
 * @param post_data JSON com as informações que devem ser enviadas para a devida função
 * @return Retorna um json que será enviado como resposta da requisição da API
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

        // If para definir a rota que foi recebida e a função que deve ser chamada
        if (strcmp(url, "/login") == 0) {
            page1 = login(post_data);
        } else if (strcmp(url, "/cadastro") == 0) {
            page1 = cadastro(post_data);
        } else if (strcmp(url, "/extrato") == 0) {
            page1 = extrato(post_data);
        } else if (strcmp(url, "/investimento") == 0) {
            page1 = investimento(post_data);
        } else if (strcmp(url, "/mostrarInvestimento") == 0) {
            page1 = mostrarInvestimento(post_data);
        } else if (strcmp(url, "/atualiza_emprestimo") == 0) {
            page1 = atualiza_emprestimo(post_data);
        } else if (strcmp(url, "/validacaoCpf") == 0) {
            page1 = validacaoCpf(post_data);
        } else if (strcmp(url, "/consulta_info_conta") == 0) {
            page1 = consulta_info_conta(post_data);
        } else if (strcmp(url, "/transferir") == 0) {
            page1 = transferir(post_data);
        } else if (strcmp(url, "/consulta_info_pix") == 0) {
            page1 = consulta_info_pix(post_data);
        } else if (strcmp(url, "/list_historico_pix") == 0) {
            page1 = list_historico_pix(post_data);
        } else if (strcmp(url, "/adicionar_chave_pix") == 0) {
            page1 = adicionar_chave_pix(post_data);
        } else if (strcmp(url, "/dados") == 0) {
            page1 = dados(post_data);
        } else if (strcmp(url, "/adicionar_contato_pix") == 0) {
            page1 = adicionar_contato_pix(post_data);
        }else if (strcmp(url, "/lista_chave_pix") == 0) {
            page1 = lista_chave_pix(post_data);
        }else if (strcmp(url, "/adicionar_emprestimo") == 0) {
            page1 = adicionar_emprestimo(post_data);
        }else if (strcmp(url, "/lista_emprestimo") == 0) {
            page1 = lista_emprestimo(post_data);
        }else if (strcmp(url, "/removeInvestimento") == 0) {
            page1 = removeInvestimento(post_data);
        }else if (strcmp(url, "/atualizaInvestimentos") == 0) {
            page1 = atualizaInvestimentos(post_data);
        }else if (strcmp(url, "/mostraGanhos") == 0) {
            page1 = mostraGanhos(post_data);
        } else {
            page1 = "{\"erro\": \"caminho não encontrado\"}";
        }
    }
    return page1;
}

