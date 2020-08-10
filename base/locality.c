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

/**
 * 局部性: 时间局部性 & 空间局部性
 * 在一个具有良好时间局部性的程序中,被引用过一次的内存位置很可能在不远的将来再被多次引用.
 * 在一个具有良好空间局部性的程序中,如果一个内存位置被引用了一次,那么程序很可能在不远的将来引用附近的一个内存位置
 */

#include "locality.h"

/*
 * 变量sum在每次循环中被引用一次,因此对于sum来说,有好的时间局部性
 * 变量v按照在内存中的顺序进行读取,因此对于v来说,有好的空间局部性,但时间局部性很差,每个元素只访问一次
 */
int sumvec(int v[], size_t length) {
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += v[i];
    }

    return sum;
}
