#include <rbtree.h>
#include <rtree.h>

int main(){
    rtree *trie = rtree_create();
    rtree_free(trie);
    return 0;
}