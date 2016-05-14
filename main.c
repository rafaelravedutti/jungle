#include <stdio.h>
#include <stdlib.h>
//--
#include <jungle.h>
#include <tree.h>

/* Compara dois inteiros */
int intcmp(const void *xptr, const void *yptr) {
    int x, y;

    /* Carrega os valores dos endereços nas variáveis */
    x = *((int *) xptr);
    y = *((int *) yptr);

    /* Retorna o valor da comparação */
    return  (x < y) ? -1 :
            (x > y) ?  1 : 0;
}

/* Realiza uma operação na floresta (árvore de árvores) */
void do_operation(struct jungle **j, char op, int *data, unsigned int length, int maxvalue) {
    struct tree *t = NULL;

    /* Se os dados não forem nulos e a operação não for a de listar os conjuntos,
     * ordenamos o vetor com os valores do conjunto e criamos a árvore representada 
     * pelo mesmo                                                                    */
    if(data != NULL && op != '*') {
        qsort(data, length, sizeof(int), intcmp);
        t = make_tree(data, length);
    }

    /* Verifica qual a operação a ser executada */
    switch(op) {
        case '+':
            /* Insere a árvore na floresta */
            *j = insert_tree(*j, t);
            break;
        case '-':
            /* Remove a árvore da floresta */
            *j = delete_tree(*j, t);
            break;
        case '=':
            /* A árvore existe na floresta? */
            if(!tree_exists(*j, t)) {
                fprintf(stdout, "E\n");
            }
            break;
        case '/':
            /* Remove os subconjuntos da árvore */
            delete_jungle_supertrees(*j, *j, t);
            break;
        case '<':
            /* Imprime os subconjuntos da árvore */
            print_jungle_subtrees(*j, t);
            break;
        case '>':
            /* Imprime os superconjuntos da árvore */
            print_jungle_supertrees(*j, t);
            break;
        case '*':
            /* Imprime os conjuntos da árvore */
            print_jungle(*j);
            break;
        default:
            /* Caso a operação seja desconhecida */
            fprintf(stdout, "Operação inválida: '%c'\n", op);
    }

    /* Se a operação não for a de adicionar árvore devemos então liberar
     * o espaço de memória ocupado pois ele não será mais usado          */
    if(op != '+') {
        free_tree(t);
    }

    /* Imprime um '0' após o fim da operação */
    fprintf(stdout, "0\n");
}

/* Função principal */
int main(int argc, const char *argv[]) {
    struct jungle *j = NULL;
    int *data;
    int maxvalue, n, i;
    char op;

    /* Lê o valor máximo do conjunto */
    if(!fscanf(stdin, "%d", &maxvalue)) {
        return -1;
    }

    /* Na próxima linha, lê o caractere que representa a operação */
    if(!fscanf(stdin, "\n%c", &op)) {
        return -1;
    }

    /* Enquanto a operação não for '0' e ainda existir dados na entrada, continua
     * lendo as linhas e executando as operações                                    */
    while(op != '0' && !feof(stdin)) {

        /* Inicializa o apontador de dados com o valor nulo */
        data = NULL;

        if(op != '*') {
            /* Lê a quantidade de elementos do conjunto */
            fscanf(stdin, "%d", &n);
        }

        /* Se a quantidade for menor ou igual a zero, o programa não faz nada e vai
         * para a próxima linha                                                     */
        if(n > 0) {
            /* Aloca memória para armazenar o conjunto de dados */
            data = (int *) malloc(sizeof(int) * n);

            /* Lê todos os elementos do conjunto e os armazena no espaço alocado */
            for(i = 0; i < n; ++i) {
                fscanf(stdin, "%d", data + i);
            }
        }

        /* Realiza a operação indicada */
        do_operation(&j, op, data, n, maxvalue);

        if(data != NULL) {
            /* Libera a memória que foi usada para o conjunto */
            free(data);
        }

        /* Vai até a próxima linha */
        while(fgetc(stdin) != '\n');

        /* Na próxima linha, lê o novo operador */
        fscanf(stdin, "%c", &op);
    }

    /* Libera todo o espaço ocupado pela floresta */
    if(j != NULL) {
        free_jungle(j);
    }

    return 0;
}
