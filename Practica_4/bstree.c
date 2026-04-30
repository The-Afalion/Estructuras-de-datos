#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"
#define root(tree) 	((tree)->root)
#define cmp(tree) 	((tree)->cmp_ele)
#define print(tree) ((tree)->print_ele)
#define info(pn) 	((pn)->info)
#define left(pn) 	((pn)->left)
#define right(pn) 	((pn)->right)
#define free_n(pn)	_bst_node_free(pn)

/* START [_BSTNode] */
typedef struct _BSTNode {
void *info;
struct _BSTNode *left;
struct _BSTNode *right;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree {
BSTNode *root;
P_ele_print print_ele;
P_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp);
int _tree_countLongSongs_rec(BSTNode *node, int min_duration);

BSTNode *_bst_node_new() {
	BSTNode *pn = NULL;

	pn = malloc(sizeof(BSTNode));
	if (!pn) return NULL;

	pn->left = NULL;
	pn->right = NULL;
	pn->info = NULL;

	return pn;
}

void _bst_node_free(BSTNode *pn) {
	if (!pn) {
		return;
	}

	free(pn);
}

void _bst_node_free_rec(BSTNode *pn) {
	if (!pn) {
		return;
	}

	_bst_node_free_rec(pn->left);
	_bst_node_free_rec(pn->right);
	_bst_node_free(pn);

	return;
}

int _bst_depth_rec(BSTNode *pn) {
	int depth_l, depth_r;

	if (!pn) {
		return 0;
	}

	depth_l = _bst_depth_rec(pn->left);
	depth_r = _bst_depth_rec(pn->right);

	if (depth_r > depth_l) {
		return depth_r + 1;
	} else {
		return depth_l + 1;
	}
}

int _bst_size_rec(BSTNode *pn) {
	int count = 0;

	if (!pn) return count;

	count += _bst_size_rec(pn->left);
	count += _bst_size_rec(pn->right);

	return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
	int count = 0;

	if (!pn) {
		return count;
	}

	count += print_ele(pf, pn->info);
	count += _bst_preOrder_rec(pn->left, pf, print_ele);
	count += _bst_preOrder_rec(pn->right, pf, print_ele);

	return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
	int count = 0;

	if (!pn) {
		return count;
	}

	count += _bst_inOrder_rec(pn->left, pf, print_ele);
	count += print_ele(pf, pn->info);
	count += _bst_inOrder_rec(pn->right, pf, print_ele);

	return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
	int count = 0;

	if (!pn) return count;

	count += _bst_postOrder_rec(pn->left, pf, print_ele);
	count += _bst_postOrder_rec(pn->right, pf, print_ele);
	count += print_ele(pf, pn->info);

	return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_ele_print print_ele, P_ele_cmp cmp_ele) {
	BSTree *tree;

	if (!print_ele || !cmp_ele) {
		return NULL;
	}

	tree = malloc(sizeof(BSTree));
	if (!tree) return NULL;

	tree->root = NULL;
	tree->print_ele = print_ele;
	tree->cmp_ele = cmp_ele;

	return tree;
}

void tree_destroy(BSTree *tree) 
{
	if (!tree) {
		return;
	}

	_bst_node_free_rec(tree->root);
	free(tree);

	return;
}

Bool tree_isEmpty(const BSTree *tree) {
	if (!tree || !tree->root) {
		return TRUE;
	}
	return FALSE;
}

int tree_depth(const BSTree *tree) {
	if (!tree) {
		return -1;
	}

	return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree) {
	if (!tree) {
		return -1;
	}

	return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree) {
	if (!f || !tree) return -1;

	return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree) {
	if (!f || !tree) return -1;

	return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree) {
	if (!f || !tree) return -1;

	return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

/**** TODO: find_min, find_max, insert, contains, remove ****/



void *_bst_find_min_rec(BSTNode *pn) {
  if (!pn) {
    return NULL;
  }

  if (!pn->left) {
    return pn->info;
  }

  return _bst_find_min_rec(pn->left);
}

void *_bst_find_max_rec(BSTNode *pn) {
  if (!pn) {
    return NULL;
  }

  if (!pn->right) {
    return pn->info;
  }

  return _bst_find_max_rec(pn->right);
}

Bool _bst_contains_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp_ele) {
  int cmp;

  if (!pn || !elem || !cmp_ele) {
    return FALSE;
  }

  cmp = cmp_ele(elem, pn->info);
  if (cmp == 0) {
    return TRUE;
  }

  if (cmp < 0) {
    return _bst_contains_rec(pn->left, elem, cmp_ele);
  }

  return _bst_contains_rec(pn->right, elem, cmp_ele);
}

Status _bst_insert_rec(BSTNode **pn, const void *elem, P_ele_cmp cmp_ele) {
  BSTNode *new_node = NULL;
  int cmp;

  if (!pn || !elem || !cmp_ele) {
    return ERROR;
  }

  if (!(*pn)) {
    new_node = _bst_node_new();
    if (!new_node) {
      return ERROR;
    }

    new_node->info = (void *)elem;
    *pn = new_node;
    return OK;
  }

  cmp = cmp_ele(elem, (*pn)->info);
  if (cmp == 0) {
    return OK;
  }

  if (cmp < 0) {
    return _bst_insert_rec(&((*pn)->left), elem, cmp_ele);
  }

  return _bst_insert_rec(&((*pn)->right), elem, cmp_ele);
}

/**
 * Pseudocodigo:
 * 
 * 	if ( elemento > info(pn) ) then 
 * 			_bst_remove_rec (right(pn), elemento, cmp_elem);   BUScamos por la derecha
 * 	else if ( elemento < info(pn) ) then 
 * 			_bst_remove_rec (left(pn), elemento, cmp_elem);   BUScamos por la izquierda
 *  if ( elemento == info(pn) ) then  
 * 									Acá está la chicha de todo el asunto.
 * 								El nodo en el que estamos, puede o no tener hijos
 * 								puede tener un hijo izquierdo,  un hijo derecho, dos hijos,
 * 								o ninguno y ser una hoja.  Por este último deberiamos de empoezar,
 * 								pues es más fácil de manejralo, veamos que se puede hacer:
 * 
 * 		if(!left(pn) && !right(pn))  then tenemos una hoja
 * 			BTNode *nodo_aux = pn;
 * 			free_node(pn);
 * 			pn = NULL;
 * 			return nodo_aux;
 * 		if(!left(pn) && right(pn)) then   Tenemos un hijo a la derecha
 * 			
 * 			
 * 
 * 
 * 
 * 
 */
BSTNode *_bst_remove_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp_elem)
{
	BSTNode *node_rest = NULL;
	if (!elem || !cmp_elem) return NULL; /*Control de Errores*/
	if (!pn) return NULL; /*CASO BASE:  si no hay puntero a nodo, pasamos NULL*/

	/*Buscamos por la derecha pues elem es más grande*/
	if (cmp_elem(elem, info(pn)) > 0)		_bst_remove_rec(right(pn), elem, cmp_elem); 
	/*Buscamos por la izquierda pues elem es más pequeño*/
	else if (cmp_elem(elem, info(pn)) < 0)	_bst_remove_rec(left(pn), elem, cmp_elem);
	/*Si son iguales, entonces hay que ver  que tipo de nodo es (Hoja, intermedio)*/
	else if (cmp_elem(elem, info(pn)) == 0)
	{
		/*EL nodo es una hoja*/
		if (!left(pn) && !right(pn))
		{
			_bst_node_free(pn);
			return NULL;
		}
		/*El nodo solo tiene una rama izquierda*/
		else if (left(pn) && !right(pn)) 
		{
			node_rest = left(pn);
			_bst_node_free(pn);
			return node_rest;
		}
		/*El nodo solo tiene una rama derecha*/
		else if (!left(pn) && right(pn))
		{
			node_rest = right(pn);
			_bst_node_free(pn);
			return node_rest; /*Devolvemos la rama reestante pue se mantiene el orden*/
		}
		else /*Ambos hijos están*/
		{
			node_rest =	_bst_find_min_rec(right(pn));
			info(pn) =	info(node_rest);
			right(pn) =	_bst_remove_rec(right(pn), info(pn), cmp_elem);
			return pn;
		}
		
	}
	
	return pn;
}


/**
 *	Pseudocidog: 
 *           
 *			
 *   if (
 * 			No existe tree o 
 * 			Tree está vacío o 
 * 		) then retorna error;
 *  if ( 
 * 		No hay elemento a comparar  o
 * 		No existe en el árbol
 *  ) then OK;  
 * 		La función tiene como objetivo remover alo de un arbol, para ello necsesita que exista el arbol, pues ahí buscara, pero 
 * 		que el "algo" exista, es contingente, si lo hace, su trabajo es quitarlo sin dejar errores, pero si no existe o no está en el árbol, 
 * 		ya su trabajo esta hecho
 *  
 *   Retornamos : si Hemos encontrado el elemento, lo removemos , lo imprimimos y luego decimos OK, en otro caso, decimos ERROR
 * 		 
 * 		Primero inspeccionamos recursivamente a través de todas las ramas, comparando info con element
 * 		la recursión nos hace inspeccionar los nodos intermedios de la rama izquierda (si existe) y eugo los nodos intermedios de la rama derecha (si existe)
 * 		por cada nodo se reitera este proceso.
 * 
 * 
 * 
 * 
 *
*/
Status tree_remove(BSTree *tree, const void *elem)
{
	/*First pass a exist tree please*/
	/*A element inexistent is and is not in anylocation, is NULL element is and is not same time*/
	if(!tree || tree_isEmpty(tree) == TRUE) return ERROR; 
	if(!elem || tree_contains(tree, elem) == FALSE) return OK; /*so, is OK*/

	return ((print(tree)(stdout,_bst_remove_rec(root(tree), elem, cmp(tree)) )) != 0 ? OK : FALSE);
}
