//
// Created by zouyi on 2021/8/26.
//

#include <stdio.h>
#include <string.h>
#include "ziplist.h"
#include "zmalloc.h"

int main(int argc, char* argv[]) {
    // 压缩列表，是一段紧凑的，两头可以插入、删除的连续内存空间，可以双向遍历
    unsigned char* zl = ziplistNew();
    printf("print zl:\n");
    ziplistRepr(zl);

    unsigned char ls[254];
    memset(ls, 'a', 254);

    zl = ziplistPush(zl, ls, 254, ZIPLIST_TAIL);
    printf("print zl:\n");
    ziplistRepr(zl);

    zl = ziplistPush(zl, "0", 1, ZIPLIST_TAIL);
    printf("print zl:\n");
    ziplistRepr(zl);

    zl = ziplistPush(zl, "1", 1, ZIPLIST_TAIL);
    printf("print zl:\n");
    ziplistRepr(zl);

    ziplistDeleteRange(zl, 1, 1);
    printf("print zl:\n");
    ziplistRepr(zl);

    zfree(zl);

    // 级联更新

    printf("Cascade update test:\n");

    zl = ziplistNew();

    // need 254 bytes, prev_len at least 1, encoding 2, content 251
    unsigned char s1[251];
    unsigned char s2[250];
    const char* ss = "split";
    memset(s1, 'a', 251);
    memset(s2, 'b', 250);

    zl = ziplistPush(zl, ss, 5, ZIPLIST_HEAD);
    zl = ziplistPush(zl, s1, 251, ZIPLIST_HEAD);
    printf("print zl:\n");
    ziplistRepr(zl);

    for (int i = 0; i < 3; i++) {
        zl = ziplistPush(zl, s2, 250, ZIPLIST_TAIL);
    }
    printf("print zl:\n");
    ziplistRepr(zl);

    ziplistDeleteRange(zl, 1, 1);
    printf("print zl:\n");
    ziplistRepr(zl);

    zfree(zl);

    return 0;
}
