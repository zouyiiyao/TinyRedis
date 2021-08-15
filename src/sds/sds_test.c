#include <stdio.h>
#include "sds.h"

// 按字节打印sds
void printByByte(sds s) {
    char c;
    int length = sdslen(s);
    for (int i = 0; i < length + 1; i++) {
        c = i == length ? '\n' : ' ';
        printf("0x%x%c", *(s + i), c);
    }
}

// 打印sds的内容以及len和free
void printMsg(sds s) {
    printf("s: %s len: %zu free: %zu\n", s, sdslen(s), sdsavail(s));
}

int main(int argc, char* argv[]) {

    sds s1 = sdsnew("Redis");

    // 空间预分配

    /* s: Redis len: 5 free: 0 */
    printMsg(s1);
    /* 拼接是安全的，因为设计时已经避免了缓冲区溢出 */
    s1 = sdscat(s1, " Cluster");
    /* s: Redis Cluster len: 13 free: 13 */
    printMsg(s1);
    s1 = sdscat(s1, "!");
    /* s: Redis Cluster! len: 14 free: 12 */
    printMsg(s1);

    // 惰性释放

    sdsclear(s1);
    /* s:  len: 0 free: 26 */
    printMsg(s1);
    /* s: Redis len: 5 free: 21 */
    s1 = sdscat(s1, "Redis");
    printMsg(s1);

    sds s2 = sdsnew(" Cluster");
    s1 = sdscatsds(s1, s2);
    /* s: Redis Cluster len: 13 free: 13 */
    printMsg(s1);

    // 二进制安全

    /* 0x52 0x65 0x64 0x69 0x73 0x20 0x43 0x6c 0x75 0x73 0x74 0x65 0x72 0x0 */
    printByByte(s1);
    /* 0x52 0x65 0x64 0x69 0x73 0x0 0x43 0x6c 0x75 0x73 0x74 0x65 0x72 0x0 */
    *(s1 + 5) = '\0';
    printByByte(s1);

    // 记得释放空间
    sdsfree(s1);
    sdsfree(s2);

    return 0;
}
