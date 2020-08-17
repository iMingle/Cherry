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
#include "sharedbuf.h"

const char *lock_buf = "shared_buf_sem_mutex";
const char *lock_slots = "shared_buf_sem_slots";
const char *lock_items = "shared_buf_sem_items";

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;                                  /* Buffer holds max of n items */
    sp->front = sp->rear = 0;                   /* Empty buffer iff front == rear */
//    Sem_init(&sp->mutex, 0, 1);  /* Binary semaphore for locking */
    sp->mutex = Sem_open(lock_buf, O_CREAT, S_IRUSR | S_IWUSR, 1);  /* Binary semaphore for locking */
//    Sem_init(&sp->slots, 0, n);        /* Initially, buf has n empty slots */
    sp->slots = Sem_open(lock_slots, O_CREAT, S_IRUSR | S_IWUSR, n);        /* Initially, buf has n empty slots */
//    Sem_init(&sp->items, 0, 0);  /* Initially, buf has zero data items */
    sp->items = Sem_open(lock_items, O_CREAT, S_IRUSR | S_IWUSR, 0);  /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
    sem_close(sp->mutex);
    sem_unlink(lock_buf);

    sem_close(sp->slots);
    sem_unlink(lock_slots);

    sem_close(sp->items);
    sem_unlink(lock_items);

    Free(sp->buf);
}

/* Insert item onto the rear of shared buffer sp */
void sbuf_insert(sbuf_t *sp, int item) {
    P(sp->slots);                          /* Wait for available slot */
    P(sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item;/* Insert the item */
    V(sp->mutex);                          /* Unlock the buffer */
    V(sp->items);                          /* Announce available item */
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp) {
    int item;
    P(sp->items);                           /* Wait for available item */
    P(sp->mutex);                           /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)];/* Remove the item */
    V(sp->mutex);                           /* Unlock the buffer */
    V(sp->slots);                           /* Announce available slot */
    return item;
}
