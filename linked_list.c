#include "linked_list.h"
#include <stdio.h>  
#include <pthread.h>
#include "memory_manager.h"


void list_init(Node** head, size_t size){
    mem_init(size);
}

void list_insert(Node** head, uint16_t data){
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->lock, NULL);

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;

        while (temp->next != NULL) {
            temp = temp->next;
        }
        pthread_mutex_lock(&temp->lock);
        temp->next = new_node;
        pthread_mutex_unlock(&temp->lock);
    }


};

void list_insert_after(Node* prev_node, uint16_t data){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = prev_node->next;

    pthread_mutex_lock(&prev_node->lock);
    prev_node->next = new_node;
    pthread_mutex_unlock(&prev_node->lock);

};

void list_insert_before(Node** head, Node* next_node, uint16_t data){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = next_node;
    Node* current = *head;
    if(next_node == *head){        
        pthread_mutex_lock(&(*head)->lock);  // Lock the head node
        new_node->next = *head;
        *head = new_node;
        pthread_mutex_unlock(&new_node->lock);  // Unlock the new node (head now)
        return;
    }
    while(current->next != next_node){
        current = current->next;
    }
    pthread_mutex_lock(&current->lock);
    current->next = new_node;
    pthread_mutex_unlock(&current->lock);

}

void list_delete(Node** head, uint16_t data){
    Node* current = *head;
    Node* prev = NULL;
    while(current->data != data){
        prev = current;
        current = current->next;
    }
    if(prev == NULL){
        *head = current->next;

    } else {
        pthread_mutex_lock(&current->lock);
        pthread_mutex_lock(&prev->lock);
        prev->next = current->next;
        pthread_mutex_unlock(&prev->lock);
    }
    pthread_mutex_unlock(&current->lock);
    pthread_mutex_destroy(&current->lock);
    mem_free(current);
}

Node* list_search(Node** head, uint16_t data){
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
    Node* current = *head;
    int count = 0;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

void list_cleanup(Node** head){
    Node* current = *head;
    Node* temp;
    while(current != NULL){
        temp = current;
        current = current->next;
        pthread_mutex_destroy(&temp->lock);
        mem_free(temp);
    }
    *head = NULL;
}