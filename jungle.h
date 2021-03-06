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

#ifndef __JUNGLE_H__
#define __JUNGLE_H__

/* Árvore */
struct tree {
  unsigned int tKey;
  struct tree *tLeft, *tRight;
  unsigned int tSize;
};

/* Floresta (árvore de árvores) */
struct jungle {
  unsigned int jKey, jCount;
  struct tree **jTree;
  struct jungle *jParent, *jLeft, *jRight;
};

/* Mescla duas listas de árvores de dois nodos */
void merge_jungle_nodes(struct jungle *dest, struct jungle *source);

/* Encontra um nodo da floresta a partir de sua chave */
struct jungle *find_jungle_node(struct jungle *j, unsigned int key);

/* Insere um nodo na floresta */
struct jungle *insert_jungle_node(struct jungle *j, struct jungle *node);

/* Obtém o menor valor da floresta */
struct jungle *jungle_minimum(struct jungle *j);

/* Transplanta um nodo da floresta para o local de outro nodo */
struct jungle *jungle_transplant(struct jungle *j, struct jungle *dest, struct jungle *source);

/* Remove um nodo da floresta */
struct jungle *delete_jungle_node(struct jungle *j, struct jungle *node);

/* Imprime todos os nodos da floresta */
void print_jungle(struct jungle *j);

/* Imprime as subárvores de uma árvore que pertencem a floresta */
void print_jungle_subtrees(struct jungle *j, struct tree *t);

/* Imprime as superárvores de uma árvore que pertencem a floresta */
void print_jungle_supertrees(struct jungle *j, struct tree *t);

/* Delete as superárvores de um nodo */
int delete_jungle_supertrees(struct jungle *node, struct jungle *j, struct tree *t);

/* Libera a memória ocupada por uma floresta */
void free_jungle(struct jungle *j);

#endif // __JUNGLE_H__
