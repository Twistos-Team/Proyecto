#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    void* key;
    void * value;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->key = key;
    new->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *tree_map = (TreeMap *) malloc(sizeof(TreeMap));
    tree_map->lower_than = lower_than;
    tree_map->current = NULL;
    tree_map->root = NULL;
    return tree_map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;
    TreeNode *aux = tree->root;
    TreeNode *parent = NULL;

    while (aux != NULL) {
        parent = aux;
        if (tree->lower_than(key, aux->key)) {
            aux = aux->left;
        }
        else if (tree->lower_than(aux->key, key)) {
            aux = aux->right;
        }
        else {
            return;
        }
    }
    TreeNode *nuevo = createTreeNode(key, value);
    nuevo->parent = parent;
    if (parent == NULL) {
        tree->root = nuevo;
    }
    else if (tree->lower_than(nuevo->key, parent->key)) {
        parent->left = nuevo;
    }
    else {
        parent->right = nuevo;
    }

    tree->current = nuevo;
}


TreeNode * minimum(TreeNode * x){
    while (x != NULL) {
        if (x->left == NULL) {
            return x;
        }
        x = x->left;
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if ((node->left == NULL)&&(node->right == NULL)) {
        if (tree->lower_than(node->key, node->parent->key)) {
            node->parent->left = NULL;
        }
        else {
            node->parent->right = NULL;
        }
        free(node);
        return;
    }
    else if ((node->left == NULL)||(node->right == NULL)) {
        if (node->left != NULL) {
            if (node->parent == NULL) {
                node->left->parent = NULL;
                tree->root = node;
                free(node);
                return;
            }
            if (tree->lower_than(node->left->key, node->parent->key)) {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            else {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        if (node->right != NULL) {
            if (node->parent == NULL) {
                tree->root = node;
                node->right->parent = NULL;
                free(node);
                return;
            }
            if (tree->lower_than(node->right->key, node->parent->key)) {
                node->parent->right = node->left;
                node->right->parent = node->parent;
            }
            else {
                node->parent->left = node->left;
                node->right->parent = node->parent;
            }
        }
        free(node);
        return;
    }
    if ((node->left != NULL)&&(node->right != NULL)) {
        TreeNode *aux = minimum(node->right);
        node->value = aux->value;
        node->key = aux->key;
        if (aux->left != NULL) {
            node->right->left = aux->left;
        }
        else {
            node->right->left = aux->left;
        }
        removeNode(tree, aux);
    }
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


void * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL) {
        if (tree->lower_than(key, aux->key)) {
            aux = aux->left;
        }
        else if (tree->lower_than(aux->key, key)) {
            aux = aux->right;
        }
        else {
            tree->current = aux;
            return aux->key;
        }
    }
    return NULL;
}


void * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;
    TreeNode *ub = aux;

    while (aux != NULL) {
        if (tree->lower_than(key, aux->key)) {
            ub = aux;
            aux = aux->left;
        }
        else if (tree->lower_than(aux->key, key)) {
            ub = aux;
            aux = aux->right;
        }
        else {
            return aux->value;
        }
    } 
    if (tree->lower_than(ub->key, key)) {
        if (ub->right != NULL) {
            return minimum(ub->right)->value;
        }
        while (ub->parent != NULL) {
            if (tree->lower_than(ub->key, ub->parent->key)) {
                return ub->parent->value;
            }
            ub = ub->parent;
        }
        if (tree->lower_than(ub->key, key)) return NULL;
    }
    else return ub->value;
    return NULL;
}


void * firstTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL) {
        aux = aux->left;
        if (aux->left == NULL) {
            return aux->value;
        }
    }
    return NULL;
}


void * nextTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->current;

    if (aux != NULL) {
        if (aux->right != NULL) {
            tree->current = minimum(aux->right);
            return tree->current->value;
        }
        while (aux->parent != NULL) {
            if (tree->lower_than(aux->key, aux->parent->key)) {
                tree->current = aux->parent;
                return aux->parent->value;
            }
            aux = aux->parent;
        }
        if (tree->lower_than(aux->key, tree->current->key)) return NULL;
    }
    return NULL;
}
