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

    while(*substrEnd != '\0')
    {    
        char oneSymbPrefix[2];
        strncpy(oneSymbPrefix,substrStart,1);
        oneSymbPrefix[1]='\0'; 
        rtree *child = get_child(node,oneSymbPrefix); 

        // Случай 1: вставляемое ребро новое -- вставляем весь ключ
        if(child==NULL)
        {
            //printf("%s\n",substrStart);
            rtree * newChild = rtree_create();
            set_child(node,substrStart,newChild);
            newChild->value=value;
            newChild->ifNodeHasValue = hasValue;
            return newChild;
        }
        else
        {
            struct rbnode *link = rbtree_lookup(node->childs->root,node->childs->nil,oneSymbPrefix);

            // Случай 2: вставляемый ключ -- префикс существующего узла
            if(strncmp(link->key,substrStart,strlen(substrStart))==0)
            {
                // Случай 2.5: длины совпадают
                if(strlen(substrStart)==strlen(link->key))
                {
                    link->value->value = value;
                }

                
                else
                {                    
                    rtree *newChild = rtree_create();  
                    newChild->ifNodeHasValue = hasValue;
                    newChild->value = value;

                    char *childSuffix = strstr(link->key,substrStart) + strlen(substrStart);
                    set_child(newChild,childSuffix,child);
                    

                    free(link->key);
                    link->key = strdup(substrStart);
                    
                    link->value = newChild;                   

                    return newChild;
                }
            }

            //Случай 4: если существующий узел - префикс вставляемого узла
            else if(strncmp(substrStart,link->key,strlen(link->key))==0)
            {
                //printf("\tlink->key:%s\n",link->key);
                node = child;
                substrStart += strlen(link->key);
                substrEnd = substrStart;
            }

            // Случай 5: поиск общего префикса
            else
            {
                //if(substrEnd==substrStart) substrEnd++;
                int substrLength = substrEnd-substrStart+2; //+2 т.к. будем вставлять ещё и *(substrEnd+1)
                if(strcmp(key,"tea")==0)
                    {
                        printf("\tdiff %d\n",substrEnd-substrStart);
                    } 
                char *substring = (char *)malloc(sizeof(char)*(substrLength+1));
                strncpy(substring,substrStart,substrLength);
                substring[substrLength] = '\0';
                
                if(get_child(node,substring)!=NULL)
                {
                    substrEnd++;
                    
                }
                else
                {
                    
                    substring[substrLength-1] = '\0';
                    child = get_child(node,substring);

                    rtree *newChild = rtree_create();
                    char *childSuffix = strstr(link->key,substring) + strlen(substring);
                                    
                    set_child(newChild,childSuffix,child);

                    link = rbtree_lookup(node->childs->root,node->childs->nil,substring);

                    

                    free(link->key);
                    link->key = strdup(substring);
                    
                    link->value = newChild;
                    
                    
                    
                    
                    
                    
                    if(get_child(child,substrEnd+1)!=NULL)
                    {
                        
                        node = child;
                    }
                    else
                    {
                        node = newChild;
                    }
                    substrStart=++substrEnd;
                }
                free(substring);
            }
        }
    }
    return node;
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

rtree *rtree_lookup(rtree *root, char *key)
{
    
    char *ptrBegin = key;
    char *ptrEnd = key;
    rtree *node = root;
    
    while(*ptrEnd!='\0')
    {
        rtree *child;
        char oneSymbStr[2];
        oneSymbStr[0] = *ptrBegin;
        oneSymbStr[1] = '\0';
        child = get_child(node,oneSymbStr);

        if(child == NULL) return NULL;
        
        
        struct rbnode *link = rbtree_lookup(node->childs->root,node->childs->nil,oneSymbStr); 
        
        
        int lengthMinimalKey = (strlen(link->key)<strlen(ptrBegin))?strlen(link->key):strlen(ptrBegin);
        
        if(strcmp(link->key,ptrBegin)==0)
        {
            if(link->value->ifNodeHasValue==hasValue) return link->value;
            else return NULL;
        }
        else if (strlen(link->key)>=strlen(ptrBegin)) return NULL;
        else
        {
            if(strncmp(ptrBegin,link->key,strlen(link->key))==0)
            {
                ptrBegin += strlen(link->key);
                ptrEnd = ptrBegin;
                node = child;
            }
            else return NULL;
        }
    }
    return node;
}

rtree *rtree_delete(rtree *root, char *key)
{
    rtree *nodeToDelete = rtree_lookup(root,key);
    if(nodeToDelete == NULL) return NULL;

    char *ptrSearch = key;
    rtree *node = root;
    stack *queue = stack_create(strlen(key));

    while(node!=nodeToDelete)
    {
        char oneSymbStr[2];
        oneSymbStr[0] = *ptrSearch;
        oneSymbStr[1] = '\0';

        rtree *child = get_child(node, oneSymbStr);
        if(child == nodeToDelete) break;

        struct rbnode *link = rbtree_lookup(node->childs->root,node->childs->nil,oneSymbStr);

        ptrSearch += strlen(key);
        stack_push(queue,node,link);
        node = child;      
    }

    while(queue->top>1)
    {
        nodeWithLink *predecessor = stack_pop(queue);
        
        char *deletingSuff = strdup(predecessor->link->key);
        rbtree_delete(predecessor->node->childs,predecessor->link->key);

        // Если у родительского узла нет других детей
        if(rbtree_height(predecessor->node->childs->root,predecessor->node->childs->nil)==0)
        {
            nodeWithLink *beforePredecessor = stack_pop(queue);
            int newLength = strlen(deletingSuff)+strlen(beforePredecessor->link->key)+1;
            char *grandPref = (char *)malloc(sizeof(char)*newLength);
            
            strncpy(grandPref,beforePredecessor->link->key,strlen(beforePredecessor->link->key));
            grandPref[strlen(beforePredecessor->link->key)] = '\0';
            strcat(grandPref,deletingSuff);
            free(beforePredecessor->link->key);
            beforePredecessor->link->key = grandPref;

            if(predecessor->node->ifNodeHasValue == hasValue)
            {
                if(rbtree_height(nodeToDelete->childs->root,nodeToDelete->childs->nil)!=0)
                { 
                    rbtree_copy(predecessor->node->childs,nodeToDelete->childs);
                }
                free(deletingSuff);
                
                stack_free(queue);
                return nodeToDelete;
            }
            else
            {
                nodeToDelete = predecessor->node;
                queue->top++;
            }
            
            
            
        }
        // Если у родительского узла есть другие дети
        else
        {
            if(rbtree_height(nodeToDelete->childs->root,nodeToDelete->childs->nil)!=0)
            {
                nodeToDelete->ifNodeHasValue = noValue;
                RBtree *childs = nodeToDelete->childs;
                if(rbtree_height(childs->root,childs->nil)==1)
                {
                    if(childs->root->left==childs->nil && childs->root->right!=childs->nil)
                    {
                        deletingSuff = realloc(deletingSuff,sizeof(char)*(strlen(deletingSuff)+strlen(childs->root->right->key)+1));
                        strcat(deletingSuff,childs->root->right->key);
                        deletingSuff[strlen(deletingSuff)]='\0';
                        set_child(predecessor->node,deletingSuff,childs->root->right->value);
                    }
                    else if(childs->root->left!=childs->nil && childs->root->right==childs->nil)
                    {
                        deletingSuff = realloc(deletingSuff,sizeof(char)*(strlen(deletingSuff)+strlen(childs->root->left->key)+1));
                        strcat(deletingSuff,childs->root->left->key);
                        deletingSuff[strlen(deletingSuff)]='\0';
                        set_child(predecessor->node,deletingSuff,childs->root->left->value);
                    }
                    
                }
                free(deletingSuff);
                stack_free(queue);
                return nodeToDelete;
            }
            else
            {
                free(deletingSuff);
                stack_free(queue);
                return nodeToDelete;
            }

        }

        
        free(deletingSuff);
    }
    stack_free(queue);
    return nodeToDelete;
}