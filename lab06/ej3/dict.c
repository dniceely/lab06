#include <assert.h>
#include <stdlib.h>
#include "dict.h"
#include "key_value.h"

struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};

static bool invrep(dict_t d) {
    bool inv = true;
    if (d == NULL)
    {
        inv = true;
    }
    else if (d->left == NULL && d->right == NULL)
    {
        inv = true;
    }
    

     else if (d->left != NULL && d->right == NULL)
    {
        inv = invrep(d->left);
    }
    else if (d->left == NULL && d->right != NULL)
    {
        inv = invrep(d->right);
    }
    else                                                    // ninguno es vacío, veo las propieades y las llamadas recursivas
    {
        bool less = key_less(d->left->key,d->key);
        bool more = !key_less(d->right->key,d->key);
        bool rgt = invrep(d->right);
        bool lft = invrep(d->left);
        inv = (less && more && rgt && lft);
    }
    return inv;
}

dict_t dict_empty(void) {
    dict_t dict = NULL;
    assert(invrep(dict) && (dict_length(dict) == 0));
    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(invrep(dict));
    dict_t new_node = malloc(sizeof (struct _node_t));
    new_node->key = word;
    new_node->value = def;
    new_node->left = NULL;
    new_node->right = NULL;
    if (dict ==  NULL)
    {
        dict = new_node;
    }
    else
    {
        dict_t aux = dict;
        bool check = true;
        while (check)
        {
            if (key_eq(word, aux->key))
            {
                aux->key = word;
                aux->value = def;
                return dict;
            }
            else if (key_less(aux->key, word) && aux->right != NULL)
            {
                aux = aux->right;
            }
            else if (key_less(word, aux->key) && aux->left != NULL)
            {
                aux = aux->left;
            }
            else
            {
                check = false;
            }
        }
        if (key_less(word,aux->key) && !key_eq(word,aux->key))
        {
            aux->left = new_node;
        }
        else if (key_less(aux->key,word) && !key_eq(word,aux->key))
        {
            aux->right = new_node;
        }
        
    }
    assert(invrep(dict) && dict_exists(dict,word));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    key_t def=NULL;              
    assert(invrep(dict));
    if (dict == NULL)
    {
        def = NULL;
    }
    
    else if (key_eq(dict->key, word))
    {
        def = dict->value;
    }
    else if (key_less(word, dict->key))
    {
        def = dict_search(dict->left, word);
    }
    else if (key_less(dict->key, word))
    {
        def = dict_search(dict->right, word);
    }

    assert((def != NULL) == dict_exists(dict,word));
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(invrep(dict));
    bool exists = false;
    if (dict == NULL)
    {
        exists = false;
    }

    else if (key_eq(dict->key, word))
    {
        exists = true;
    }
    else if (key_less(word, dict->key))
    {
        exists = dict_exists(dict->left, word);
    }
    else
    {
        exists = dict_exists(dict->right, word);
    }

    assert(invrep(dict));
    return exists;
}

unsigned int dict_length(dict_t dict) {
    unsigned int length=0;
    assert(invrep(dict));
    if (dict == NULL)
    {
        return length;
    }
    
    length = dict_length(dict->left) + dict_length(dict->right) + 1;
    assert(invrep(dict));
    return length;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(invrep(dict));
    if (dict == NULL)
    {
        return dict;
    }


    if (key_less(word, dict->key))
    {
        dict->left = dict_remove(dict->left, word);
    }
    else if (key_less(dict->key, word))
    {
        dict->right = dict_remove(dict->right, word);
    }
    else
    {
        if (dict->left == NULL && dict->right == NULL)
        {
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = NULL;
        }
        else if (dict->left == NULL)
        {
            dict_t aux = dict->right;
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = aux;
        }
        else if (dict->right == NULL)
        {
            dict_t aux = dict->left;
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = aux;
        }
        else
        {
            // este es el caso donde el elemento es igual
            dict_t aux = dict->right;
            while (aux->left != NULL)
            {
                aux = aux->left;
            }
            dict->key = aux->key;
            dict->value = aux->value;
            aux->key = key_destroy(aux->key);
            aux->value = value_destroy(aux->value);
            free(aux);
            aux = NULL;
        }
    
    }
    assert(invrep(dict) && !dict_exists(dict, word));
    return dict;
}

dict_t dict_remove_all(dict_t dict) {
    assert(invrep(dict));
    if (dict == NULL)
    {
        return dict;
    }
    else if (dict->left != NULL)
    {
        dict->left = dict_remove_all(dict->left);
    }
    else if (dict->right != NULL)
    {
        dict->right = dict_remove_all(dict->right);
    }
    dict->key = key_destroy(dict->key);
    dict->value = value_destroy(dict->value);
    free(dict);
    dict = NULL;

    assert(invrep(dict) && (dict_length(dict) == 0));
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(invrep(dict));
    if (dict != NULL)
    {
        key_dump(dict->key, file);
        value_dump(dict->value, file);
        dict_dump(dict->left, file);
        dict_dump(dict->right, file);
    }
    
}

dict_t dict_destroy(dict_t dict) {
    assert(invrep(dict));
    if (dict == NULL)
    {
        return NULL;
    }
    dict->key = key_destroy(dict->key);
    dict->value = value_destroy(dict->value);
    dict->left = dict_destroy(dict->left);
    dict->right = dict_destroy(dict->right);
    free(dict);
    dict = NULL;
    assert(dict == NULL);
    return dict;
}

