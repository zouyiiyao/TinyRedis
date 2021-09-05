//
// Created by zouyi on 2021/9/1.
//

#ifndef TINYREDIS_UTILS_H
#define TINYREDIS_UTILS_H

#include "sds.h"

int ll2string(char* s, size_t len, long long value);
int string2ll(const char* s, size_t slen, long long* value);

#endif //TINYREDIS_UTILS_H
