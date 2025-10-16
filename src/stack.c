#include <trees.h>

stack *stack_create(int capacity)
{
    stack *one = (stack *)malloc(sizeof(*one));
    if(one==NULL) return NULL;
    one->capacity = capacity;
    one->top = -1;
    one->data = (nodeWithLink **)malloc(capacity*sizeof(nodeWithLink *));

    return one;
}

void stack_push(stack *one, rtree *node, struct rbnode *link)
{
    if(one->top==one->capacity-1) return;
    nodeWithLink* pair = (nodeWithLink *)malloc(sizeof(*pair));
    pair->link = link;
    pair->node = node;
    one->data[++one->top]=pair;
}

nodeWithLink  *stack_pop(stack *one)
{
    if(one->top == -1) return NULL;
    return one->data[one->top--];
}

void stack_free(stack *one)
{
    free(one->data);
    free(one);
}
