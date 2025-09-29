#ifndef RBTREE_H
#define RBTREE_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct RBtree RBtree;

typedef enum{
    noValue,
    hasValue,
} Mark;


typedef struct{
    uint32_t value;
     
    RBtree *childs;
    Mark ifNodeHasValue;
} rtree;

struct rbnode {
    struct rbnode* parent;
    struct rbnode* left;
    struct rbnode* right;
    char color; // RED=0, BLACK=1
    rtree *value;
    char *key;
};

struct RBtree{
    struct rbnode* root;
    struct rbnode* nil;
};

//Red-black tree
RBtree *rbtree_create();
void rb_add_fixup(RBtree *T,struct rbnode *z);
void left_rotate(RBtree *T, struct rbnode *x);
void right_rotate(RBtree *T, struct rbnode *x);
void rbtree_transplant(RBtree *T, struct rbnode *u,struct rbnode *v);
void rb_delete_fixup(RBtree *T,struct rbnode *x);

struct rbnode *rbtree_add(RBtree *root,char *key, rtree *value);
struct rbnode *rbtree_lookup(struct rbnode *node,struct rbnode *nil, char *key);
struct rbnode *rbtree_delete(RBtree *T, char *key);
struct rbnode *rbtree_min(struct rbnode *node,struct rbnode *nil);
struct rbnode *rbtree_max(struct rbnode *node,struct rbnode *nil);
void rbtree_free(RBtree *T);
void recursion_free(struct rbnode *root,struct rbnode *nil);
void rbtree_print_dfs(struct rbnode *root, struct rbnode *nil, int level);

int rbtree_height(struct rbnode *node, struct rbnode *nil);


//rtree:
rtree *rtree_create();
rtree *rtree_insert(rtree *root,char *key, uint32_t value);
//rtree *rtree_lookup(rtree *root, char *key);
//rtree *rtree_delete(rtree *root, char *key);
void rtree_print(rtree *root, int level);

rtree *get_child(rtree *node, char *literal);
void set_child(rtree *node, char *literal, rtree *child);
void rtree_free(rtree *root);
#endif