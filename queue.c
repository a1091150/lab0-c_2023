#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    element_t *e, *safe;
    list_for_each_entry_safe (e, safe, l, list) {
        free(e->value);
        free(e);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *e = malloc(sizeof(element_t));
    char *dup = strdup(s);
    if (!e) {
        return false;
    }
    if (!dup) {
        free(e);
        return false;
    }
    e->value = dup;
    list_add(&e->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *e = malloc(sizeof(element_t));
    char *dup = strdup(s);
    if (!e) {
        return false;
    }
    if (!dup) {
        free(e);
        return false;
    }
    e->value = dup;
    list_add_tail(&e->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !bufsize) {
        return NULL;
    }
    if (list_empty(head)) {
        return NULL;
    }

    struct list_head *node = head->next;
    element_t *e = container_of(node, element_t, list);
    size_t len = strlen(e->value);
    memset(sp, 0, bufsize);
    if (len > bufsize) {
        strncpy(sp, e->value, bufsize);
        sp[bufsize - 1] = '\n';
    } else {
        strncpy(sp, e->value, len);
    }
    list_del(node);
    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !bufsize) {
        return NULL;
    }
    if (list_empty(head)) {
        return NULL;
    }

    struct list_head *node = head->prev;
    element_t *e = container_of(node, element_t, list);
    size_t len = strlen(e->value);
    memset(sp, 0, bufsize);
    if (len > bufsize) {
        strncpy(sp, e->value, bufsize);
        sp[bufsize - 1] = '\n';
    } else {
        strncpy(sp, e->value, len);
    }
    list_del(node);
    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return 0;
    }
    int c = 0;
    struct list_head *node;
    list_for_each (node, head) {
        c++;
    }
    return c;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head) {
        return false;
    }

    if (list_empty(head)) {
        return false;
    }

    struct list_head *slow, *fast;
    slow = head->next;
    fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_del(slow);
    element_t *e = container_of(slow, element_t, list);
    free(e->value);
    free(e);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head) {
        return;
    }
    head->prev->next = NULL;
    struct list_head *node = head->next;
    struct list_head *cur = head;
    while (node) {
        struct list_head *tmp = node->next;
        cur->prev = node;
        node->next = cur;
        node->prev = tmp;

        cur = node;
        node = tmp;
    }
    cur->prev = head;
    head->next = cur;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
