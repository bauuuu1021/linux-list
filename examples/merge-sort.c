#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

static uint16_t values[N];

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

    INIT_LIST_HEAD(p);
    list_for_each_entry_safe (cur, is, &result, list) {
        list_move_tail(&cur->list, p);
    }
}

static void list_mergesort(struct list_head *head)
{
    struct list_head p, q;
    struct listitem *cur, *is;
    int i, n = 0;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&p);
    INIT_LIST_HEAD(&q);

    list_for_each_entry_safe (cur, is, head, list)
        n++;

    for (i = 0; i < n / 2; i++)
        list_move_tail(head->next, &p);

    for (i = 0; i < n - n / 2; i++)
        list_move_tail(head->next, &q);

    list_mergesort(&p);
    list_mergesort(&q);
    list_combine(&p, &q);
    list_for_each_entry_safe (cur, is, &p, list) {
        list_move_tail(&cur->list, head);
    }
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *cur, *is = NULL;
    struct timespec start, end;
    size_t i;

#if select
    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));
#elif 1 - select
    sequential_array(values, (uint16_t) ARRAY_SIZE(values));
#endif

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    clock_gettime(CLOCK_REALTIME, &start);
    list_mergesort(&testlist);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("merge sort  : %f sec\n", diff_in_second(start, end));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}