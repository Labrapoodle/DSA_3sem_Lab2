#include <trees.h>



rtree *rtree_create()
{
    rtree *node = (rtree *)malloc(sizeof(*node));
    if(node == NULL){
        printf("Error with node initialization");
        return NULL;
    }
    node->ifNodeHasValue = noValue;
    
    node->childs = rbtree_create();

    return node;
}

rtree *get_child(rtree *node, char *literal)
{
    struct rbnode *searchResult = rbtree_lookup(node->childs->root,node->childs->nil,literal);
    if(searchResult==node->childs->nil)
    {
        return NULL;
    }
    else{
        return searchResult->value;
    }
}


void set_child(rtree *node, char *literal, rtree *child)
{
    
    
    char *s = strdup(literal);
    rbtree_add(node->childs, s, child);
}

rtree *rtree_insert(rtree *root,char *key, uint32_t value)
{
    char *substrStart = key; //Указывает на первый символ в префиксе
    char *substrEnd = key; //Указывает на последний символ в префиксе
    rtree *node = root;

    

    //while(*substrEnd!='\0')
    //{
         //= get_child(node,key);

        //char oneSymbPrefix[2];
        //strncpy(oneSymbPrefix,key,1);
        //oneSymbPrefix[1]='\0'; 
        //rtree *child = get_child(node,oneSymbPrefix);
//
        ////Случай 1: Первый символ ключа не ведёт ни к одному потомку
        //if(child==NULL)
        //{
        //    rtree *newChild = rtree_create();
        //    newChild->ifNodeHasValue = hasValue;
        //    
        //    newChild->value = value;
        //    set_child(node,key,newChild);
        //    return newChild;
        //}

        int prefixLength = substrEnd-substrStart+1;

        //Поиск наидлиннейшего уже существующего префикса
        rtree *child = NULL;
        while(*substrEnd!='\0' )
        {
                       
            
            prefixLength = substrEnd-substrStart+1;
            char subToSearch[prefixLength+1];
            strncpy(subToSearch,key+(substrStart-key),prefixLength);
            subToSearch[prefixLength] = '\0';
            child = get_child(node,subToSearch);     
            
            if(child == NULL)
            {
                rtree *newChild = rtree_create();
                newChild->ifNodeHasValue = hasValue;
                newChild->value = value; 
            }
            else
            {
                

                struct rbnode *linkByRBT = rbtree_lookup(node->childs->root,node->childs->nil,subToSearch);
                if(linkByRBT == node->childs->nil) exit;

                if(strstr(key,linkByRBT->key)!= NULL)
                {
                    substrEnd = key + strlen(linkByRBT->key);
                    substrStart = substrEnd;
                    node = child;
                }
                else
                {
                    substrEnd++;
                    substrStart = substrEnd;
                }
            }
        }
        //Случай 2: длина ключей совпала :
        if(*(substrEnd)=='\0' && child != NULL)
        {
            child->value = value;
            child->ifNodeHasValue = hasValue;
                       
            return NULL;
        }
        
        substrEnd--;
        prefixLength = substrEnd-substrStart+1;
        
        char *longestPrefix = (char *)malloc((prefixLength+1)*sizeof(char));
        strncpy(longestPrefix,key,prefixLength);
        longestPrefix[prefixLength] = '\0';
        child = get_child(node,longestPrefix);
        
        // Случай 3, префикс вставляем и двигаемся дальше
        
        if(child == NULL)
        {
            free(longestPrefix);
            rtree_free(root);
            printf("Error in insert func, child is null (but it shouldn't)\n");
            printf("%s\n",key);
            
            exit(EXIT_FAILURE);
        }
        rtree *newChild = rtree_create();
        rtree *newForExisting  = rtree_create();
        newChild->ifNodeHasValue = hasValue;
        newChild->value = value;

        struct rbnode *link = rbtree_lookup(node->childs->root,node->childs->nil,longestPrefix);
        char *childKeyPref = strdup(link->key+prefixLength);
        if(strlen(link->key)==prefixLength)
        {
            set_child(child,key+prefixLength,newChild);
            free(childKeyPref);
            free(longestPrefix);
            rtree_free(newForExisting);
            return newChild;
            
        }
        free(link->key);
        link->key = strdup(longestPrefix);

        


        if(child->ifNodeHasValue==hasValue)
        {       
            newForExisting->ifNodeHasValue=hasValue;
            newForExisting->value = child->value;
            child->ifNodeHasValue = noValue;    
        }
        else
        {
            rbtree_free(newForExisting->childs);
            newForExisting->childs = child->childs;
            child->childs = rbtree_create();
        
        }
        
        

        
        

        

        int suffLength = strlen(key)-prefixLength;
        char *suffix = (char *)malloc((suffLength+1)*sizeof(char)); //abcdef
        strncpy(suffix,key+prefixLength,suffLength);
        suffix[suffLength] = '\0';

        set_child(child,childKeyPref,newForExisting);
        set_child(child, suffix,newChild);

        
        //set_child(node,longestPrefix,link->value);
        //node->
        free(childKeyPref);
        free(longestPrefix);
        free(suffix);
        

        
        
        
        
        //child = get_child(node,longestPrefix);
        substrEnd++;  
        substrStart = substrEnd;      

    //}
    printf("-----\n");
    rtree_print(root,0);
    printf("-----\n");
    return node;
}

rtree *rtree_lookup(rtree *root, char *key)
{

    return root;
}


void rtree_print(rtree *root, int level) {
    if (root == NULL) {
        return;
    }
    
    // Отступ для уровня
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    // Информация о текущем узле
    printf("Node: ");
    if (root->ifNodeHasValue == hasValue) {
        printf("VALUE=%u", root->value);
    } else {
        printf("INTERNAL");
    }
    
    // Информация о количестве детей
    if (root->childs != NULL && root->childs->root != root->childs->nil) {
        printf(" [children present]");
    } else {
        printf(" [no children]");
    }
    printf("\n");
    
    // Рекурсивно печатаем детей
    if (root->childs != NULL) {
        // Функция для обхода RBtree
        void print_children(struct rbnode *node, struct rbnode *nil, int lvl) {
            if (node == nil) return;
            
            print_children(node->left, nil, lvl);
            
            for (int i = 0; i < lvl; i++) printf("  ");
            printf("Child key: \"%s\" -> ", node->key ? node->key : "(null)");
            
            if (node->value != NULL) {
                printf("\n");
                rtree_print(node->value, lvl + 1);
            } else {
                printf("NULL\n");
            }
            
            print_children(node->right, nil, lvl);
        }
        
        print_children(root->childs->root, root->childs->nil, level + 1);
    }
}
    

void rtree_free(rtree *root){
    if(root == NULL) return;
    if(root->childs!=NULL) rbtree_free(root->childs);
    free(root);
    
    
    
}


