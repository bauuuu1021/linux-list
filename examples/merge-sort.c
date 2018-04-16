#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/list.h"
#include "../private/common.h"

static uint16_t values[256];

static void list_merge(struct list_head *head, int front, int mid, int end)
{
    struct list_head leftList, rightList, *newList = NULL, *nextVisit;
    int nLeft = mid - front, nRight = end - mid;  // num of **link** in sublist
    struct list_head *lCurrent, *rCurrent, *delete;
    int n;
    struct listitem *leftEnd, *rightEnd;

    // init two sublist
    INIT_LIST_HEAD(&leftList);
    INIT_LIST_HEAD(&rightList);

    // complete left sublist
    n = front;
    for (lCurrent = head->next; n > 0; lCurrent = lCurrent->next, n--)
        ;  // find start of leftlist
    for (; nLeft > 0; lCurrent = lCurrent->next, nLeft--)
        list_add_tail(lCurrent, &leftList);

    // complete right sublist
    n = mid;
    for (rCurrent = head->next; n > 0; rCurrent = rCurrent->next, n--)
        ;  // find start of rightlist
    for (; nLeft > 0; rCurrent = rCurrent->next, nLeft--)
        list_add_tail(rCurrent, &rightList);

    // set INT_MAX to the end of both sublist
    leftEnd = (struct listitem *) malloc(sizeof(struct listitem));
    rightEnd = (struct listitem *) malloc(sizeof(struct listitem));
    leftEnd->i = INT16_MAX;
    rightEnd->i = INT16_MAX;
    list_add_tail(&leftEnd->list, &leftList);
    list_add_tail(&rightEnd->list, &rightList);

    // compare and insert into **testlist**
    n = front;
    for (lCurrent = head->next; n > 0; lCurrent = lCurrent->next, n--)
        ;  // find start of leftlist
    n = mid;
    for (rCurrent = head->next; n > 0; rCurrent = rCurrent->next, n--)
        ;  // find start of rightlist

    n = end - front + 1;
    for (delete = head; n > 0; delete = delete->next, n--)
        list_del(delete);

    while (!(list_is_singular(&leftList) && list_is_singular(&rightList))) {
        if (list_entry(lCurrent, struct listitem, list)->i <=
            list_entry(rCurrent, struct listitem, list)->i) {
            nextVisit = lCurrent->next;
            list_move_tail(lCurrent, newList);
            lCurrent = nextVisit;
        } else {
            nextVisit = rCurrent->next;
            list_move_tail(rCurrent, newList);
            rCurrent = nextVisit;
        }
    }
}

static void list_split(struct list_head *head, int front, int end)
{
    if (list_empty(head) || list_is_singular(head) || front >= end)
        return;

    int mid = (int) ((front + end) / 2);
    list_split(head, front, mid);
    list_split(head, mid + 1, end);
    list_merge(head, front, mid, end);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item = NULL, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    //////change//////
    list_split(&testlist, 0, ARRAY_SIZE(values) - 1);
    ///////////////////
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
