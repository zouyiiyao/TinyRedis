//
// Created by zouyi on 2021/8/31.
//

#ifndef TINYREDIS_REDIS_H
#define TINYREDIS_REDIS_H

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "redis_obj.h"
#include "zmalloc.h"
#include "utils.h"
#include "sds.h"
#include "adlist.h"
#include "ziplist.h"
#include "zskiplist.h"
#include "dict.h"
#include "intset.h"

/* 执行函数的状态码 */
/* Error codes */
#define REDIS_OK 0
#define REDIS_ERR -1

/* 字典负载参数，小于该值则缩小字典 */
/* Hash table parameters */
#define REDIS_HT_MINFILL 10

/* 对象类型 */
/* Object types */
#define REDIS_STRING 0    /* 字符串类型 */
#define REDIS_LIST 1      /* 列表类型 */
#define REDIS_SET 2       /* 集合类型 */
#define REDIS_ZSET 3      /* 有序集合类型 */
#define REDIS_HASH 4      /* 哈希类型 */

/* 对象编码 */
/* Object encoding */
#define REDIS_ENCODING_RAW 0           /* 底层sds */
#define REDIS_ENCODING_INT 1           /* 底层long */
#define REDIS_ENCODING_HT 2            /* 底层字典 */
#define REDIS_ENCODING_ZIPMAP 3
#define REDIS_ENCODING_LINKEDLIST 4    /* 底层双向链表 */
#define REDIS_ENCODING_ZIPLIST 5       /* 底层压缩列表 */ 
#define REDIS_ENCODING_INTSET 6        /* 底层整数集合 */
#define REDIS_ENCODING_SKIPLIST 7      /* 底层跳表&字典 */
#define REDIS_ENCODING_EMBSTR 8        /* 底层sds */

/* 列表方向 */
/* List related stuff */
#define REDIS_HEAD 0
#define REDIS_TAIL 1

/*
 * 时钟最大值&时钟精度 
 */
#define REDIS_LRU_CLOCK_MAX ((1<<REDIS_LRU_BITS)-1)
#define REDIS_LRU_CLOCK_RESOLUTION 1000

/*
 * 更换底层编码有关的宏
 */
#define HASH_MAX_ZIPLIST_ENTRIES 512
#define HASH_MAX_ZIPLIST_VALUE 64
#define LIST_MAX_ZIPLIST_ENTRIES 512
#define LIST_MAX_ZIPLIST_VALUE 64
#define SET_MAX_INTSET_ENTRIES 512

/*
 * 有序集合结构体定义
 */
typedef struct zset {

    dict* dict;

    zskiplist* zsl;

} zset;

/*
 * 有序集合，表示开区间/闭区间范围的结构
 */
typedef struct {

    double min, max;

    int minex, maxex;
} zrangespec;

/*
 * 列表类型迭代器结构体定义
 */
typedef struct {

    robj* subject;

    unsigned char encoding;

    unsigned char direction;

    unsigned char* zi;

    listNode* ln;

} listTypeIterator;

/*
 * 迭代列表时使用的记录结构，用于保存迭代器，以及迭代器返回的列表节点
 */
typedef struct {

    listTypeIterator* li;

    unsigned char* zi;

    listNode* ln;

} listTypeEntry;

/*
 * 集合类型迭代器结构体定义
 */
typedef struct {

    robj* subject;

    int encoding;

    int ii;

    dictIterator* di;

} setTypeIterator;

/*
 * 哈希类型迭代器结构体定义
 */
typedef struct {

    robj* subject;

    int encoding;

    unsigned char* fptr;

    unsigned char* vptr;

    dictIterator* di;

    dictEntry* de;

} hashTypeIterator;

#define REDIS_HASH_KEY 1
#define REDIS_HASH_VALUE 2

/*
 * 外部使用的全局变量声明
 */
/*
 * Extern declarations
 */
extern dictType setDictType;
extern dictType zsetDictType;
extern dictType hashDictType;

/*
 * 判断是否要缩小字典
 */
int htNeedsResize(dict* dict);

/*
 * 获取LRU时钟
 */
unsigned int getLRUClock(void);

/* Redis object implementation */
void decrRefCount(robj* o);
void decrRefCountVoid(void* o);
void incrRefCount(robj* o);
robj* resetRefCount(robj* obj);
void freeStringObject(robj* o);
void freeListObject(robj* o);
void freeSetObject(robj* o);
void freeZsetObject(robj* o);
void freeHashObject(robj* o);
robj* createObject(int type, void* ptr);
robj* createStringObject(char* ptr, size_t len);
robj* createRawStringObject(char* ptr, size_t len);
robj* createEmbeddedStringObject(char* ptr, size_t len);
robj* dupStringObject(robj* o);
int isObjectRepresentableAsLongLong(robj* o, long long* llongval);
robj* tryObjectEncoding(robj* o);
robj* getDecodedObject(robj* o);
size_t stringObjectLen(robj* o);
robj* createStringObjectFromLongLong(long long value);
robj* createStringObjectFromLongDouble(long double value);
robj* createListObject(void);
robj* createZiplistObject(void);
robj* createSetObject(void);
robj* createIntsetObject(void);
robj* createHashObject(void);
robj* createZsetObject(void);
robj* createZsetZiplistObject(void);
int getLongLongFromObject(robj* o, long long* target);
int getLongDoubleFromObject(robj* o, long double* target);
char* strEncoding(int encoding);
int compareStringObjects(robj* a, robj* b);
int collateStringObjects(robj* a, robj* b);
int equalStringObjects(robj* a, robj* b);
unsigned long long estimateObjectIdleTime(robj* o);
#define sdsEncodesObject(objptr) (objptr->encoding == REDIS_ENCODING_RAW || objptr->encoding == REDIS_ENCODING_EMBSTR)

/* List data type */
void listTypeTryConversion(robj* subject, robj* value);
void listTypePush(robj* subject, robj* value, int where);
robj* listTypePop(robj* subject, int where);
unsigned long listTypeLength(robj* subject);
listTypeIterator* listTypeInitIterator(robj* subject, long index, unsigned char direction);
void listTypeReleaseIterator(listTypeIterator* li);
int listTypeNext(listTypeIterator* li, listTypeEntry* entry);
robj* listTypeGet(listTypeEntry* entry);
void listTypeInsert(listTypeEntry* entry, robj* value, int where);
int listTypeEqual(listTypeEntry* entry, robj* o);
void listTypeDelete(listTypeEntry* entry);
void listTypeConvert(robj* subject, int enc);

/* Set data type */
robj* setTypeCreate(robj* value);
int setTypeAdd(robj* subject, robj* value);
int setTypeRemove(robj* subject, robj* value);
int setTypeIsMember(robj* subject, robj* value);
setTypeIterator* setTypeInitIterator(robj* subject);
void setTypeReleaseIterator(setTypeIterator* si);
int setTypeNext(setTypeIterator* si, robj** objele, int64_t* llele);
robj* setTypeNextObject(setTypeIterator* si);
int setTypeRandomElement(robj* setobj, robj** objele, int64_t* llele);
unsigned long setTypeSize(robj* subject);
void setTypeConvert(robj* subject, int enc);

/* Hash data type */
void hashTypeConvert(robj* o, int enc);
void hashTypeTryConversion(robj* subject, robj** argv, int start, int end);
/* void hashTypeTryObjectEncoding(robj* subject, robj** o1, robj** o2); */
robj* hashTypeGetObject(robj* o, robj* key);
int hashTypeExists(robj* o, robj* key);
int hashTypeSet(robj* o, robj* key, robj* value);
int hashTypeDelete(robj* o, robj* key);
unsigned long hashTypeLength(robj* o);
hashTypeIterator* hashTypeInitIterator(robj* subject);
void hashTypeReleaseIterator(hashTypeIterator* hi);
int hashTypeNext(hashTypeIterator* hi);
void hashTypeCurrentFromZiplist(hashTypeIterator* hi, int what, unsigned char** vstr, unsigned int* vlen, long long* vll);
void hashTypeCurrentFromHashTable(hashTypeIterator* hi, int what, robj** dst);
robj* hashTypeCurrentObject(hashTypeIterator* hi, int what);

/* Sorted sets data type */
zskiplist* zslCreate(void);
void zslFree(zskiplist* zsl);
void zslFreeNode(zskiplistNode* node);
zskiplistNode* zslInsert(zskiplist* zsl, double score, robj* obj);
int zslDelete(zskiplist* zsl, double score, robj* obj);
zskiplistNode* zslFirstInRange(zskiplist* zsl, zrangespec* range);
zskiplistNode* zslLastInRange(zskiplist* zsl, zrangespec* range);
unsigned long zslGetRank(zskiplist* zsl, double score, robj* o);
zskiplistNode* zslGetElementByRank(zskiplist* zsl, unsigned long rank);
// encoding is ziplist
unsigned char* zzlInsert(unsigned char* zl, robj* ele, double score);
double zzlGetScore(unsigned char* sptr);
void zzlNext(unsigned char* zl, unsigned char** eptr, unsigned char** sptr);
void zzlPrev(unsigned char* zl, unsigned char** eptr, unsigned char** sptr);
// zset
unsigned int zsetLength(robj* zobj);
void zsetConvert(robj* zobj, int encoding);

#endif //TINYREDIS_REDIS_H
