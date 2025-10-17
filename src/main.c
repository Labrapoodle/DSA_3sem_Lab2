#include <trees.h>

int main(){
    char *test_words[] = {
    "test", "team", "lion", NULL
};
    rtree *trie = rtree_create();

    
    
    for(int i = 0; test_words[i]!=NULL;i++)
    {
        rtree_insert(trie,test_words[i],i);
        //rtree_print(trie,0);
        //printf("\n");
    }
    
    rtree_print(trie,0);
    printf("-----\n");

    rtree *res = rtree_delete(trie,"test");
    if(res==NULL) printf("res is NULL");
    else if(res->ifNodeHasValue==hasValue) rtree_free(res);
    

    

    rtree_print(trie,0);
    rtree_free(trie);
    return 0;
}