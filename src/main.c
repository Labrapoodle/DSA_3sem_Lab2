#include <trees.h>

int main(){
    rtree *trie = rtree_create();

    char s[] = "Lemon";

    rtree_insert(trie,s,5);
    rtree_print(trie,0);
    rtree_free(trie);
    return 0;
}