#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//--
#include <tree.h>
#include <jungle.h>

/* Calcula o hash de uma árvore */
unsigned int hash(struct tree *t) {
  return t->tSize;
}

/* Cria uma árvore a partir de um vetor ordenado */
struct tree *make_tree(int *data, unsigned int length) {
  struct tree *t;
  unsigned int half;

  /* Se o tamanho do vetor for nulo, gera um nodo nulo */
  if(!length) {
    return NULL;
  }

  /* Aloca o nodo atual a ser definido */
  t = (struct tree *) malloc(sizeof(struct tree));

  /* Se não conseguiu alocar, retorna erro */
  if(!t) {
    fprintf(stdout, "E\n");
    return NULL;
  }

  /* Define o tamanho da árvore */
  t->tSize = length;

  /* Se existir apenas um valor, ele é atribuido ao nodo e seus filhos 
   * são definidos como nulos                       */
  if(length == 1) {
    t->tKey = data[0];
    t->tLeft = t->tRight = NULL;
  /* Se existir mais valores, o valor do meio é atribuido ao nodo e seu
   * filho a esquerda é uma subárvore gerada com essa mesma operação nos
   * valores a esquerda (menores) do vetor. Seu filho a direita é uma
   * subárvore gerada também com essa operação nos valores a direita
   * (maiores) do vetor.                          */
  } else {
    half = length / 2;
    t->tKey = data[half];
    t->tLeft = make_tree(data, half);
    t->tRight = make_tree(data + half + 1, length - half - 1);
  }

  return t;
}

/* Encontra um nodo da árvore a partir de sua chave */
struct tree *find_tree_node(struct tree *t, unsigned int key) {
  while(t != NULL && t->tKey != key) {
    if(t->tKey > key) {
      t = t->tLeft;
    } else {
      t = t->tRight;
    }
  }

  return t;
}

/* Insere uma árvore na floresta */
struct jungle *insert_tree(struct jungle *j, struct tree *t) {
  struct jungle *node;

  /* Aloca o nodo da árvore */
  node = (struct jungle *) malloc(sizeof(struct jungle));

  /* Caso ocorre erro ao alocar o nodo, imprime um caractere de erro 'E' */
  if(!node) {
    fprintf(stdout, "E\n");
    return j;
  }

  /* Define as propriedades do nodo */
  node->jKey = hash(t);
  node->jCount = 1;
  node->jTree = (struct tree **) malloc(sizeof(struct tree *));
  node->jTree[0] = t;

  /* Insere o nodo na floresta */
  return insert_jungle_node(j, node);
}

/* Remove uma árvore da floresta */
struct jungle *delete_tree(struct jungle *j, struct tree *t) {
  struct jungle *node, *ret;
  unsigned int pos;

  /* Encontra o nodo o qual a árvore pertence */
  node = find_jungle_node(j, hash(t));

  /* Se o nodo não existir, a árvore não existe, então não faz nada */
  if(!node) {
    fprintf(stderr, "E\n");
    return j;
  }

  /* Encontra a posição da árvore na lista de árvores do nodo */
  pos = find_node_tree_pos(node, t);

  /* Se a árvore não estiver na lista, então não faz nada */
  if(pos == -1) {
    fprintf(stderr, "E\n");
    return j;
  }

  /* Libera o espaço de memória ocupado pela árvore */
  free_tree(node->jTree[pos]);

  /* Se existir apenas essa árvore no nodo, remove o nodo da floresta */
  if(node->jCount == 1) {
    ret = delete_jungle_node(j, node);
    free(node->jTree);
    free(node);
    return ret;
  }

  node->jTree[pos] = NULL;
  return j;
}

/* Compara duas árvores */
bool tree_cmp(struct tree *t1, struct tree *t2) {
  /* Se ambas as árvores não forem nulas */
  if(t1 != NULL && t2 != NULL) {
    /* Se os valores (chaves) dos nodos forem diferentes, retorna 1 */
    if(t1->tKey != t2->tKey) {
      return 1;
    }

    /* Se os valores (chaves) dos nodos forem iguais, vai comparando
     * os nodos até chegar as folhas, caso alguma comparação retorne 1,
     * o operador ou '|' irá fazer este 1 ser carregado por todas as
     * chamadas recursivas até o resultado final da comparação      */
    return tree_cmp(t1->tLeft, t2->tLeft) | tree_cmp(t1->tRight, t2->tRight);
  } else if(t1 == NULL && t2 == NULL) {
    /* Se ambos os nodos forem nulos, retorna 0 */
    return 0;
  }

  /* Se um nodo for nulo e o outro não, retorna 1 */
  return 1;
}

/* Encontra a posição de uma árvore na lista do nodo */
int find_node_tree_pos(struct jungle *node, struct tree *t) {
  int i;

  /* Percorre todos os elementos da lista */
  for(i = 0; i < node->jCount; ++i) {
    /* Se a árvore for igual ao elemento atual, retorna sua posição */
    if(node->jTree[i] != NULL && !tree_cmp(node->jTree[i], t)) {
      return i;
    }
  }

  return -1;
}

/* Verifica se uma árvore existe em uma floresta */
bool tree_exists(struct jungle *j, struct tree *t) {
  struct jungle *node;

  /* Encontra o nodo em que a árvore pode pertencer */
  node = find_jungle_node(j, hash(t));

  /* Se o nodo não existir, então a árvore não existe */
  if(!node) {
    return false;
  }

  /* Se o nodo não existir, procura a árvore na lista de árvores do nodo */
  return (find_node_tree_pos(node, t) != -1);
}

/* Verifica se uma árvore é subárvore de outra */
bool is_subtree(struct tree *t1, struct tree *t2) {
  /* Se a segunda árvore for nula, ela é subárvore da primeira */
  if(!t2) {
    return true;
  }

  /* Caso o elemento da segunda árvore não exista na primeira, ela não
   * é uma subárvore da mesma.                       */
  if(!find_tree_node(t1, t2->tKey)) {
    return false;
  }

  /* Recursivamente, realizamos a mesma operação com os outros elementos
   * da segunda árvore, e caso algum não esteja contido na primeira
   * árvore, então a segunda árvore não será subárvore da primeira     */
  return is_subtree(t1, t2->tLeft) & is_subtree(t1, t2->tRight);
}

/* Imprime uma árvore */
void print_tree(struct tree *t) {
  /* Se o nodo for nulo, não faz nada */
  if(t != NULL) {
    /* Imprime os elementos a esquerda (menores) */
    print_tree(t->tLeft);
    /* Imprime o elemento atual */
    fprintf(stdout, "%d ", t->tKey);
    /* Imprime os elementos a direita (maiores) */
    print_tree(t->tRight);
  }
}

/* Libera a memória ocupada por uma árvore */
void free_tree(struct tree *t) {
  /* Percorremos todos os nodos da árvore recursivamente e liberamos
     o espaço de memória ocupados pelos mesmos              */
  if(t != NULL) {
    free_tree(t->tLeft);
    free_tree(t->tRight);
    free(t);
  }
}
