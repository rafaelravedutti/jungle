/*
 * jungle - Tree of trees implementation with sets
 *
 * Copyright (C) 2014  Rafael Ravedutti Lucio Machado
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdbool.h>
//--
#include <jungle.h>

#ifndef __TREE_H__
#define __TREE_H__

/* Calcula o hash de uma árvore */
unsigned int hash(struct tree *t);

/* Cria uma árvore a partir de um vetor ordenado */
struct tree *make_tree(int *data, unsigned int length);

/* Encontra um nodo da árvore a partir de sua chave */
struct tree *find_tree_node(struct tree *t, unsigned int key);

/* Insere uma árvore na floresta */
struct jungle *insert_tree(struct jungle *j, struct tree *t);

/* Remove uma árvore da floresta */
struct jungle *delete_tree(struct jungle *j, struct tree *t);

/* Compara duas árvores */
bool tree_cmp(struct tree *t1, struct tree *t2);

/* Encontra a posição de uma árvore na lista do nodo */
int find_node_tree_pos(struct jungle *node, struct tree *t);

/* Verifica se uma árvore existe em uma floresta */
bool tree_exists(struct jungle *j, struct tree *t);

/* Verifica se uma árvore é subárvore de outra */
bool is_subtree(struct tree *t1, struct tree *t2);

/* Imprime uma árvore */
void print_tree(struct tree *t);

/* Libera a memória ocupada por uma árvore */
void free_tree(struct tree *t);

#endif // __TREE_H__
