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
#include "goodcount.h"

/* Global shared variables */
volatile long cnt = 0; /* Counter */
sem_t *mutex;           /* Semaphore that protects counter */

void goodcount(long niters) {
    pthread_t tid1, tid2;

    const char *lock = "good_count_sem_mutex";

    /* Create threads and wait for them to finish */
//    Sem_init(&mutex, 0, 1);  /* mutex = 1 */
    mutex = Sem_open(lock, O_CREAT, S_IRUSR | S_IWUSR, 1);  /* mutex = 1 */

    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    sem_close(mutex);
    sem_unlink(lock);

    /* Check result */
    if (cnt != (2 * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
}

/* thread routine */
void *thread(void *vargp) {
    int i, niters = *((int *) vargp);

    for (i = 0; i < niters; i++) {
        P(mutex);
        cnt++;
        V(mutex);
    }

    return NULL;
}
