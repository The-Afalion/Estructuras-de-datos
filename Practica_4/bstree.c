#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"
#include "music.h"

/* START */
typedef struct _BSTNode {
  void *info;
  struct _BSTNode *left;
  struct _BSTNode *right;
} BSTNode;
/* END */

/* START */
struct _BSTree {
  BSTNode *root;
  P_ele_print print_ele;
  P_ele_cmp cmp_ele;
};
/* END */

/* BSTNode TAD private functions */
void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp);
int _tree_countLongSongs_rec(BSTNode *node, int min_duration);
void *_bst_find_min_rec(BSTNode *pn);
void *_bst_find_max_rec(BSTNode *pn);
Bool _bst_contains_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp_ele);
Status _bst_insert_rec(BSTNode **pn, const void *elem, P_ele_cmp cmp_ele);
BSTNode *_bst_remove_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp_elem);
BSTNode *_bst_node_new() {
  BSTNode *pn = NULL;

  pn = malloc(sizeof(BSTNode));
  if (!pn) {
    return NULL;
  }

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

  if (!pn) {
    return count;
  }

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

  if (!pn) {
    return count;
  }

  count += _bst_postOrder_rec(pn->left, pf, print_ele);
  count += _bst_postOrder_rec(pn->right, pf, print_ele);
  count += print_ele(pf, pn->info);

  return count;
}

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

int _tree_countLongSongs_rec(BSTNode *node, int min_duration) {
  int count = 0;

  if (!node) {
    return 0;
  }

  count += _tree_countLongSongs_rec(node->left, min_duration);
  if (music_getDuration((const Music *)node->info) > min_duration) {
    count++;
  }
  count += _tree_countLongSongs_rec(node->right, min_duration);

  return count;
}

/**
 * @brief Recursive private function that removes an element from the BST.
 *
 * Implements the algorithm described in the practice statement:
 *   - If the current node is NULL, the element is not in the tree.
 *   - If elem is smaller than the current info, recurse on the left subtree.
 *   - If elem is greater than the current info, recurse on the right subtree.
 *   - If elem matches the current info, remove the node attending to the
 *     number of children:
 *       * No children: free the node and return NULL.
 *       * Only right child: keep the right child and free the node.
 *       * Only left child: keep the left child and free the node.
 *       * Two children: copy the smallest info of the right subtree into
 *         the current node and recursively remove that smallest info from
 *         the right subtree.
 *
 * @param pn Pointer to the current tree node.
 * @param elem Pointer to the element to remove.
 * @param cmp_elem Pointer to the comparison function.
 *
 * @return Pointer to the node that remains in the current position
 * (NULL when the node has been removed and had no replacement).
 */
BSTNode *_bst_remove_rec(BSTNode *pn, const void *elem, P_ele_cmp cmp_elem) {
  BSTNode *ret_node = NULL;
  void *aux_info = NULL;
  int cmp;

  if (!pn || !elem || !cmp_elem) {
    return pn;
  }

  cmp = cmp_elem(elem, pn->info);

  if (cmp < 0) {
    pn->left = _bst_remove_rec(pn->left, elem, cmp_elem);
  } else if (cmp > 0) {
    pn->right = _bst_remove_rec(pn->right, elem, cmp_elem);
  } else {
    /* Element found: remove this node */
    if (!pn->left && !pn->right) {
      /* Case 1: leaf node, no children */
      _bst_node_free(pn);
      return NULL;
    } else if (!pn->left) {
      /* Case 2: only right child */
      ret_node = pn->right;
      _bst_node_free(pn);
      return ret_node;
    } else if (!pn->right) {
      /* Case 3: only left child */
      ret_node = pn->left;
      _bst_node_free(pn);
      return ret_node;
    } else {
      /* Case 4: two children. Replace info with the smallest one in the
         right subtree and remove that smallest from the right subtree. */
      aux_info = _bst_find_min_rec(pn->right);
      pn->info = aux_info;
      pn->right = _bst_remove_rec(pn->right, aux_info, cmp_elem);
      return pn;
    }
  }

  return pn;
}

/**
 * @brief Recursive private function that collects every element of the BST
 * whose info is in the closed interval [min, max] into a list.
 *
 * The traversal is in-order, with smart pruning: we only visit the left
 * subtree if the current node is greater than min, and we only visit the
 * right subtree if the current node is smaller than max. Thanks to this
 * pruning, branches outside the range are never explored, and the resulting
 * list is sorted by the comparison function.
 *
 * @param node Pointer to the current tree node.
 * @param min Pointer to the lower bound element.
 * @param max Pointer to the upper bound element.
 * @param list Pointer to the destination list.
 * @param cmp Pointer to the comparison function used by the tree.
 */
void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp) {
  int cmp_min, cmp_max;

  if (!node || !list || !cmp) {
    return;
  }

  cmp_min = cmp(node->info, min);
  cmp_max = cmp(node->info, max);

  /* Only descend left if the current node is strictly greater than min,
     because every node in the left subtree is smaller than the current. */
  if (cmp_min > 0) {
    _tree_rangeSearch_rec(node->left, min, max, list, cmp);
  }

  /* If the current node is inside [min, max], add it to the list. */
  if (cmp_min >= 0 && cmp_max <= 0) {
    list_pushBack(list, node->info);
  }

  /* Only descend right if the current node is strictly smaller than max,
     because every node in the right subtree is greater than the current. */
  if (cmp_max < 0) {
    _tree_rangeSearch_rec(node->right, min, max, list, cmp);
  }
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_ele_print print_ele, P_ele_cmp cmp_ele) {
  BSTree *tree;

  if (!print_ele || !cmp_ele) {
    return NULL;
  }

  tree = malloc(sizeof(BSTree));
  if (!tree) {
    return NULL;
  }

  tree->root = NULL;
  tree->print_ele = print_ele;
  tree->cmp_ele = cmp_ele;

  return tree;
}

void tree_destroy(BSTree *tree) {
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
  if (!f || !tree) {
    return -1;
  }

  return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

void *tree_find_min(BSTree *tree) {
  if (!tree) {
    return NULL;
  }

  return _bst_find_min_rec(tree->root);
}

void *tree_find_max(BSTree *tree) {
  if (!tree) {
    return NULL;
  }

  return _bst_find_max_rec(tree->root);
}

Bool tree_contains(BSTree *tree, const void *elem) {
  if (!tree) {
    return FALSE;
  }

  return _bst_contains_rec(tree->root, elem, tree->cmp_ele);
}

Status tree_insert(BSTree *tree, const void *elem) {
  if (!tree) {
    return ERROR;
  }

  return _bst_insert_rec(&(tree->root), elem, tree->cmp_ele);
}


Status tree_remove(BSTree *tree, const void *elem) {
  if (!tree || !elem) {
    return ERROR;
  }

  tree->root = _bst_remove_rec(tree->root, elem, tree->cmp_ele);
  return OK;
}


List *tree_rangeSearch(const BSTree *tree, void *min, void *max) {
  List *list = NULL;

  if (!tree || !min || !max) {
    return NULL;
  }

  /* If min > max according to the tree's comparison function, the range is
     empty: return an empty list rather than an error so the caller can
     safely call list_size / list_print. */
  list = list_new();
  if (!list) {
    return NULL;
  }

  if (tree->cmp_ele(min, max) > 0) {
    return list;
  }

  _tree_rangeSearch_rec(tree->root, min, max, list, tree->cmp_ele);
  return list;
}

int tree_countLongSongs(const BSTree *tree, int min_duration) {
  if (!tree || min_duration < 0) {
    return ERROR_I;
  }

  return _tree_countLongSongs_rec(tree->root, min_duration);
}
