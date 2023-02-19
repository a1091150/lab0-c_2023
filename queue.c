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
    q_reverseK(head, 2);
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
    if (k <= 1)
        return;

    struct list_head *safe, *node, *start = head->next;
    int c = 0;
    list_for_each_safe (node, safe, head) {
        c++;
        if (c == k) {
            c = 0;
            struct list_head *prev = start->prev;

            LIST_HEAD(sub_head);
            sub_head.next = start;
            start->prev = &sub_head;
            sub_head.prev = node;
            node->next = &sub_head;
            q_reverse(&sub_head);
            prev->next = sub_head.next;
            sub_head.next->prev = prev;
            sub_head.prev->next = safe;
            safe->prev = sub_head.prev;

            start = safe;
        }
    }
}


void merge_two_lists(struct list_head *left_head,
                     struct list_head *right_head,
                     struct list_head *head)
{
    while (!list_empty(left_head) && !list_empty(right_head)) {
        element_t *e1 = list_entry(left_head->next, element_t, list);
        element_t *e2 = list_entry(right_head->next, element_t, list);
        int c = strcmp(e1->value, e2->value);
        struct list_head *node = c > 0 ? right_head->next : left_head->next;
        list_del(node);
        list_add_tail(node, head);
    }

    if (list_empty(left_head)) {
        list_splice_tail(right_head, head);
    } else {
        list_splice_tail(left_head, head);
    }
}
/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head)
        return;
    if (list_is_singular(head) || list_empty(head))
        return;

    // split with middle.
    struct list_head *slow = head->next, *fast = head->next->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    LIST_HEAD(left_head);
    LIST_HEAD(right_head);
    struct list_head *right_node = slow->next;
    left_head.next = head->next;
    left_head.next->prev = &left_head;
    left_head.prev = slow;
    left_head.prev->next = &left_head;

    right_head.next = right_node;
    right_head.next->prev = &right_head;
    right_head.prev = head->prev;
    right_head.prev->next = &right_head;
    INIT_LIST_HEAD(head);

    q_sort(&left_head);
    q_sort(&right_head);
    merge_two_lists(&left_head, &right_head, head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    int ret = 0;
    if (!head)
        return ret;
    if (list_empty(head))
        return ret;

    ret = 1;
    LIST_HEAD(less);
    struct list_head *cur = head->prev, *node = head->prev->prev;

    while (node != head) {
        struct list_head *prev = node->prev;
        element_t *e1 = list_entry(cur, element_t, list);
        element_t *e2 = list_entry(node, element_t, list);
        int c = strcmp(e2->value, e1->value);
        if (c >= 0) {
            cur = node;
            ret++;
        } else {
            list_move(node, &less);
        }
        node = prev;
    }

    element_t *e, *safe;
    list_for_each_entry_safe (e, safe, &less, list) {
        free(e->value);
        free(e);
    }
    return ret;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
