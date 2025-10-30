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
                    

                    link->value = NULL;
                    rbtree_delete(node->childs,link->key);
                    set_child(node,substrStart,newChild);
                    //free(link->key);
                    //link->key = strdup(substrStart);
                    
                    //link->value = newChild;                   

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

                    //rtree *valueBuffer = link->value;                    
                    link->value=NULL;
                    rbtree_delete(node->childs,link->key);
                    set_child(node,substring,newChild);
                    //free(link->key);
                    //link->key = strdup(substring);
                    
                    //link->value = newChild;
                    
                    
                    
                    
                    
                    
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

void print_rtree_node(struct rbnode* node, struct RBtree* tree, int depth, const char* prefix, int is_last) {
    if (node == tree->nil) return;
    
    // Вывод текущего узла
    printf("%s", prefix);
    printf(is_last ? "└── " : "├── ");
    
    if (node->key[0] != '\0') {
        printf("(%s)", node->key);
    } else {
        printf("[]");
    }
    
    // Если у узла есть значение, выводим его
    if (node->value->ifNodeHasValue) {
        printf(" [%u $]", node->value->value);
    }
    printf("\n");
    
    // Формируем префикс для дочерних узлов
    char new_prefix[256];
    strcpy(new_prefix, prefix);
    strcat(new_prefix, is_last ? "    " : "│   ");
    
    // Рекурсивный обход дочерних узлов
    if (node->value->childs != NULL && node->value->childs->root != node->value->childs->nil) {
        // Находим все дочерние узлы в RBtree
        struct rbnode* current = node->value->childs->root;
        struct rbnode** stack = malloc(100 * sizeof(struct rbnode*));
        int stack_size = 0;
        
        // In-order traversal для получения узлов в порядке возрастания
        while (stack_size > 0 || current != node->value->childs->nil) {
            if (current != node->value->childs->nil) {
                stack[stack_size++] = current;
                current = current->left;
            } else {
                current = stack[--stack_size];
                
                // Выводим дочерний узел
                int is_last_child = (current->right == node->value->childs->nil);
                print_rtree_node(current, node->value->childs, depth + 1, new_prefix, is_last_child);
                
                current = current->right;
            }
        }
        free(stack);
    }
}

// Функция для печати всего дерева
void print_rtree(struct RBtree* tree) {
    if (tree == NULL || tree->root == tree->nil) {
        printf("Empty tree\n");
        return;
    }
    
    printf("Tree structure:\n");
    print_rtree_node(tree->root, tree, 0, "", 0);
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
    nodeWithLink pair = {NULL,NULL};
    struct rbnode *preLink = NULL;
    rtree *preNode = NULL;

    char *ptrSearch = key;
    rtree *node = root;
    //stack *queue = stack_create(strlen(key));

    while(node!=nodeToDelete && *ptrSearch!='\0')
    {
        char oneSymbStr[2];
        oneSymbStr[0] = *ptrSearch;
        oneSymbStr[1] = '\0';

        rtree *child = get_child(node, oneSymbStr);
        //if(child == nodeToDelete) break;

        struct rbnode *link = rbtree_lookup(node->childs->root,node->childs->nil,oneSymbStr);
        
        

        
        ptrSearch += strlen(link->key);
        if(pair.node!=NULL) preNode = pair.node;
        pair.node = node;
        
        if(pair.link!=NULL) preLink = pair.link;
        pair.link = link;
        
        
        node = child;
              
    }


    if(pair.node==NULL || pair.link==NULL) return NULL;


    if(rbtree_height(nodeToDelete->childs->root,nodeToDelete->childs->nil)==0)
    {
        pair.link->value=NULL;
        rbtree_delete(pair.node->childs,pair.link->key);


        // Если у родителя остался один потомок и в родителе нет значения -- объединяем ключи до родителя и до племянника
        if(preLink!=NULL 
            && rbtree_height(pair.node->childs->root,pair.node->childs->nil)==1
            && pair.node->ifNodeHasValue==noValue)
        {
            
            struct rbnode *linkToNephew = NULL;
            if(pair.node->childs->root != NULL && pair.node->childs->root!= pair.node->childs->nil) linkToNephew = pair.node->childs->root;

            if(linkToNephew!=NULL && preNode!=NULL)
            {
                int lengthNewLink = (strlen(linkToNephew->key)+strlen(preLink->key)+1);
                char *newLinkKey = (char *)malloc(sizeof(char)*lengthNewLink);
                strncpy(newLinkKey,preLink->key,strlen(preLink->key)+1);
                strcat(newLinkKey,linkToNephew->key);
                preLink->value = NULL;
                rbtree_delete(preNode->childs,preLink->key);
                set_child(preNode,newLinkKey,linkToNephew->value);


                linkToNephew->value=NULL;



                rbtree_delete(pair.node->childs,linkToNephew->key);
                
                free(newLinkKey);
                rtree_free(pair.node);

            }

            
        }
    }
    else
    {
        nodeToDelete->ifNodeHasValue = noValue;
        if(rbtree_height(nodeToDelete->childs->root,nodeToDelete->childs->nil)==1)
        {
            
            
            char *newKey = (char *)malloc(sizeof(char)*(strlen(pair.link->key)+
                strlen(nodeToDelete->childs->root->key)+1));
            strncpy(newKey,pair.link->key,strlen(pair.link->key)+1);
            newKey[strlen(pair.link->key)] = '\0';
            strcat(newKey,nodeToDelete->childs->root->key);

            rbtree_delete(pair.node->childs, pair.link->key);

            set_child(pair.node, newKey, nodeToDelete->childs->root->value);
            //free(pair.link->key);

            

            free(newKey);

        }
    }
    //rtree_delete
    
    return nodeToDelete;
}