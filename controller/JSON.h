/**
 * Estrura para armazenar um campo, possui os seguintes informações:
 * <ul>
 * <li><b>char * key:</b> Nome do campo</li>
 * <li><b>char * valor:</b> Valor do campo</li>
 * </ul>
 */
typedef struct {
    char *key;
    char *valor;
} Campo;

/**
 * Estrura para armazenar uma lista de campos, possui os seguintes informações:
 * <ul>
 * <li><b>int tamanho:</b> tamanho da lista de campos</li>
 * <li><b>Campo * campos:</b> lista de campos</li>
 * </ul>
 */
typedef struct {
    int tamanho;
    Campo * campos;
}ListCampo;

/**
 * Estrura para armazenar linhas do banco de dados, sendo que cada linha
 * é uma lista de campos, possui os seguintes informações:
 * <ul>
 * <li><b>int tamanho:</b> numero de linhas</li>
 * <li><b>ListCampo * list_campos:</b> linhas do banco</li>
 * </ul>
 */
typedef struct{
    int tamanho;
    ListCampo * list_campos;
}Linhas;

char * concatena(char *data1, char *data2);
char * convertJSON(Linhas linhas_json);
void removeChar(char *str, char charToRemove);
ListCampo convertObj(char *obj);
