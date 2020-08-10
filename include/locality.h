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

#ifndef CHERRY_LOCALITY_H
#define CHERRY_LOCALITY_H

#include <ntsid.h>

#define GET_ARRAY_LEN(array, len) {len = (sizeof(array) / sizeof((array)[0]));}

int sumvec(int v[], size_t length);

#endif //CHERRY_LOCALITY_H
