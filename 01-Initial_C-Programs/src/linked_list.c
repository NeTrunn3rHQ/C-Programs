/*
 * Singly linked list of int values.
 *
 * Build: gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -O2 linked_list.c -o linked_list
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *node_create(int value)
{
    Node *node = malloc(sizeof(*node));
    if (node == NULL) return NULL;
    node->value = value;
    node->next = NULL;
    return node;
}

static int list_push_front(Node **head, int value)
{
    if (head == NULL) return 1;
    Node *node = node_create(value);
    if (node == NULL) return 1;
    node->next = *head;
    *head = node;
    return 0;
}

static int list_push_back(Node **head, int value)
{
    if (head == NULL) return 1;
    Node *node = node_create(value);
    if (node == NULL) return 1;

    if (*head == NULL) {
        *head = node;
        return 0;
    }
    Node *current = *head;
    while (current->next != NULL) current = current->next;
    current->next = node;
    return 0;
}

static int list_delete_first(Node **head, int value)
{
    if (head == NULL) return 1;
    Node **link = head;
    while (*link != NULL && (*link)->value != value)
        link = &(*link)->next;
    if (*link == NULL) return 1;

    Node *removed = *link;
    *link = removed->next;
    free(removed);
    return 0;
}

static Node *list_find(Node *head, int value)
{
    for (Node *current = head; current != NULL; current = current->next)
        if (current->value == value) return current;
    return NULL;
}

static void list_reverse(Node **head)
{
    if (head == NULL) return;
    Node *previous = NULL;
    Node *current = *head;
    while (current != NULL) {
        Node *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    *head = previous;
}

static size_t list_length(const Node *head)
{
    size_t length = 0U;
    while (head != NULL) {
        ++length;
        head = head->next;
    }
    return length;
}

static void list_destroy(Node **head)
{
    if (head == NULL) return;
    while (*head != NULL) {
        Node *next = (*head)->next;
        free(*head);
        *head = next;
    }
}

static void list_print(const Node *head)
{
    for (; head != NULL; head = head->next)
        printf("%d%s", head->value, head->next == NULL ? "\n" : " -> ");
}

int main(void)
{
    Node *head = NULL;
    assert(list_push_back(&head, 10) == 0);
    assert(list_push_back(&head, 20) == 0);
    assert(list_push_front(&head, 5) == 0);
    assert(list_length(head) == 3U);
    assert(list_find(head, 20) != NULL);

    puts("List:");
    list_print(head);
    assert(list_delete_first(&head, 10) == 0);
    list_reverse(&head);
    puts("After deleting 10 and reversing:");
    list_print(head);

    list_destroy(&head);
    assert(head == NULL);
    return EXIT_SUCCESS;
}
