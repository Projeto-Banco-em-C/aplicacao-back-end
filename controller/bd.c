#include <stdio.h>
#include "../lib/sqlite3.h"
#include <windows.h>
#include <libgen.h>
#include "bd.h"

Linhas linhas_retorno; // Variavel que armazena os dados retornados do banco de dados

/**
 * Função executada para cada linha do retorno do sqlite3_exec
 */
int sqlite3_retorno(void *NotUsed, int argc, char **argv, char **coluna){
    linhas_retorno.tamanho++; // Incrementa a quantidade de linhas
    linhas_retorno.list_campos = (ListCampo*) realloc(linhas_retorno.list_campos, linhas_retorno.tamanho * sizeof(ListCampo)); // Realoca a memoria para caber mais uma list_campos

    int i = linhas_retorno.tamanho - 1; // ultima posição do list_campos

    linhas_retorno.list_campos[i].campos = (Campo *) malloc(argc * sizeof(Campo)); // Aloca a memoria para os campos da ultima list_campos
    linhas_retorno.list_campos[i].tamanho = argc;//Armazena a quantidade de campos
    for (int j = 0; j < argc; j++) { // Loop para armazenar todas as colunas da tabela em seus respectivos campos
        printf("%s = %s\n", coluna[j], argv[j] ? argv[j] : "NULL");
        linhas_retorno.list_campos[i].campos[j].key = strdup(coluna[j]);
        linhas_retorno.list_campos[i].campos[j].valor = strdup(argv[j] ? argv[j] : "NULL");
    }
    return 0;
}

/**
 * Função Responsável por conectar com o banco de dados através da biblioteca sqlite3.h e encapsula toda a interação com o banco de dados
 * @param query Comando SQL que será enviado para o banco de dados
 * @return Retorna uma struct Linhas com todas as linhas que retornaram da requisição do banco de dados
 */
Linhas bd(char * query){
    //Inicialização das variaveis
    sqlite3 * db = 0;
    char * mensagem_erro = 0;
    linhas_retorno.tamanho = 0;
    linhas_retorno.list_campos = (ListCampo *)malloc(0);

    printf("Versao %s \n", sqlite3_libversion()); // Exibe a versão da biblioteca utilizada

    //Pegar arquivo do banco de dados:
    char path[1000];
    GetCurrentDirectory(1000, path);
    char *parent_dir = dirname(path); // ObteM o diretório pai
    strcat(parent_dir, "\\BancoDeDados.db"); // Adicionar o nome do arquivo do banco de dados ao path

    int rc = sqlite3_open(parent_dir, &db); // Abre a conexão com o banco de dados

    if(rc != SQLITE_OK){
        printf("ERRO ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return linhas_retorno;
    }
    printf("Banco de dados conectado com sucesso...\n\n");


    // Tratamento do Codigos SQL
    rc = sqlite3_exec(db, query, sqlite3_retorno, 0, &mensagem_erro);

    if ( rc != SQLITE_OK ){
        printf( "ERRO ao consultar usuario: %s\n", sqlite3_errmsg(db) );
        sqlite3_close(db);
        return linhas_retorno;
    }
    printf( "Consulta executada com sucesso.\n" );

    sqlite3_close(db); // Fecha a conexão com o banco de dados

    return linhas_retorno;
}