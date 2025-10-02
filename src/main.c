#include <trees.h>

int main(){
    char* test_words[] = {
    "test",
    "team", 
    "tea",
    "teach",
    "teacher",
    "teaming",
    NULL
    };
    rtree *trie = rtree_create();

    
    
    for(int i = 0; test_words[i]!=NULL;i++)
    {
        rtree_insert(trie,test_words[i],i);
    }
    
    rtree_print(trie,0);
    rtree_free(trie);
    return 0;
}