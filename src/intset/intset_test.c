//
// Created by zouyi on 2021/8/26.
//

#include <stdio.h>
#include <stdint.h>
#include "intset.h"

void printIntset(intset* is) {
    int64_t cur;
    printf("intset length: %d intset encoding: %d\n", is->length, is->encoding);
    printf("intset content: ");
    for (int i = 0; i < is->length; i++) {
        if (is->encoding == INTSET_ENC_INT64) {
            cur = ((int64_t*)is->contents)[i];
        } else if (is->encoding == INTSET_ENC_INT32) {
            cur = ((int32_t*)is->contents)[i];
        } else {
            cur = ((int16_t*)is->contents)[i];
        }
        printf("%d,%ld ", i, cur);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    // 插入元素
    uint8_t success;
    intset* is = intsetNew();
    // 注意使用方式，is指针可能改变
    is = intsetAdd(is, 10, &success);
    printf("Add %d into intset, success: %d\n", 10, success);
    is = intsetAdd(is, 20, &success);
    printf("Add %d into intset, success: %d\n", 20, success);
    is = intsetAdd(is, 30, &success);
    printf("Add %d into intset, success: %d\n", 30, success);
    is = intsetAdd(is, 40, &success);
    printf("Add %d into intset, success: %d\n", 40, success);
    is = intsetAdd(is, 50, &success);
    printf("Add %d into intset, success: %d\n", 50, success);
    /*
     * IntsetLen: 5 IntsetBolbLen 18
     * intset length: 5 intset encoding: 2
     * intset content: 0,10 1,20 2,30 3,40 4,50
     */
    printf("IntsetLen: %d IntsetBolbLen %ld\n", intsetLen(is), intsetBlobLen(is));
    printIntset(is);
    printf("\n");

    // 升级
    is = intsetAdd(is, INT16_MAX + 1, &success);
    printf("Add %d into intset, success: %d\n", INT16_MAX + 1, success);
    int64_t val = 0;
    intsetGet(is, 5, &val);
    printf("Intset pos %d value is: %ld\n", 5, val);
    /*
     * IntsetLen: 6 IntsetBolbLen 32
     * intset length: 6 intset encoding: 4
     * intset content: 0,10 1,20 2,30 3,40 4,50 5,32768
     */
    printf("IntsetLen: %d IntsetBolbLen %ld\n", intsetLen(is), intsetBlobLen(is));
    printIntset(is);
    printf("\n");

    // 不会降级
    is = intsetRemove(is, INT16_MAX + 1, &success);
    printf("Remove %d from intset, success: %d\n", INT16_MAX + 1, success);
    printf("%d in intset or not: %d\n", INT16_MAX + 1, intsetFind(is, INT16_MAX + 1));
    /*
     * IntsetLen: 5 IntsetBolbLen 28
     * intset length: 5 intset encoding: 4
     * intset content: 0,10 1,20 2,30 3,40 4,50
     */
    printf("IntsetLen: %d IntsetBolbLen %ld\n", intsetLen(is), intsetBlobLen(is));
    printIntset(is);
    printf("\n");
    return 0;
}
