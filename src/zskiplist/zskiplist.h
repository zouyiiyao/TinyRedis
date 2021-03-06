//
// Created by zouyi on 2021/8/21.
//

#ifndef TINYREDIS_ZSKIPLIST_H
#define TINYREDIS_ZSKIPLIST_H

#define ZSKIPLIST_MAXLEVEL 32
#define DEBUG
#ifdef DEBUG
#define ZSKIPLIST_P 0.5
#else
#define ZSKIPLIST_P 0.25
#endif

/*
 * Redis对象
 */
typedef struct redisObject {
    // C语言技巧: 位域
    
    // 类型
    /* unsigned type:4; */

    // 编码
    /* unsigned encoding:4; */

    // 对象最后一次被访问的时间
    /* unsigned lru:24; */

    // 引用计数
    /* int refcount; */

    // 指向实际值的指针
    void* ptr;
} robj;

/*
 * 跳跃表节点
 */
typedef struct zskiplistNode {

    // 对象
    robj* obj;

    // 分值
    double score;

    // 后退指针
    struct zskiplistNode* backward;

    // 层结构
    struct zskiplistLevel {

        // 前进指针
        struct zskiplistNode* forward;

        // 跨度
        unsigned int span;

    } level[];

} zskiplistNode;

/*
 * 跳跃表
 */
typedef struct zskiplist {

    // 表头节点
    struct zskiplistNode* header;

    // 表尾节点
    struct zskiplistNode* tail;

    // 表中节点的数量(表头节点不计算在内)
    unsigned long length;

    // 表中层数最大的节点的层数(表头节点的层数不计算在内)
    int level;

} zskiplist;

/* 有序集合 */

/*
 * 表示开区间/闭区间范围的结构
 */
typedef struct {

    double min, max;

    int minex, maxex;
} zrangespec;

zskiplist* zslCreate(void);
void zslFree(zskiplist* zsl);
zskiplistNode* zslInsert(zskiplist* zsl, double score, robj* obj);
int zslDelete(zskiplist* zsl, double score, robj* obj);
int zslIsInRange(zskiplist* zsl, zrangespec* range);
zskiplistNode* zslFirstInRange(zskiplist* zsl, zrangespec* range);
zskiplistNode* zslLastInRange(zskiplist* zsl, zrangespec* range);
unsigned long zslDeleteRangeByScore(zskiplist* zsl, zrangespec* range /* , dict* dict */);
unsigned long zslDeleteRangeByRank(zskiplist* zsl, unsigned int start, unsigned int end /* , dict* dict */);
unsigned long zslGetRank(zskiplist* zsl, double score, robj* obj);
zskiplistNode* zslGetElementByRank(zskiplist* zsl, unsigned long rank);

int compareStringObjects(robj* a, robj* b);
int equalStringObjects(robj* a, robj* b);

#endif //TINYREDIS_ZSKIPLIST_H
