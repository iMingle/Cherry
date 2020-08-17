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

#include "base.h"
#include "echo_cnt.h"

static int byte_cnt;  /* Byte counter */
static sem_t *mutex;   /* and the mutex that protects it */
const char *lock_echo = "echo_cnt_sem_mutex";

static void init_echo_cnt(void) {
//    Sem_init(&mutex, 0, 1);
    mutex = Sem_open(lock_echo, O_CREAT, S_IRUSR | S_IWUSR, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd) {
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    Pthread_once(&once, init_echo_cnt);
    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        P(mutex);
        byte_cnt += n;
        printf("server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
        V(mutex);
        Rio_writen(connfd, buf, n);
    }
}

void echo_release() {
    sem_close(mutex);
    sem_unlink(lock_echo);
}
