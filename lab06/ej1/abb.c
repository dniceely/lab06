#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "abb.h"

struct _s_abb {
    abb_elem elem;
    struct _s_abb *left;
    struct _s_abb *right;
};

static bool elem_eq(abb_elem a, abb_elem b) {
    return a == b;
}

static bool elem_less(abb_elem a, abb_elem b) {
    return a < b;
}

    
static bool invrep(abb tree) {
    bool inv = true;
    if (tree == NULL)
    {
        inv = true;
    }
    else if (tree->left == NULL && tree->right == NULL)
    {
        inv = true;
    }
    

     else if (tree->left != NULL && tree->right == NULL)
    {
        inv = invrep(tree->left);
    }
    else if (tree->left == NULL && tree->right != NULL)
    {
        inv = invrep(tree->right);
    }
    else                                                    // ninguno es vacío, veo las propieades y las llamadas recursivas
    {
        bool less = elem_less(tree->left->elem,tree->elem);
        bool more = !elem_less(tree->right->elem,tree->elem);
        bool rgt = invrep(tree->right);
        bool lft = invrep(tree->left);
        inv = (less && more && rgt && lft);
    }
    return inv;
}



abb abb_empty(void) {
    abb tree;
    tree = NULL;
    assert(invrep(tree) && abb_is_empty(tree));
    return tree;
}



abb abb_add(abb tree, abb_elem e) {
    assert(invrep(tree));
    struct _s_abb  *node = malloc(sizeof(struct _s_abb));
    node->elem = e;
    node->left = NULL;
    node->right = NULL;
    if (abb_is_empty(tree))
    {
        tree = node;
    }
    else
    {
        abb aux = tree;
        bool check = true;
        while (check)
        {
            if (elem_less(aux->elem,e) && aux->right != NULL)
            {
                aux = aux->right;
            }
            else if (elem_less(e,aux->elem) && aux->left != NULL)
            {
                aux = aux->left;
            }
            else
            {
                check = false;
            }    
        }
        if (elem_less(e,aux->elem) && !elem_eq(e,aux->elem))
        {
            aux->left = node;
        }
        else if (elem_less(aux->elem,e) && !elem_eq(e,aux->elem))
        {
            aux->right = node;
        }

    }
    assert(invrep(tree) && abb_exists(tree, e));
    return tree;
}

bool abb_is_empty(abb tree) {
    assert(invrep(tree));
    bool is_empty=false;
    is_empty = (tree == NULL);
    return is_empty;
}

bool abb_exists(abb tree, abb_elem e) {
    assert(invrep(tree));
    bool exists=false;
    if (abb_is_empty(tree))
    {
        exists = false;
    }
    

    else if (elem_eq(tree->elem, e))
    {
        exists = true;
    }
    else if (elem_less(e,tree->elem))
    {
        exists = abb_exists(tree->left, e);
    }
    else
    {
        exists = abb_exists(tree->right, e);
    }
    
    return exists;
}

unsigned int abb_length(abb tree) {
    unsigned int length=0;
    assert(invrep(tree));
    if (abb_is_empty(tree))
    {
        return length;
    }
    
    length = abb_length(tree->left) + abb_length(tree->right) + 1;
    assert(invrep(tree) && (abb_is_empty(tree) || length > 0));
    return length;
}

abb abb_remove(abb tree, abb_elem e) {
    assert(invrep(tree));
    if (abb_is_empty(tree))
    {
        return tree;
    }


    if (elem_less(e,tree->elem))
    {
        tree->left = abb_remove(tree->left, e);
    }
    else if (!elem_less(e,tree->elem) && !elem_eq(e,tree->elem))
    {
        tree->right = abb_remove(tree->right, e);
    }
    else
    {
        if (tree->left == NULL && tree->right == NULL)
        {
            free(tree);
            return NULL;
        }
        else if (tree->left == NULL)
        {
            abb node = tree->right;
            free(tree);
            return node;
        }
        else if (tree->right == NULL)
        {
            abb node = tree->left;
            free(tree);
            return node;
        }
        else
        {
            // si llegue hasta aca ocurrio que "e" es igual a tree->elem
            abb nuevaraiz = tree->right;
            while (nuevaraiz->left != NULL)
            {
                nuevaraiz = nuevaraiz->left;
            }
            tree->elem = nuevaraiz->elem;
            tree->right = abb_remove(tree->right, nuevaraiz->elem);
        }
        
    }
    
    assert(invrep(tree) && !abb_exists(tree, e));
    return tree;
}


abb_elem abb_root(abb tree) {
    assert(invrep(tree) && !abb_is_empty(tree));
    abb_elem root;
    root = tree->elem;

    assert(abb_exists(tree, root));
    return root;
}

abb_elem abb_max(abb tree) {
    abb_dump(tree);
    assert(invrep(tree) && !abb_is_empty(tree));
    abb_elem max_e = tree->elem;
if (tree->right == NULL) {
        return max_e;
    } else {
        return abb_max(tree->right);
    }
    assert(invrep(tree) && abb_exists(tree, max_e));
    return max_e;
}

abb_elem abb_min(abb tree) {
    abb_elem min_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    if (tree->left == NULL)
    {
        min_e = tree->elem;
        return min_e;
    }
    
    min_e = abb_min(tree->left);

    assert(invrep(tree) && abb_exists(tree, min_e));
    return min_e;
}

void abb_dump(abb tree) {
    assert(invrep(tree));
    if (!abb_is_empty(tree)) {
        abb_dump(tree->left);
        printf("%d ", tree->elem);
        abb_dump(tree->right);
    }
}

abb abb_destroy(abb tree) {
    assert(invrep(tree));
    if (tree == NULL) {
        return NULL;
    }
    tree->left=abb_destroy(tree->left);
    tree->right= abb_destroy(tree->right);
    free(tree);
    tree=NULL;
    assert(tree == NULL);
    return tree;
}
