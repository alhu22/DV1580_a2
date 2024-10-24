#include <stdio.h>  
#include <pthread.h>
#include "memory_manager.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>   // for malloc, free

typedef struct Node
{
    uint16_t data;     // Stores the data as an unsigned 16-bit integer
    struct Node *next; // Pointer to the next node in the list
} Node;

pthread_mutex_t list_lock;

void list_init(Node** head, size_t size){
    """The function initializes the list and the mutex lock for thread safety."""
    mem_init(size);
    pthread_mutex_init(&list_lock, NULL);
}

void list_insert(Node** head, uint16_t data){
    """The function inserts a new node at the end of the list."""
    pthread_mutex_lock(&list_lock);
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
        pthread_mutex_unlock(&list_lock);
        return;
    }
    pthread_mutex_unlock(&list_lock);
}

void list_insert_after(Node* prev_node, uint16_t data){
    """The function inserts a new node after the specified node."""
    pthread_mutex_lock(&list_lock);
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    pthread_mutex_unlock(&list_lock);

}

void list_insert_before(Node** head, Node* next_node, uint16_t data){
    """The function inserts a new node before the specified node."""
    pthread_mutex_lock(&list_lock);
    if (*head == NULL || next_node == NULL) {
        printf("The list is empty or the next node is NULL\n");
        return;
    }

    Node* new_node = mem_alloc(sizeof(Node));
    if (new_node == NULL){
        return;
    }

    new_node->data = data;
    new_node->next = next_node;
    Node* current = *head;
    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
        pthread_mutex_unlock(&list_lock);
        return;
    }
    while (current->next != next_node) {
        current = current->next;
    }
    current->next = new_node;
    pthread_mutex_unlock(&list_lock);
}

void list_delete(Node** head, uint16_t data){
    """The function deletes the node with the specified data."""
    pthread_mutex_lock(&list_lock);
    Node* current = *head;
    Node* prev = NULL;
    while(current->data != data){
        prev = current;
        current = current->next;
    }
    if(prev == NULL){
        *head = current->next;

    } else {
        prev->next = current->next;
    }
    pthread_mutex_unlock(&list_lock);
    mem_free(current);
}

Node* list_search(Node** head, uint16_t data){
    """The function searches for a node with the specified data."""
    Node* current = *head;
    while(current != NULL){
        if(current->data == data){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void list_display(Node** head) {
    """The function traverse the list and displays every data in the list."""
    Node* current = *head;
    // If the list is empty
    if (current == NULL) {
        printf("[]");
        return;
    }
    printf("[");
    printf("%d", current->data);
    current = current->next;
    while (current != NULL) {
        printf(", %d", current->data);
        current = current->next;
    }
    printf("]");
}

void list_display_range(Node** head, Node* start_node, Node* end_node) {
    """The function displays the data in the list within the specified range."""
    // If the list is empty
    if (*head == NULL) {
        printf("[]");
        return;
    }

    Node* current = *head;

    // Traverse to start_node if specified
    if (start_node) {
        while (current != NULL && current != start_node) {
            current = current->next;
        }
    }

    if (current == NULL) {
        // If start_node is not found or the list is empty
        printf("[]");
        return;
    }

    // Start printing the list range
    printf("[");
    bool first = true;  // For proper comma placement
    while (current != NULL && (end_node == NULL || current != end_node->next)) {
        if (!first) {
            printf(", ");
        }
        printf("%d", current->data);
        first = false;
        current = current->next;
    }
    printf("]");
}


int list_count_nodes(Node** head){
    """The function counts the number of nodes in the list."""
    Node* current = *head;
    int count = 0;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

void list_cleanup(Node** head){
    """The function frees the memory allocated for the list."""
    pthread_mutex_lock(&list_lock);
    Node* current = *head;
    Node* temp;
    while(current != NULL){
        temp = current;
        current = current->next;
        mem_free(temp);
    }
    *head = NULL;
    pthread_mutex_unlock(&list_lock);
    pthread_mutex_destroy(&list_lock);
    mem_deinit();
}
