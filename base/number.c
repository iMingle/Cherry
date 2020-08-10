/*
 * Copyright 2020 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "number.h"

void num() {
    int lval = (1 << 31) - 1;
    int rval = 1 >> 1;
    printf("%d,%d\n", lval, rval);

    short int v = -12345;
    unsigned short uv = (unsigned short) v;
    printf("v = %d, uv = %u\n", v, uv);
    printf("v = %f\n", (3.14 + 1e10) - 1e10);
    printf("v = %f\n", 1e10);
    printf("v = %f\n", 1e20 * 1e20 * 1e-20);

    printf("v = %d\n", INT_MAX);
    printf("v = %u\n", UINT_MAX);
}

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) & x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) & x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) & x, sizeof(void *));
}

void test_show_bytes(int val) {
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;

    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}
