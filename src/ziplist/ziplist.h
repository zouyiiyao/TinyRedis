//
// Created by zouyi on 2021/8/26.
//

#ifndef TINYREDIS_ZIPLIST_H
#define TINYREDIS_ZIPLIST_H

#define ZIPLIST_HEAD 0
#define ZIPLIST_TAIL 1

unsigned char* ziplistNew(void);
unsigned char* ziplistPush(unsigned char* zl, unsigned char* s, unsigned int slen, int where);
unsigned char* ziplistIndex(unsigned char* zl, int index);
unsigned char* ziplistNext(unsigned char* zl, unsigned char* p);
unsigned char* ziplistPrev(unsigned char* zl, unsigned char* p);
unsigned int ziplistGet(unsigned char* p, unsigned char** sval, unsigned int* slen, long long* lval);
unsigned char* ziplistInsert(unsigned char* zl, unsigned char* p, unsigned char* s, unsigned int slen);
unsigned char* ziplistDelete(unsigned char* zl, unsigned char** p);
unsigned char* ziplistDeleteRange(unsigned char* zl, unsigned int index, unsigned int num);
unsigned int ziplistCompare(unsigned char* p, unsigned char* s, unsigned int slen);
unsigned char* ziplistFind(unsigned char* p, unsigned char* vstr, unsigned int vlen, unsigned int skip);
unsigned int ziplistLen(unsigned char* zl);
size_t ziplistBlobLen(unsigned char* zl);
void ziplistRepr(unsigned char *zl);

#endif //TINYREDIS_ZIPLIST_H