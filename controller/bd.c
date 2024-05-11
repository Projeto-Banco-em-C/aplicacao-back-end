#include <stdio.h>
#include "../lib/sqlite3.h"
#include <windows.h>
#include <libgen.h>
#include "bd.h"

Linhas linhas_retorno;

//função de retorno para o sqlite3_exec
int sqlite3_retorno(void *NotUsed, int argc, char **argv, char **coluna){
    linhas_retorno.tamanho++;
    linhas_retorno.list_campos = (ListCampo*) realloc(linhas_retorno.list_campos, linhas_retorno.tamanho * sizeof(ListCampo));

    int i = linhas_retorno.tamanho - 1;

    linhas_retorno.list_campos[i].campos = (Campo *) malloc(argc * sizeof(Campo));
    linhas_retorno.list_campos[i].tamanho = argc;
    for (int j = 0; j < argc; j++) {
        printf("%s = %s\n", coluna[j], argv[j] ? argv[j] : "NULL");
        linhas_retorno.list_campos[i].campos[j].key = strdup(coluna[j]);
        linhas_retorno.list_campos[i].campos[j].valor = strdup(argv[j] ? argv[j] : "NULL");
    }
    //printf("\n");
    return 0;
}

/**
 * <h3>Descrição:</h3>
 * Arquivo bd é o arquivo de conexão com  o banco de dados.
 * Responsável por conectar com o banco de dados através da biblioteca sqlite3.h e disponibilizar comando para uso do banco de dados.
 */
Linhas bd(char * query){
    //conexção com o banco de dados
    sqlite3 * db = 0;
    char * mensagem_erro = 0;
    linhas_retorno.tamanho = 0;
    linhas_retorno.list_campos = (ListCampo *)malloc(0);

    printf("Versao %s \n", sqlite3_libversion());

    //Pegar arquivo do banco de dados:
    char path[1000];
    GetCurrentDirectory(1000, path);
    char *parent_dir = dirname(path); // obter o diretório pai
    strcat(parent_dir, "\\BancoDeDados.db"); // adicionar o nome do arquivo do banco de dados

    int rc = sqlite3_open(parent_dir, &db);

    if(rc != SQLITE_OK){
        printf("ERRO ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return linhas_retorno;
    }
    printf("Banco de dados conectado com sucesso...\n\n");


    //Tratamento do Codigos SQL
    rc = sqlite3_exec(db, query, sqlite3_retorno, 0, &mensagem_erro);

    if ( rc != SQLITE_OK ){
        printf( "ERRO ao consultar usuario: %s\n", sqlite3_errmsg(db) );
        sqlite3_close(db);
        return linhas_retorno;
    }
    printf( "Consulta executada com sucesso.\n" );

    sqlite3_close(db);

    return linhas_retorno;
}