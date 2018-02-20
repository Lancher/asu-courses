#include "q.h"


int main() {

    // declarations

    Item *item;

    Queue *queue = NewQueue();

    Item *item1 = NewItem(1);
    Item *item2 = NewItem(2);
    Item *item3 = NewItem(3);
    Item *item4 = NewItem(4);
    Item *item5 = NewItem(5);

    // testcase 1: push 5 items, then pop 5 items
    printf("--- Run testcase1 ---\n");

    AddQueue(queue, item1);  // [1]
    AddQueue(queue, item2);  // [1, 2]
    AddQueue(queue, item3);  // [1, 2, 3]
    AddQueue(queue, item4);  // [1, 2, 3, 4]
    AddQueue(queue, item5);  // [1, 2, 3, 4, 5]

    item = queue->head;
    assert( 5 == item->prev->val );  // 5 <- 1 -> 2
    assert( 2 == item->next->val );
    item = item->next;
    assert( 1 == item->prev->val );  // 1 <- 2 -> 3
    assert( 3 == item->next->val );
    item = item->next;
    assert( 2 == item->prev->val );  // 2 <- 3 -> 4
    assert( 4 == item->next->val );
    item = item->next;
    assert( 3 == item->prev->val );  // 3 <- 4 -> 5
    assert( 5 == item->next->val );
    item = item->next;
    assert( 4 == item->prev->val );  // 4 <- 5 -> 1
    assert( 1 == item->next->val );

    item = DelQueue(queue);
    assert( 1 == item->val );  // 1 == 1
    item = DelQueue(queue);
    assert( 2 == item->val );  // 2 == 2
    item = DelQueue(queue);
    assert( 3 == item->val );  // 3 == 3
    item = DelQueue(queue);
    assert( 4 == item->val );  // 4 == 4
    item = DelQueue(queue);
    assert( 5 == item->val );  // 5 == 5

    // testcase 2: push 1 items, rotate 3 times, push 2 items, pop all 3 items
    printf("--- Run testcase2 ---\n");

    AddQueue(queue, item1);  // [1]
    RotateQ(queue);          // [1]
    RotateQ(queue);          // [1]
    RotateQ(queue);          // [1]
    AddQueue(queue, item2);  // [1, 2]
    AddQueue(queue, item3);  // [1, 2, 3]

    item = queue->head;
    assert( 3 == item->prev->val );  // 3 <- 1 -> 2
    assert( 2 == item->next->val );
    item = item->next;
    assert( 1 == item->prev->val );  // 1 <- 2 -> 3
    assert( 3 == item->next->val );
    item = item->next;
    assert( 2 == item->prev->val );  // 2 <- 3 -> 1
    assert( 1 == item->next->val );

    item = DelQueue(queue);
    assert( 1 == item->val );  // 1 == 1
    item = DelQueue(queue);
    assert( 2 == item->val );  // 2 == 2
    item = DelQueue(queue);
    assert( 3 == item->val );  // 3 == 3

    // testcase 3: push 3 items, rotate 2 times, push 2 items, pop all 5 items
    printf("--- Run testcase3 ---\n");

    AddQueue(queue, item1);  // [1]
    AddQueue(queue, item2);  // [1, 2, ]
    AddQueue(queue, item3);  // [1, 2, 3]
    RotateQ(queue);          // [2, 3, 1]
    RotateQ(queue);          // [3, 1, 2]
    AddQueue(queue, item4);  // [3, 1, 2, 4]
    AddQueue(queue, item5);  // [3, 1, 2, 4, 5]

    item = queue->head;
    assert( 5 == item->prev->val );  // 5 <- 3 -> 1
    assert( 1 == item->next->val );
    item = item->next;
    assert( 3 == item->prev->val );  // 3 <- 1 -> 2
    assert( 2 == item->next->val );
    item = item->next;
    assert( 1 == item->prev->val );  // 1 <- 2 -> 4
    assert( 4 == item->next->val );
    item = item->next;
    assert( 2 == item->prev->val );  // 2 <- 4 -> 5
    assert( 5 == item->next->val );
    item = item->next;
    assert( 4 == item->prev->val );  // 4 <- 5 -> 3
    assert( 3 == item->next->val );

    item = DelQueue(queue);
    assert( 3 == item->val );  // 3 == 3
    item = DelQueue(queue);
    assert( 1 == item->val );  // 1 == 1
    item = DelQueue(queue);
    assert( 2 == item->val );  // 2 == 2
    item = DelQueue(queue);
    assert( 4 == item->val );  // 4 == 4
    item = DelQueue(queue);
    assert( 5 == item->val );  // 5 == 5

    // testcase4: pop empty queue will raise error, please uncomment this line to test
    // item = DelQueue(queue);

    exit (EXIT_SUCCESS);
}
