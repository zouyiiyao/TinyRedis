#include <stdio.h>
#include "adlist.h"
#include "zmalloc.h"

#define PRINT_FONT_YEL printf("\033[33m");
#define PRINT_FONT_WHI printf("\033[37m");

// 值结构定义
struct Node {
    int data;
};

typedef struct Node Node;

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

// 复制值
void* dupNode(void* o) {
    PRINT_FONT_YEL
    printf("CALL zmalloc %d\n", ((Node*)o)->data);
    PRINT_FONT_WHI
    Node* copy;
    copy = zmalloc(sizeof(Node));
    copy->data = ((Node*)o)->data;
    return copy;
}

// 释放值
void freeNode(void* n) {
    PRINT_FONT_YEL
    printf("CALL zfree %d\n", ((Node*)n)->data);
    PRINT_FONT_WHI
    zfree(n);
}

// 检查值是否相等
int matchNode(void* a, void* b) {
    return ((Node*)a)->data == ((Node*)b)->data;
}

// 顺序打印链表中节点保存的值数据域
void printNodeList(list* l) {
    listIter* iter;
    listNode* lnode;
    iter = listGetIterator(l, AL_START_HEAD);

    while ((lnode = listNext(iter)) != NULL) {
        Node* node;
        node = lnode->value;
        printf("%d ", node->data);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    // 初始化保存值的链表
    list* nodeList = listCreate();
    listSetDupMethod(nodeList, dupNode);
    listSetFreeMethod(nodeList, freeNode);
    listSetMatchMethod(nodeList, matchNode);

    // 往链表中添加
    Node* n1;
    Node* n2;
    Node* n3;
    n1 = getNode(1);
    n2 = getNode(2);
    n3 = getNode(3);
    listAddNodeTail(nodeList, n1);
    listAddNodeTail(nodeList, n2);
    listAddNodeHead(nodeList, n3);
    // Origin nodeList: 3 1 2
    printf("Origin nodeList: ");
    printNodeList(nodeList);

    // 查找给定的值是否在链表中有匹配
    Node* key;
    listNode* lnode;
    key = getNode(3);
    lnode = listSearchKey(nodeList, key);
    // key->data: 3 lnode address: 0x5568229c6778
    printf("key->data: %d lnode address: %p\n", key->data, lnode);
    freeNode(key);

    key = getNode(5);
    lnode = listSearchKey(nodeList, key);
    // key->data: 5 lnode address: (nil)
    printf("key->data: %d lnode address: %p\n", key->data, lnode);

    // 通过索引查找节点
    // length: 3
    printf("length: %lu\n", listLength(nodeList));
    lnode = listIndex(nodeList, 1);
    listInsertNode(nodeList, lnode, key, 1);
    // 3 1 5 2
    printNodeList(nodeList);
    // preNode: 3 nextNode: 5
    printf("preNode: %d nextNode: %d\n", ((Node*)listPrevNode(lnode)->value)->data, ((Node*)listNextNode(lnode)->value)->data);
    
    listDelNode(nodeList, lnode);
    // length: 3
    printf("length: %lu\n", listLength(nodeList));
    // 3 5 2
    printNodeList(nodeList);

    // 将尾节点转到链表头
    listRotate(nodeList);
    // Rotated nodeList: 2 3 5 
    printf("Rotated nodeList: ");
    printNodeList(nodeList);

    // 拷贝整个链表
    list* nodeListCopy = listDup(nodeList);
    // Copied nodeList: 2 3 5 
    printf("Copied nodeList: ");
    printNodeList(nodeListCopy);

    // 记得释放链表
    listRelease(nodeListCopy);
    listRelease(nodeList);
    return 0;
}
