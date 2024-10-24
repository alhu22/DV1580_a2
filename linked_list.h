// linked_list.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdint.h>
#include <pthread.h>
typedef struct Node
{
    uint16_t data;     // Stores the data as an unsigned 16-bit integer
    struct Node *next; // Pointer to the next node in the list
} Node;

extern pthread_mutex_t list_lock; // Mutex lock for the linked list

/**
    * @brief Initialize the linked list with allocation of memory with size
    * @brief initialize listlock
 */
void list_init(Node **head, size_t size);
/**
    * @brief Insert a new node at the end of the list
 */
void list_insert(Node **head, uint16_t data);
/**
    * @brief Insert a new node after the previous node
 */
void list_insert_after(Node *prev_node, uint16_t data);
/**
    * @brief Insert a new node before the next node
 */
void list_insert_before(Node **head, Node *next_node, uint16_t data);
/**
    * @brief Delete a node with the specified data
 */
void list_delete(Node **head, uint16_t data);
/**
    * @brief Search for a node with the specified data
 */
Node *list_search(Node **head, uint16_t data);
/**
    * @brief Display data in the linked list
 */
void list_display(Node **head);
/**
    * @brief Display data in the linked list within a specified range
 */
void list_display_range(Node **head, Node *start_node, Node *end_node);
/**
    * @brief Count the number of nodes in the linked list
 */
int list_count_nodes(Node **head);
/**
    * @brief Cleanup the linked list
 */
void list_cleanup(Node **head);

#endif // LINKED_LIST_H
