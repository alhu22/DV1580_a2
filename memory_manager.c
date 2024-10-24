#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <stdatomic.h>   // for atomic operations

typedef struct Block {
    size_t size;
    bool is_free;
    void* address;
    struct Block* next;
} Block;

void* memory_pool;
Block* head_pool;
pthread_mutex_t lock;

// Initialize the memory pool and head block
void mem_init(size_t size) {
    """The function initializes the memory pool and head block.
    The head block is set to free and points to the memory pool.
    The function also initializes the mutex lock for thread safety."""
    memory_pool = malloc(size);
    head_pool = malloc(sizeof(Block));

    head_pool->size = size;
    head_pool->is_free = true;
    head_pool->address = memory_pool;
    head_pool->next = NULL;
    pthread_mutex_init(&lock, NULL);
}

// Allocate a block of memory
void* mem_alloc(size_t size) {
    """The function allocates the first block that have enough size.
    If the block is larger than the requested size, the block is split into two blocks."""
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;

            // Only split the block if there is enough space left
            if (current->size > size) {
                Block* new_block = malloc(sizeof(Block));
                new_block->size = current->size - size;
                new_block->is_free = true;
                new_block->address = (char*)current->address + size;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }
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
    """The function frees the block of memory and merges every free blocks with the next free block."""
    if (block == NULL) {
        return;
    }
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->address == block) {
            current->is_free = true;
            break;
        }
        current = current->next;
    }

    //merge free blocks with next free block
    current = head_pool;
    while (current != NULL) {
        if (current->is_free && current->next != NULL && current->next->is_free) {
            current->size += current->next->size;
            Block* temp = current->next;
            current->next = current->next->next;
            free(temp);
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
}

// Resize a block of memory
void* mem_resize(void* block, size_t size) {
    """The function resizes the block of memory."""
    if (block == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        if (current->address == block) {
            if (current->size >= size) {
                Block* extra_size = malloc(sizeof(Block));
                extra_size->size = current->size - size;
                extra_size->is_free = true;
                extra_size->address = (char*)current->address + size;
                extra_size->next = current->next;
                current->next = extra_size;
                current->size = size;

                pthread_mutex_unlock(&lock);
                return block;
            } else {
                Block* temp = head_pool;
                while (temp != NULL) {
                    if (temp->is_free && temp->size >= size) {
                        temp->is_free = false;
                        Block* new_block = malloc(sizeof(Block));
                        new_block->size = temp->size - size;
                        new_block->is_free = true;
                        new_block->address = (char*)temp->address + size;
                        new_block->next = temp->next;
                        temp->size = size;
                        temp->next = new_block;
                        break;
                    }
                    temp = temp->next;
                }

                if (temp->address == NULL) {
                    pthread_mutex_unlock(&lock);
                    return NULL;
                }
                memcpy(temp->address, block, current->size);
                current->is_free = true;
                pthread_mutex_unlock(&lock);
                return temp->address;
            }
        }
        current = current->next;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

// Deinitialize the memory pool
void mem_deinit() {
    """The function deinitializes the memory pool and frees all the blocks."""
    pthread_mutex_lock(&lock);
    Block* current = head_pool;
    while (current != NULL) {
        Block* temp = current;
        current = current->next;
        free(temp);
    }
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
    free(memory_pool);
}
