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
#include "psum.h"

/* Global shared variables */
long gsum = 0;           /* Global sum */
long nelems_per_thread;  /* Number of elements to sum */
sem_t *mutex;            /* Mutex to protect global sum */
long psum[MAXTHREADS];   /* Partial sum computed by each thread */

long sum(long nthreads, long log_nelems, void *(*routine)(void *)) {
    long i, nelems, myid[MAXTHREADS], result = 0;
    struct timespec start, end;
    pthread_t tid[MAXTHREADS];

    timespec_get(&start, TIME_UTC);

    nelems = (1L << log_nelems);
    /* Check input arguments */
    if ((nelems % nthreads) != 0 || (log_nelems > 31)) {
        printf("Error: invalid nelems\n");
        exit(0);
    }

    const char *lock = "psum_sem_mutex";
    nelems_per_thread = nelems / nthreads;
    mutex = Sem_open(lock, O_CREAT, S_IRUSR | S_IWUSR, 1);

    /* Create peer threads and wait for them to finish */
    for (i = 0; i < nthreads; i++) {
        myid[i] = i;
        Pthread_create(&tid[i], NULL, routine, &myid[i]);
    }

    for (i = 0; i < nthreads; i++)
        Pthread_join(tid[i], NULL);

    sem_close(mutex);
    sem_unlink(lock);

    /* Check final answer */
    if (routine == sum_mutex && gsum != (nelems * (nelems - 1)) / 2)
        printf("mutex Error: result=%ld\n", gsum);

    /* Add up the partial sums computed by each thread */
    for (i = 0; i < nthreads; i++)
        result += psum[i];

    /* Check final answer */
    if (routine != sum_mutex && result != (nelems * (nelems - 1)) / 2)
        printf("array or local Error: result=%ld\n", result);

    timespec_get(&end, TIME_UTC);
    time_t d_sec = end.tv_sec - start.tv_sec;
    long d_nsec = end.tv_nsec - start.tv_nsec;
    double duration = (d_sec * 1E9 + d_nsec) / 1E6;
    printf("duration: %f ms\n", duration);

    if (routine == sum_mutex) {
        return gsum;
    } else {
        return result;
    }
}

/* Thread routine for psum-mutex.c */
void *sum_mutex(void *vargp) {
    long myid = *((long *) vargp);          /* Extract the thread ID */
    long start = myid * nelems_per_thread; /* Start element index */
    long end = start + nelems_per_thread;  /* End element index */
    long i;

    for (i = start; i < end; i++) {
        P(mutex);
        gsum += i;
        V(mutex);
    }
    return NULL;
}

void *sum_array(void *vargp) {
    long myid = *((long *) vargp);          /* Extract the thread ID */
    long start = myid * nelems_per_thread; /* Start element index */
    long end = start + nelems_per_thread;  /* End element index */
    long i;

    for (i = start; i < end; i++) {
        psum[myid] += i;
    }
    return NULL;
}

void *sum_local(void *vargp) {
    long myid = *((long *) vargp);          /* Extract the thread ID */
    long start = myid * nelems_per_thread; /* Start element index */
    long end = start + nelems_per_thread;  /* End element index */
    long i, sum = 0;

    for (i = start; i < end; i++) {
        sum += i;
    }
    psum[myid] = sum;
    return NULL;
}
