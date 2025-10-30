#include <trees.h>
#include <sys/time.h>
#define WORDS_AMOUNT 100000
#define MAX_WORD_LENGTH 33
#define ITERATIONS_FOR_AVERAGE 10000000


double wtime()
{
struct timeval t;
gettimeofday(&t, NULL);
return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max)
{
return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

int main(){
    char *words_array[] = {
        
        "forest",
        "for",
        
        "fort",
        "f",
        "forever",
        "fo",
        NULL
    };
    
    rtree *trie = rtree_create();

    
    
    for(int i = 0; words_array[i]!=NULL;i++)
    {
        //printf("%d: %s\n",i,wordsArray[i]);
        rtree_insert(trie,words_array[i],i);

        
        
    }
    
    
    rtree_delete(trie,"fo");
    
    //if(res==NULL) printf("res is NULL\n");
    //else if(res->ifNodeHasValue==hasValue) rtree_free(res);
    

    print_rtree(trie->childs);
    //rtree_print(trie,0);
    rtree_free(trie);
    return 0;
}