#include <stdio.h>
#include <stdlib.h>
//--
#include <jungle.h>
#include <tree.h>

/* Mescla duas listas de árvores de dois nodos */
void merge_jungle_nodes(struct jungle *dest, struct jungle *source) {
    unsigned int t, i, exists;
    int fpos;

    /* Percorre todas as árvores da lista do nodo fonte */
    for(t = 0; t < source->jCount; ++t) {
        fpos = -1;
        exists = 0;

        /* Percorre todas as árvores da lista do nodo destino */
        for(i = 0; i < dest->jCount; ++i) {
            /* Se não tiver nenhuma árvore na posição, então escolhe esta
             * posição como posição livre para adicionar a árvore do nodo
             * fonte                                                         */
            if(!dest->jTree[i]) {
                fpos = i;
            /* Se a árvore já está na lista, então imprime um código de erro */
            } else if(!tree_cmp(dest->jTree[i], source->jTree[t])) {
                exists = 1;
                fprintf(stdout, "E\n");
            }
        }

        /* Se a árvore não existe, adiciona ela na lista do nodo destino */
        if(!exists) {
            /* Se não existe uma posição livre, realoca a lista adicionando
             * a posição em que a árvore será colocada                      */
            if(fpos == -1) {
                dest->jTree = (struct tree **) realloc(dest->jTree, sizeof(struct tree *) * (++dest->jCount));
                dest->jTree[dest->jCount - 1] = source->jTree[t];
            /* Se existe uma posição livre, adiciona a árvore nesta posição */
            } else {
                dest->jTree[fpos] = source->jTree[t];
            }
        }
    }

    /* Remove o nodo fonte */
    source->jCount = 0;
    free(source->jTree);
    free(source);
}

/* Encontra um nodo da floresta a partir de sua chave */
struct jungle *find_jungle_node(struct jungle *j, unsigned int key) {
    while(j != NULL && j->jKey != key) {
        if(j->jKey > key) {
            j = j->jLeft;
        } else {
            j = j->jRight;
        }
    }

    return j;
}

/* Insere um nodo na floresta */
struct jungle *insert_jungle_node(struct jungle *j, struct jungle *node) {
    struct jungle *jptr;

    /* Define como nulos os nodos relativos do nodo */
    node->jParent = node->jRight = node->jLeft = NULL;

    /* Se a árvore não existir, retorna uma árvore em que o nodo é a raiz */
    if(!j) {
        return node;
    }

    /* Percorre a árvore até encontrar o caminho para inserir o nodo */
    for(jptr = j; jptr != NULL; ) {
        /* Define o ultimo nodo valido encontrado como pai do nodo */
        node->jParent = jptr;

        /* Se a chave do nodo atual for maior que a do nodo, vai para esquerda */
        if(jptr->jKey > node->jKey) {
            jptr = jptr->jLeft;
        /* Se a chave do nodo atual for menor que a do nodo, vai para a direita */
        } else if(jptr->jKey < node->jKey) {
            jptr = jptr->jRight;
        /* Se as chaves forem iguais, mesclam as árvores dos dois nós */
        } else {
            merge_jungle_nodes(jptr, node);
            return j;
        }
    }

    /* Define o nodo como filho do ultimo nodo valido encontrado */
    if(node->jParent->jKey > node->jKey) {
        node->jParent->jLeft = node;
    } else {
        node->jParent->jRight = node;
    }

    return j;
}

/* Obtém o menor valor da floresta */
struct jungle *jungle_minimum(struct jungle *j) {
    /* Se a floresta for nula, retorna um ponteiro nulo */
    if(!j) {
        return NULL;
    }

    /* Enquanto houver filho a esquerda, prossegue para ela */
    while(j->jLeft != NULL) {
        j = j->jLeft;
    }

    return j;
}

/* Transplanta um nodo da floresta para o local de outro nodo */
struct jungle *jungle_transplant(struct jungle *j, struct jungle *dest, struct jungle *source) {
    struct jungle *root;

    /* Define como j a raiz da árvore (valor retornado posteriormente) */
    root = j;

    /* Se o nodo destino for a raiz, define o nodo fonte como a raiz */
    if(dest->jParent == NULL) {
        root = source;
    /* Se o nodo destino for o filho da esquerda, define o nodo fonte como
     * o filho a esquerda do seu pai                                         */
    } else if(dest == dest->jParent->jLeft) {
        dest->jParent->jLeft = source;
    /* Se o nodo destino for o filho da direita, define o nodo fonte como
       o filho da direita do seu pai                                         */
    } else {
        dest->jParent->jRight = source;
    }

    /* Se o nodo fonte for diferente de nulo, define seu pai como o pai do
     * nodo destino                                                          */
    if(source != NULL) {
        source->jParent = dest->jParent;
    }

    /* Retorna a raiz da árvore após o transplante */
    return root;
}

/* Remove um nodo da floresta */
struct jungle *delete_jungle_node(struct jungle *j, struct jungle *node) {
    struct jungle *sucessor;

    /* Se o nodo for nulo, retorna a própria floresta */
    if(!node) {
        return j;
    }

    /* Se o nodo não tiver filho a esquerda, retorna a floresta com seu filho
     * a direita transplantado para o seu lugar                              */
    if(!node->jLeft) {
        return jungle_transplant(j, node, node->jRight);
    }

    /* Se o nodo não tiver filho a direita, retorna a floresta com seu filho
     * a esquerda transplantado para o seu lugar                             */
    if(!node->jRight) {
        return jungle_transplant(j, node, node->jLeft);
    }

    /* Obtem o sucessor do nodo */
    sucessor = jungle_minimum(node->jRight);

    /* Se o sucessor não for filho do nodo, transplanta o seu filho a
     * direita para o seu lugar e define o filho a direita do nodo como
     * seu filho                                                             */
    if(sucessor->jParent != node) {
        jungle_transplant(j, sucessor, sucessor->jRight);
        sucessor->jRight = node->jRight;
        sucessor->jRight->jParent = sucessor;
    }

    /* Define o filho a esquerda do nodo como o filho do sucessor */
    sucessor->jLeft = node->jLeft;
    sucessor->jLeft->jParent = sucessor;
    return jungle_transplant(j, node, sucessor);
}

/* Imprime todos os nodos da floresta */
void print_jungle(struct jungle *j) {
    unsigned int i;

    /* Se o nodo for nulo, não imprimos nada */
    if(j != NULL) {
        /* Imprime primeiro as árvores do nodo a esquerda (menores) */
        print_jungle(j->jLeft);

        /* Imprime a árvore do nodo atual */
        for(i = 0; i < j->jCount; ++i) {
            if(j->jTree[i] != NULL) {
                fprintf(stdout, "%d ", j->jTree[i]->tSize);
                print_tree(j->jTree[i]);
                fprintf(stdout, "\n");
            }
        }

        /* Imprime as árvores do nodo a direita (maiores) */
        print_jungle(j->jRight);
    }
}

/* Imprime as subárvores de um nodo */
void print_jungle_subtrees(struct jungle *j, struct tree *t) {
    unsigned int i;

    /* Se o nodo for nulo não faz nada */
    if(j != NULL) {
        /* Imprime as subárvores a esquerda (menores) do nodo */
        print_jungle_subtrees(j->jLeft, t);

        /* Percorre todas as árvores da lista do nodo */
        for(i = 0; i < j->jCount; ++i) {
            /* Se a árvore for uma subárvore do nodo, então a imprime */
            if(j->jTree[i] != NULL && t->tSize > j->jTree[i]->tSize && is_subtree(t, j->jTree[i])) {
                fprintf(stdout, "%d ", j->jTree[i]->tSize);
                print_tree(j->jTree[i]);
                fprintf(stdout, "\n");
            }
        }

        /* Imprime as subárvores a direita (maiores) do nodo */
        print_jungle_subtrees(j->jRight, t);
    }
}

/* Imprime as superárvores de um nodo */
void print_jungle_supertrees(struct jungle *j, struct tree *t) {
    unsigned int i;

    /* Se o nodo for nulo não faz nada */
    if(j != NULL) {
        /* Imprime as superárvores a esquerda (menores) do nodo */
        print_jungle_supertrees(j->jLeft, t);

        /* Percorre todas as árvores da lista do nodo */
        for(i = 0; i < j->jCount; ++i) {
            /* Se a árvore for uma superárvore do nodo, então a imprime */
            if(j->jTree[i] != NULL && t->tSize < j->jTree[i]->tSize && is_subtree(j->jTree[i], t)) {
                fprintf(stdout, "%d ", j->jTree[i]->tSize);
                print_tree(j->jTree[i]);
                fprintf(stdout, "\n");
            }
        }

        /* Imprime as superárvores a direita (maiores) do nodo */
        print_jungle_supertrees(j->jRight, t);
    }
}

/* Delete as superárvores de um nodo */
int delete_jungle_supertrees(struct jungle *node, struct jungle *j, struct tree *t) {
    unsigned int i;
    int ret;

    if(node != NULL) {
        /* Deleta as superárvores filhas do nodo recursivamente */
        ret = delete_jungle_supertrees(node->jLeft, j, t) | delete_jungle_supertrees(node->jRight, j, t);

        /* Percorre todas as árvores da lista */
        for(i = 0; i < node->jCount; ++i) {
            /* Se a árvore for uma superárvore do nodo, então a deleta */
            if(node->jTree[i] != NULL && t->tSize > j->jTree[i]->tSize && is_subtree(node->jTree[i], t)) {
                /* Libera o espaço ocupado pela superárvore */
                free_tree(node->jTree[i]);

                /* Se existia apenas essa superárvore no nodo, deleta o nodo */
                if(node->jCount == 1) {
                    delete_jungle_node(j, node);
                    free(node->jTree);
                    free(node);
                /* Caso contrário, remove a superárvore da lista de árvores */
                } else {
                    node->jTree[i] = NULL;
                }

                /* Se deletou alguma árvore, então a operação é bem sucedida */
                ret = true;
            }
        }

        return ret;
    }

    return false;
}

/* Libera a memória ocupada por uma floresta */
void free_jungle(struct jungle *j) {
    unsigned int i;

    /* Percorremos todos os nodos da floresta recursivamente e liberamos
       o espaço de memória ocupado pelos mesmos                            */
    if(j != NULL) {
        free_jungle(j->jLeft);
        free_jungle(j->jRight);

        /* Libera o espaço ocupado pelas árvores da lista do nodo */
        for(i = 0; i < j->jCount; ++i) {
            free_tree(j->jTree[i]);
        }

        /* Libera o espaço ocupado pela lista do nodo */
        free(j->jTree);
    }
}
