cmake_minimum_required(VERSION 3.26)
project(aplicacao_back_end C)

set(CMAKE_C_STANDARD 11)

include_directories(lib\\libmicrohttpd\\include)
link_directories(lib\\libmicrohttpd\\lib)

add_executable(aplicacao_back_end main.c lib/sqlite3.c
        controller/rotas.c
        controller/rotas.h
        controller/API.c
        controller/API.h
        controller/JSON.c
        controller/JSON.h
        controller/bd.c
        controller/bd.h
        scripts/cadastro.c
        scripts/cadastro.h
        scripts/pix.c
        scripts/pix.h
        scripts/validacaoCpf.c
        scripts/validacaoCpf.h
        scripts/login.c
        scripts/login.h
        scripts/dados.c
        scripts/dados.h
        scripts/extrato.c
        scripts/extrato.h
        scripts/ted.c
        scripts/ted.h
        scripts/transferencia.c
        scripts/transferencia.h
)

target_link_libraries(aplicacao_back_end microhttpd ws2_32)

