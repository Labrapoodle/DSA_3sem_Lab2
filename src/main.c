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
    char filename[] = "list_of_words.txt";
    char **wordsArray= (char **)malloc(WORDS_AMOUNT*sizeof(char*));
    FILE *f = fopen(filename,"r");
    for(int j = 0; j<WORDS_AMOUNT;j++)
    {
        wordsArray[j] = (char *)malloc(MAX_WORD_LENGTH*sizeof(char));
        fgets(wordsArray[j],MAX_WORD_LENGTH,f);
        wordsArray[j][strcspn(wordsArray[j],"\n")]=0;
        //printf("%d word is: %s\n",j+1,wordsArray[j]);
    }

    fclose(f);
    
    rtree *trie = rtree_create();

    
    
    for(int i = 0; i<WORDS_AMOUNT;i++)
    {
        //printf("%d: %s\n",i,wordsArray[i]);
        rtree_insert(trie,wordsArray[i],i);

        if((i+1)%10000==0)
        {
            
            double averTime = 0;
            for(int iter = 0; iter<ITERATIONS_FOR_AVERAGE; iter++)
            {
                int randomIndex = getrand(0,i-1);
                double t = wtime();
                rtree_lookup(trie,wordsArray[randomIndex]);
                averTime += wtime()-t;
            }
            averTime /= ITERATIONS_FOR_AVERAGE;
            
            printf("i+1: %d, Avertime: %.12f\n",i+1,averTime);
        }
        
    }
    
    

    
    //if(res==NULL) printf("res is NULL\n");
    //else if(res->ifNodeHasValue==hasValue) rtree_free(res);
    

    

    //rtree_print(trie,0);
    rtree_free(trie);

    for(int k=0;k<WORDS_AMOUNT;k++)
    {
        free(wordsArray[k]);
    }
    free(wordsArray);

    return 0;
}