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

#include <stdio.h>

#include "base.h"
#include "mmapcopy.h"
#include "locality.h"
#include "tiny.h"
#include "number.h"
#include "select.h"
#include "echoserverp.h"
#include "echoservers.h"
#include "echoservert.h"

int main(int argc, char **argv) {
//    readfile(argv[1], 10);
    char *port = "8000";
//    tiny_web_server_start(port);
//    select_server_start(port);
//    echo_process_server_start(port);
//    echo_select_server_start(port);
    echo_thread_server_start(port);
    exit(0);
}
