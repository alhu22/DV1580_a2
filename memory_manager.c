#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include "memory_manager.h"


void* memory_pool;
Block* head_pool;
pthread_mutex_t lock;

// Initialize the memory pool and head block
void mem_init(size_t size) {
    memory_pool = malloc(size);
    head_pool = (Block*)malloc(sizeof(Block));
    
    head_pool->size = size;
    head_pool->is_free = true;
    head_pool->address = memory_pool;
    head_pool->next = NULL;
    pthread_mutex_init(&lock, NULL);
}

// Allocate a block of memory
void* mem_alloc(size_t size) {
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->size = current->size - size;
            new_block->is_free = true;
            new_block->address = (char*)current->address + size;
            new_block->next = current->next;

            current->size = size;
            current->next = new_block;
            pthread_mutex_unlock(&lock);
            return current->address;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

// Free a block of memory
void mem_free(void* block) {
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->address == block) {
            current->is_free = true;

            // Try to merge with the next block if it's also free
            if (current->next != NULL && current->next->is_free) {
                current->size += current->next->size;
                Block* temp = current->next;
                current->next = current->next->next;
                free(temp);
            }
            pthread_mutex_unlock(&lock);
            return;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
}
// Resize a block of memory
void* mem_resize(void* block, size_t size) {
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->address == block) {
            if (current->size >= size) {
                current->size = size;
                pthread_mutex_unlock(&lock);
                return block;
            } else {

                pthread_mutex_unlock(&lock);
                void* new_block = mem_alloc(size);
                pthread_mutex_lock(&lock);
                if (new_block == NULL) {
                    pthread_mutex_unlock(&lock);
                    return NULL;
                }
                memcpy(new_block, block, current->size);
                pthread_mutex_unlock(&lock);
                mem_free(block);
                return new_block;
            }
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

// Deinitialize the memory pool
void mem_deinit() {
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        Block* temp = current;
        current = current->next;
        free(temp);
    }
    free(memory_pool);
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
}

