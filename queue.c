#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define MAX_STR_LEN 512

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
    element_t *e, *safe = NULL;

    list_for_each_entry_safe(e, safe, head, list)
        q_release_element(e);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *e = (element_t *) malloc(sizeof(element_t));
    if (!head || !e)
        return false;

    e->value = strdup(s);
    if (!e->value) {
        free(e);
        return false;
    }

    list_add(&e->list, head);
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
    list_del(&p->list);

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
    list_del(&p->list);

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

    list_for_each(li, head)
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
    list_del(&e->list);
    q_release_element(e);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    char s[MAX_STR_LEN];
    struct list_head *li = head->next;
    while (li != head && li->next != head) {
        element_t *e = list_entry(li, element_t, list);
        const element_t *e_next = list_entry(li->next, element_t, list);
        if (strcmp(e->value, e_next->value) == 0) {
            strncpy(s, e->value, MAX_STR_LEN - 1);
            s[MAX_STR_LEN - 1] = '\0';
            while (strcmp(e->value, s) == 0) {
                li = li->next;
                list_del(&e->list);
                q_release_element(e);
                if (li == head)
                    break;
                e = list_entry(li, element_t, list);
            }
        } else
            li = li->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;

    struct list_head *li = head->next;

    while (li != head && li->next != head) {
        list_move(li, li->next);
        li = li->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    LIST_HEAD(tmp);

    list_splice_init(head, &tmp);

    while (!list_empty(&tmp)) {
        struct list_head *li = tmp.next;
        list_move(li, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

    int n = q_size(head) / k, r = q_size(head) % k;
    LIST_HEAD(tmp);

    for (int i = 0; i < n; i++) {
        struct list_head *li = head;
        for (int j = 0; j < k; j++)
            li = li->next;
        list_cut_position(&tmp, head, li);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, head);
    }

    for (int i = 0; i < r; i++)
        list_move_tail(head->next, head);
}

void merge(struct list_head *head,
           struct list_head *left,
           struct list_head *right)
{
    while (!list_empty(left) && !list_empty(right)) {
        element_t *l = list_first_entry(left, element_t, list);
        element_t *r = list_first_entry(right, element_t, list);
        if (strcmp(l->value, r->value) <= 0)
            list_move_tail(&l->list, head);
        else
            list_move_tail(&r->list, head);
    }

    if (list_empty(left))
        list_splice_tail(right, head);
    else
        list_splice_tail(left, head);
}

void merge_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *fast = head->next, *slow = head->next;

    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    LIST_HEAD(left);
    LIST_HEAD(right);

    list_cut_position(&left, head, slow);
    list_splice_tail_init(head, &right);

    merge_sort(&left);
    merge_sort(&right);

    merge(head, &left, &right);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    merge_sort(head);

    if (descend)
        q_reverse(head);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    q_reverse(head);

    const char *min = NULL;
    struct list_head *li, *tmp;

    list_for_each_safe(li, tmp, head) {
        element_t *e = list_entry(li, element_t, list);
        if (!min || strcmp(min, e->value) >= 0)
            min = e->value;
        else {
            list_del(li);
            q_release_element(e);
        }
    }

    q_reverse(head);
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    q_reverse(head);

    const char *max = NULL;
    struct list_head *li, *tmp;

    list_for_each_safe(li, tmp, head) {
        element_t *e = list_entry(li, element_t, list);
        if (!max || strcmp(max, e->value) <= 0)
            max = e->value;
        else {
            list_del(li);
            q_release_element(e);
        }
    }

    q_reverse(head);
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    queue_contex_t *fqc = list_first_entry(head, queue_contex_t, chain);
    queue_contex_t *iqc;

    list_for_each_entry(iqc, head, chain) {
        if (iqc == fqc)
            continue;
        list_splice_init(iqc->q, fqc->q);
        fqc->size += iqc->size;
        iqc->size = 0;
    }

    q_sort(fqc->q, descend);

    return fqc->size;
}
