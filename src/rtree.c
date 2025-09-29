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
    
    
    
    rbtree_add(node->childs, literal, child);
}

rtree *rtree_insert(rtree *root,char *key, uint32_t value)
{
    char *substrStart = key; //Указывает на первый символ в префиксе
    char *substrEnd = key; //Указывает на последний символ в префиксе
    rtree *node = root;

    while(*substrEnd!='\0')
    {
        rtree *child = get_child(node,key);
        int prefixLength = substrEnd-substrStart+1;

        //Поиск наидлиннейшего уже существующего префикса
        while(child!=NULL && *(substrEnd)!='\0') 
        {
            substrEnd++;
            prefixLength = substrEnd-substrStart+1;
            char subToSearch[prefixLength+1];
            strncpy(subToSearch,key,prefixLength);
            subToSearch[prefixLength] = '\0';
            child = get_child(node,subToSearch);
            
        }
        
        
        char *longestPrefix = (char *)malloc((prefixLength+1)*sizeof(char));
        strncpy(longestPrefix,key,prefixLength);
        longestPrefix[prefixLength] = '\0';
        child = get_child(node,longestPrefix);

        

        if(child == NULL)
        {            
            rtree *newChild = rtree_create();
            
            newChild->ifNodeHasValue = noValue;
            newChild->childs = rbtree_create();
            
            set_child(node,longestPrefix,newChild);
            node = newChild;

        }
        else
        {
            rtree *newChild = rtree_create();
            
            newChild->ifNodeHasValue = hasValue;
            newChild->childs = rbtree_create();
            newChild->value = value;

            char *ptrOfEnd = strchr(key,'\0');
            uint32_t childStrLength = ptrOfEnd-substrEnd+1;
            char *keyForChild = (char *)malloc((childStrLength+1)*sizeof(char));
            strncpy(keyForChild,substrEnd+1,childStrLength);
            keyForChild[childStrLength] = '\0';
            set_child(node,keyForChild,newChild);

            free(longestPrefix);
        }
               

        substrStart = substrEnd;      

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
    if(root->childs!=NULL)
    {
        rbtree_free(root->childs);
    }
    free(root);
    
}

