#ifndef RTREE_H
#define RTREE_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <rbtree.h>







rtree *rtree_create();
rtree *rtree_insert(rtree *root,char *key, uint32_t value);
//rtree *rtree_lookup(rtree *root, char *key);
//rtree *rtree_delete(rtree *root, char *key);
void rtree_print(rtree *root, int level);

rtree *get_child(rtree *node, char *literal);
void set_child(rtree *node, char *literal, rtree *child);
void rtree_free(rtree *root);

#endif