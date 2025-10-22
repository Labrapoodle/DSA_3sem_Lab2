#include <trees.h>
#define FILENAME "list_of_words"
#define WORDSAMOUNT 100000
#define MAXWORDLENGTH 30

int main(){

    char wordsArray[WORDSAMOUNT][MAXWORDLENGTH];
    FILE *f = fopen(FILENAME,'r');
    for(int j = 0; j<)


    char *test_words[] = {
    "test", "tea","team", NULL
};
    rtree *trie = rtree_create();

    
    
    for(int i = 0; test_words[i]!=NULL;i++)
    {
        rtree_insert(trie,test_words[i],i);
        
    }
    
    

    rtree_delete(trie,"tea");
    //if(res==NULL) printf("res is NULL\n");
    //else if(res->ifNodeHasValue==hasValue) rtree_free(res);
    

    

    rtree_print(trie,0);
    rtree_free(trie);
    return 0;
}