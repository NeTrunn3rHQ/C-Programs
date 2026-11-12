/*
 * Linked-list LIFO stack of int values.
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t size;
} Stack;

static void stack_init(Stack *stack)
{
    if (stack == NULL) return;
    stack->top = NULL;
    stack->size = 0U;
}

static int stack_push(Stack *stack, int value)
{
    if (stack == NULL) return 1;
    StackNode *node = malloc(sizeof(*node));
    if (node == NULL) return 1;
    node->value = value;
    node->next = stack->top;
    stack->top = node;
    ++stack->size;
    return 0;
}

static int stack_pop(Stack *stack, int *value)
{
    if (stack == NULL || stack->top == NULL || value == NULL) return 1;
    StackNode *removed = stack->top;
    *value = removed->value;
    stack->top = removed->next;
    free(removed);
    --stack->size;
    return 0;
}

static int stack_peek(const Stack *stack, int *value)
{
    if (stack == NULL || stack->top == NULL || value == NULL) return 1;
    *value = stack->top->value;
    return 0;
}

static void stack_destroy(Stack *stack)
{
    if (stack == NULL) return;
    while (stack->top != NULL) {
        StackNode *next = stack->top->next;
        free(stack->top);
        stack->top = next;
    }
    stack->size = 0U;
}

int main(void)
{
    Stack stack;
    int value;
    stack_init(&stack);
    assert(stack_push(&stack, 10) == 0);
    assert(stack_push(&stack, 20) == 0);
    assert(stack_push(&stack, 30) == 0);
    assert(stack_peek(&stack, &value) == 0 && value == 30);
    assert(stack_pop(&stack, &value) == 0 && value == 30);
    assert(stack_pop(&stack, &value) == 0 && value == 20);
    assert(stack_pop(&stack, &value) == 0 && value == 10);
    assert(stack_pop(&stack, &value) == 1);
    printf("Stack drained; size=%zu\n", stack.size);
    stack_destroy(&stack);
    return 0;
}
