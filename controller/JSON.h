typedef struct {
    char *key;
    char *valor;
} Campo;

typedef struct {
    int tamanho;
    Campo * campos;
}ListCampo;

typedef struct{
    ListCampo * list_campos;
    int tamanho;
}Linhas;

ListCampo convertObj(char *obj);
char *concatena(char *data1, char *data2);
