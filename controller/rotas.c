#include "rotas.h"
#include <string.h>

char * end_point_1(char * post_data){ // Não precisa estar nesse arquivo
    //Exemplo
    return "{\"status\":\"ok\"}";
}

char * end_point_2(char * post_data){ // Não precisa estar nesse arquivo
    //Exemplo
    return "{\"status\":\"ok\"}";
}

char * criaRotas(const char * url, char * post_data){
    char * page1;
    if (strcmp(url, "/end_point_1") == 0) {
        page1 = end_point_1(post_data);
    } else if (strcmp(url, "/end_point_2") == 0) {
        page1 = end_point_2(post_data);
    } else {
        page1 = "{\"erro\": \"caminho não encontrado\"}";
    }
    return page1;
}

