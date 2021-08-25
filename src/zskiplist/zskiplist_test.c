#include <stdio.h>
#include "zskiplist.h"
#include "zmalloc.h"

#define PRINT_FONT_YEL printf("\033[33m");
#define PRINT_FONT_WHI printf("\033[37m");
#define getDataFromZslNode(zslNode) ((Node*)zslNode->obj->ptr)->data

// 值结构定义
typedef struct Node {
    int data;
} Node;


// 创建值
Node* getNode(int data) {
    PRINT_FONT_YEL
    printf("CALL zmalloc %d\n", data);
    PRINT_FONT_WHI
    Node* node;
    node = zmalloc(sizeof(Node));
    node->data = data;
    return node;
}

// 释放值
void freeNode(void* n) {
    PRINT_FONT_YEL
    printf("CALL zfree %d\n", ((Node*)n)->data);
    PRINT_FONT_WHI
    zfree(n);
}

int main(int argc, char* argv[]) {
    robj obj1;
    robj obj2;
    robj obj3;
    robj obj4;
    robj obj5;
    Node* node1 = getNode(1);
    Node* node2 = getNode(2);
    Node* node3 = getNode(3);
    Node* node4 = getNode(4);
    Node* node5 = getNode(5);
    obj1.ptr = node1;
    obj2.ptr = node2;
    obj3.ptr = node3;
    obj4.ptr = node4;
    obj5.ptr = node5;

    // level: 3 2 2 5 3
    zskiplist* nodeZsl = zslCreate();
    zskiplistNode* zslNode;
    zslNode = zslInsert(nodeZsl, 10, &obj1);
    zslNode = zslInsert(nodeZsl, 20, &obj2);
    zslNode = zslInsert(nodeZsl, 40, &obj4);
    zslNode = zslInsert(nodeZsl, 30, &obj3);
    zslNode = zslInsert(nodeZsl, 50, &obj5);
    printf("max level: %d\n", nodeZsl->level);

    // delete obj3
    zslDelete(nodeZsl, 30, &obj3);
    printf("max level: %d\n", nodeZsl->level);

    // range [20, 45]
    zrangespec zrange = {
            .min = 20,
            .max = 45,
            .minex = 0,
            .maxex = 0
    };
    zskiplistNode* hintedNode = zslFirstInRange(nodeZsl, &zrange);
    printf("First element in range [%f, %f] is %d\n", zrange.min, zrange.max, getDataFromZslNode(hintedNode));
    hintedNode = zslLastInRange(nodeZsl, &zrange);
    printf("Last element in range [%f, %f] is %d\n", zrange.min, zrange.max, getDataFromZslNode(hintedNode));

    // rank
    unsigned long rank = zslGetRank(nodeZsl, 20, &obj2);
    printf("obj2 score: %f rank: %lu\n", 20., rank);
    zskiplistNode* rankedNode = zslGetElementByRank(nodeZsl, 3);
    printf("Rank 3 element is %d\n", getDataFromZslNode(rankedNode));

    // delete range
    unsigned long removed = zslDeleteRangeByScore(nodeZsl, &zrange);
    printf("Removed in range [%f, %f] is %lu\n", zrange.min, zrange.max, removed);
    removed = zslDeleteRangeByRank(nodeZsl, 1, 5);
    printf("Removed in rank [%d, %d] is %lu\n", 1, 5, removed);

    zslFree(nodeZsl);

    freeNode(node1);
    freeNode(node2);
    freeNode(node3);
    freeNode(node4);
    freeNode(node5);
}

