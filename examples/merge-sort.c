#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

static uint16_t values[16];

static void list_combine(struct list_head *p, struct list_head *q)
{
    struct listitem *left, *right;
    struct list_head result;
    INIT_LIST_HEAD(&result);


    struct listitem *cur, *is;
    while (!list_empty(p) && !list_empty(q)) {
        left = list_first_entry(p, struct listitem, list);
        right = list_first_entry(q, struct listitem, list);

        if (left->i < right->i)
            list_move_tail(&left->list, &result);
        else
            list_move_tail(&right->list, &result);

        printf("result:");
        list_for_each_entry_safe (cur, is, &result, list) {
            printf(" %d ", cur->i);
        }
        printf("\n");
    }


    if (!list_empty(p)) {
        list_for_each_entry_safe (cur, is, p, list) {
            list_move_tail(&cur->list, &result);
        }
    }
    if (!list_empty(q)) {
        list_for_each_entry_safe (cur, is, q, list) {
            list_move_tail(&cur->list, &result);
        }
    }
    printf("test\n");
    INIT_LIST_HEAD(p);
    list_for_each_entry_safe (cur, is, &result, list) {
        printf("%d ", cur->i);
        list_move_tail(&cur->list, p);
    }
    printf("test\n");
}

static void list_merge(struct list_head *head, int n)
{
    struct list_head p, q;
    int i;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&p);
    INIT_LIST_HEAD(&q);

    for (i = 0; i < n / 2; i++)
        list_move_tail(head->next, &p);

    for (i = 0; i < n - n / 2; i++)
        list_move_tail(head->next, &q);

    struct listitem *cur, *is;
    list_for_each_entry_safe (cur, is, &p, list) {
        printf("%d ", cur->i);
    }
    printf("\n");
    list_for_each_entry_safe (cur, is, &q, list) {
        printf("%d ", cur->i);
    }
    printf("\n");


    list_merge(&p, n / 2);
    list_merge(&q, n - n / 2);
    list_combine(&p, &q);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        printf("%d\n", item->i);
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    int n = ARRAY_SIZE(values);
    list_merge(&testlist, n);

    struct listitem *cur;

    list_for_each_entry_safe (cur, is, &testlist, list) {
        printf("[]%d ", cur->i);
    }
}