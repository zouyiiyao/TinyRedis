#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "dict.h"
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

// 释放值
void freeNode(void* n) {
    PRINT_FONT_YEL
    printf("CALL zfree %d\n", ((Node*)n)->data);
    PRINT_FONT_WHI
    zfree(n);
}

// 顺序迭代
void printDict(dict* d) {
    // 非安全迭代器
    dictIterator* iter;
    dictEntry* entry;
    iter = dictGetIterator(d);

    while ((entry = dictNext(iter)) != NULL) {
        Node* k;
        Node* v;
        k = entry->key;
        v = entry->v.val;
        printf("iter->table: %d iter->index: %d key: %d val: %d ", iter->table, iter->index, k->data, v->data);
    }
    printf("\n");
    dictReleaseIterator(iter);
}

/*
 * dictType定义
 */

unsigned int dictNodeHash(const void* key) {
    return dictGenHashFunction(key, 4);
}

int dictNodeCompare(void* privdata, const void* key1, const void* key2) {
    DICT_NOTUSED(privdata);
    // 比较n个字节，返回0表示相同
    return memcmp(key1, key2, 4) == 0;
}

void dictNodeDestructor(void* privdata, void* val) {
    DICT_NOTUSED(privdata);
    freeNode(val);
}

dictType nodeDictType = {
        .hashFunction = dictNodeHash,
        .keyDup = NULL,
        .valDup = NULL,
        .keyCompare = dictNodeCompare,
        .keyDestructor = dictNodeDestructor,
        .valDestructor = dictNodeDestructor
};

int main(int argc, char* argv[]) {
    // 哈希函数
    Node n1 = {1};
    Node n2 = {1};
    Node n3 = {3};
    unsigned int h = dictNodeHash(&n1);
    // bcca4d7 bcca4d7 4efb0e25
    printf("%x %x %x\n", h, dictNodeHash(&n2), dictNodeHash(&n3));

    Node* k1 = getNode(1);
    Node* k2 = getNode(2);
    Node* k3 = getNode(3);
    Node* k4 = getNode(4);
    Node* v1 = getNode(10);
    Node* v2 = getNode(20);
    Node* v3 = getNode(30);
    Node* v4 = getNode(40);
    int ret;
    dict* nodeDict = dictCreate(&nodeDictType, NULL);
    ret = dictAdd(nodeDict, k1, v1);
    ret = dictAdd(nodeDict, k2, v2);
    ret = dictAdd(nodeDict, k3, v3);
    ret = dictAdd(nodeDict, k4, v4);
    // size: 4 sizemask: 3 used: 4
    printf("size: %lu sizemask: %lu used: %lu\n", nodeDict->ht[0].size, nodeDict->ht[0].sizemask, nodeDict->ht[0].used);
    // Node(1) value is Node(10)
    printf("Node(%d) value is Node(%d)\n", k1->data, ((Node*)dictFetchValue(nodeDict, k1))->data);
    // iter->table: 0 iter->index: 1 key: 3 val: 30 iter->table: 0 iter->index: 2 key: 4 val: 40 iter->table: 0 iter->index: 3 key: 2 val: 20 iter->table: 0 iter->index: 3 key: 1 val: 10
    printDict(nodeDict);

    // 渐进式rehash
    // rehashidx: -1
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    Node* k5 = getNode(5);
    Node* v5 = getNode(50);
    // 开始rehash之后，新增节点都将添加到1号哈希表
    // add to ht[1] !
    ret = dictAdd(nodeDict, k5, v5);

    // 开始rehash后，rehashidx标识为0
    // rehashidx: 0
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    // 还未进行rehash
    // iter->table: 0 iter->index: 1 key: 3 val: 30 iter->table: 0 iter->index: 2 key: 4 val: 40 iter->table: 0 iter->index: 3 key: 2 val: 20 iter->table: 0 iter->index: 3 key: 1 val: 10 iter->table: 1 iter->index: 4 key: 5 val: 50
    printDict(nodeDict);
    // ht[0] size: 4 ht[0].used: 4	ht[1] size: 8 ht[1].used: 1
    printf("ht[0] size: %lu ht[0].used: %lu\tht[1] size: %lu ht[1].used: %lu\n",
           nodeDict->ht[0].size, nodeDict->ht[0].used, nodeDict->ht[1].size, nodeDict->ht[1].used);

    // 执行第一次rehash后，rehashidx = 2，因为第一个bucket为NULL，被跳过了
    dictEntry* randomEntry = dictGetRandomKey(nodeDict);
    // randomEntry key: 3
    printf("randomEntry key: %d\n", ((Node*)randomEntry->key)->data);
    // rehashidx: 2
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    // iter->table: 0 iter->index: 2 key: 4 val: 40 iter->table: 0 iter->index: 3 key: 2 val: 20 iter->table: 0 iter->index: 3 key: 1 val: 10 iter->table: 1 iter->index: 4 key: 5 val: 50 iter->table: 1 iter->index: 5 key: 3 val: 30 
    printDict(nodeDict);
    // ht[0] size: 4 ht[0].used: 3	ht[1] size: 8 ht[1].used: 2
    printf("ht[0] size: %lu ht[0].used: %lu\tht[1] size: %lu ht[1].used: %lu\n",
           nodeDict->ht[0].size, nodeDict->ht[0].used, nodeDict->ht[1].size, nodeDict->ht[1].used);

    // 执行第二次rehash后，rehashidx = 3
    /* dictEntry* entry = */ dictFind(nodeDict, k5);
    // rehashidx: 3
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    // iter->table: 0 iter->index: 3 key: 2 val: 20 iter->table: 0 iter->index: 3 key: 1 val: 10 iter->table: 1 iter->index: 2 key: 4 val: 40 iter->table: 1 iter->index: 4 key: 5 val: 50 iter->table: 1 iter->index: 5 key: 3 val: 30
    printDict(nodeDict);
    // ht[0] size: 4 ht[0].used: 2	ht[1] size: 8 ht[1].used: 3
    printf("ht[0] size: %lu ht[0].used: %lu\tht[1] size: %lu ht[1].used: %lu\n",
           nodeDict->ht[0].size, nodeDict->ht[0].used, nodeDict->ht[1].size, nodeDict->ht[1].used);


    // 执行第三次rehash后，rehashidx = 4
    dictDelete(nodeDict, k1);
    // rehashidx: 4
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    // iter->table: 1 iter->index: 2 key: 4 val: 40 iter->table: 1 iter->index: 3 key: 2 val: 20 iter->table: 1 iter->index: 4 key: 5 val: 50 iter->table: 1 iter->index: 5 key: 3 val: 30
    printDict(nodeDict);
    // ht[0] size: 4 ht[0].used: 0	ht[1] size: 8 ht[1].used: 4
    printf("ht[0] size: %lu ht[0].used: %lu\tht[1] size: %lu ht[1].used: %lu\n",
           nodeDict->ht[0].size, nodeDict->ht[0].used, nodeDict->ht[1].size, nodeDict->ht[1].used);

    // rehash结束
    /* entry = */ dictFind(nodeDict, k5);
    // rehashidx: -1
    printf("rehashidx: %d\n", nodeDict->rehashidx);
    // iter->table: 0 iter->index: 2 key: 4 val: 40 iter->table: 0 iter->index: 3 key: 2 val: 20 iter->table: 0 iter->index: 4 key: 5 val: 50 iter->table: 0 iter->index: 5 key: 3 val: 30
    printDict(nodeDict);
    // ht[0] size: 8 ht[0].used: 4	ht[1] size: 0 ht[1].used: 0
    printf("ht[0] size: %lu ht[0].used: %lu\tht[1] size: %lu ht[1].used: %lu\n",
           nodeDict->ht[0].size, nodeDict->ht[0].used, nodeDict->ht[1].size, nodeDict->ht[1].used);

    // 释放字典
    dictRelease(nodeDict);
    return 0;
}
