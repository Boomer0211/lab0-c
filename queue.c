#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *del, *safe = NULL;

    list_for_each_entry_safe (del, safe, head, list) {
        if (del->value)
            free(del->value);
        free(del);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *p = (element_t *) malloc(sizeof(element_t));
    if (!head || !p)
        return false;

    p->value = strdup(s);
    if (!p->value) {
        free(p);
        return false;
    }

    list_add(&p->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *p = (element_t *) malloc(sizeof(element_t));
    if (!head || !p)
        return false;

    p->value = strdup(s);
    if (!p->value) {
        free(p);
        return false;
    }

    list_add_tail(&p->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if ((!head) || list_empty(head))
        return NULL;

    element_t *p = list_first_entry(head, element_t, list);
    (p->list).prev->next = (p->list).next;
    (p->list).next->prev = (p->list).prev;

    if (sp) {
        strncpy(sp, p->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return p;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if ((!head) || list_empty(head))
        return NULL;

    element_t *p = list_last_entry(head, element_t, list);
    (p->list).prev->next = (p->list).next;
    (p->list).next->prev = (p->list).prev;

    if (sp) {
        strncpy(sp, p->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return p;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if ((!head) || list_empty(head))
        return false;

    struct list_head *fast, *slow;
    for (fast = head->next->next, slow = head->next;
         fast != head && fast != head->prev;
         fast = fast->next->next, slow = slow->next) {
    }

    element_t *e = list_entry(slow, element_t, list);
    (e->list).prev->next = (e->list).next;
    (e->list).next->prev = (e->list).prev;
    q_release_element(e);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *curr = head->next;
    while (curr != head && curr->next != head) {
        element_t *e = list_entry(curr, element_t, list);
        const element_t *e_next = list_entry(curr->next, element_t, list);
        /*  If found duplicate nodes, store the duplicate word to s,
         *  then delete duplicate nodes until found different words.
         */
        if (strcmp(e->value, e_next->value) == 0) {
            struct list_head *prev = curr->prev;
            char *s = strdup(e->value);
            while (strcmp(e->value, s) == 0) {
                curr = curr->next;
                q_release_element(e);
                if (curr == head)
                    break;
                e = list_entry(curr, element_t, list);
            }
            free(s);
            prev->next = curr;
            curr->prev = prev;
        } else
            curr = curr->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
