#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdbool.h>
#include <pthread.h>


typedef struct Block {
    int size;
    bool is_free;
    void* address;
    pthread_mutex_t lock;
    struct Block* next;
} Block;

void mem_init(int size);
void* mem_alloc(int size);
void mem_free(void* block);
void* mem_resize(void* block, int size);
void mem_deinit();

#endif