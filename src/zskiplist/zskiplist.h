//
// Created by zouyi on 2021/8/21.
//

#ifndef TINYREDIS_ZSKIPLIST_H
#define TINYREDIS_ZSKIPLIST_H

/*
 * ...
 */
typedef struct redisObject {
    // C skill
    unsigned type:4;

    unsigned encoding:4;

    unsigned lru:24;

    int refcount;

    void* ptr;
} robj;

/*
 * ...
 */
typedef struct zskiplistNode {

    robj* obj;

    double score;

    struct zskiplistNode* backward;

    struct zskiplistLevel {

        struct zskiplistNode* forward;

        unsigned int span;

    } level[];

} zskiplistNode;

/*
 * ...
 */
typedef struct zskiplist {

    struct zskiplistNode* header;

    struct zskiplistNode* tail;

    unsigned long length;

    int level;

} zskiplist;

/* Sorted sets data type */

/*
 * ...
 */
typedef struct {

    double min, max;

    int minex, maxex;
} zrangespec;

typedef struct {

    robj* min;
    robj* max;

    int minex, maxex;
} zlexrangespec;

zskiplist* zslCreate(void);
void zslFree(zskiplist* zsl);
zskiplistNode* zslInsert(zskiplist* zsl, double score, robj* obj);
int zslDelete(zskiplist* zsl, double score, robj* obj);
zskiplistNode* zslFirstInRange(zskiplist* zsl, zrangespec* range);
zskiplistNode* zslLastInRange(zskiplist* zsl, zrangespec* range);

#endif //TINYREDIS_ZSKIPLIST_H
