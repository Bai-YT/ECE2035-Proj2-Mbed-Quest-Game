#include <stdlib.h>
#include <stdio.h>
#include "doublely_linked_list.h"

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->current = NULL;
    newList->size = 0;
    return newList;
}

LLNode* create_llnode(void* data) {
  // Your code here
  LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
  newNode->previous = NULL;
  newNode->next = NULL;
  newNode->data = data;
  return newNode;
}


void insertHead(DLinkedList* dLinkedList, void* data){
  LLNode* newNode = create_llnode(data);
  if(dLinkedList->head == NULL){
    dLinkedList->size++;
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;

  }else{
    dLinkedList->size++;
    newNode->next = dLinkedList->head;
    (dLinkedList->head)->previous = newNode;
    dLinkedList->head = newNode;
  }
}


void insertTail(DLinkedList* dLinkedList, void* data){
  LLNode* newNode = create_llnode(data);
  if(dLinkedList->tail == NULL){
    dLinkedList->size++;
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
  }else{
    dLinkedList->size++;
    newNode->previous = dLinkedList->tail;
    (dLinkedList->tail)->next = newNode;
    dLinkedList->tail = newNode;
  }
  // Your code here

}

int insertAfter(DLinkedList* dLinkedList, void* newData){
  if(dLinkedList->current == NULL){
    return 0;
  } else {
    LLNode* newNode = create_llnode(newData);
    dLinkedList->size++;
    newNode->previous = dLinkedList->current;
    if(dLinkedList->current == dLinkedList->tail){
      dLinkedList->tail = newNode;
    }else{
      newNode->next = (dLinkedList->current)->next;
      ((dLinkedList->current)->next)->previous = newNode;
    }
    (dLinkedList->current)->next = newNode;
    return 1;
  }
  // Your code here
}

int insertBefore(DLinkedList* dLinkedList, void* newData){
  if(dLinkedList->current == NULL){
    return 0;
  } else {
    LLNode* newNode = create_llnode(newData);
    dLinkedList->size++;
    newNode->next = dLinkedList->current;
    if(dLinkedList->current == dLinkedList->head){
      dLinkedList->head = newNode;
    }else{
      newNode->previous = (dLinkedList->current)->previous;
      ((dLinkedList->current)->previous)->next = newNode;
    }
    (dLinkedList->current)->previous = newNode;
    return 1;
  }
    // Your code here
}


void* deleteBackward(DLinkedList* dLinkedList){
  if(dLinkedList->current == NULL) {
    return NULL;
  }
  LLNode* deleteNode = dLinkedList->current;
  if(dLinkedList->head == dLinkedList->tail){
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->head){
    dLinkedList->head = (dLinkedList->current)->next;
    (dLinkedList->head)->previous = NULL;
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->tail){
    dLinkedList->tail = (dLinkedList->current)->previous;
    (dLinkedList->tail)->next = NULL;
    dLinkedList->current = dLinkedList->tail;
  }else{
    ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
    ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
    dLinkedList->current = (dLinkedList->current)->previous;
  }
  free(deleteNode->data);
  free(deleteNode);
  dLinkedList->size--;
  return (dLinkedList->current)->data;
}



void* deleteForward(DLinkedList* dLinkedList){
  if(dLinkedList->current == NULL) {
    return NULL;
  }
  LLNode* deleteNode = dLinkedList->current;
  if(dLinkedList->head == dLinkedList->tail){
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->tail){
    dLinkedList->tail = (dLinkedList->current)->previous;
    (dLinkedList->tail)->next = NULL;
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->head){
    dLinkedList->head = (dLinkedList->current)->next;
    (dLinkedList->head)->previous = NULL;
    dLinkedList->current = dLinkedList->head;
  }else{
    ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
    ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
    dLinkedList->current = (dLinkedList->current)->next;
  }
  free(deleteNode->data);
  free(deleteNode);
  dLinkedList->size--;
  return (dLinkedList->current)->data;

}


void* removeBackward(DLinkedList* dLinkedList){
  if(dLinkedList->current == NULL) {
  return NULL;
  }
  LLNode* removeNode = dLinkedList->current;
  if(dLinkedList->head == dLinkedList->tail){
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->head){
    dLinkedList->head = (dLinkedList->current)->next;
    (dLinkedList->head)->previous = NULL;
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->tail){
    dLinkedList->tail = (dLinkedList->current)->previous;
    (dLinkedList->tail)->next = NULL;
    dLinkedList->current = dLinkedList->tail;
  }else{
    ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
    ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
    dLinkedList->current = (dLinkedList->current)->previous;
  }
  void* removeData = removeNode->data;
  free(removeNode);
  dLinkedList->size--;
  return removeData;
}

void* removeForward(DLinkedList* dLinkedList){
  if(dLinkedList->current == NULL) {
    return NULL;
  }
  LLNode* removeNode = dLinkedList->current;
  if(dLinkedList->head == dLinkedList->tail){
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->tail){
    dLinkedList->tail = (dLinkedList->current)->previous;
    (dLinkedList->tail)->next = NULL;
    dLinkedList->current = NULL;
  }else if(dLinkedList->current == dLinkedList->head){
    dLinkedList->head = (dLinkedList->current)->next;
    (dLinkedList->head)->previous = NULL;
    dLinkedList->current = dLinkedList->head;
  }else{
    ((dLinkedList->current)->previous)->next = (dLinkedList->current)->next;
    ((dLinkedList->current)->next)->previous = (dLinkedList->current)->previous;
    dLinkedList->current = (dLinkedList->current)->next;
  }
  void* removeData = removeNode->data;
  free(removeNode);
  dLinkedList->size--;
  return removeData;
}


void destroyList(DLinkedList* dLinkedList){
  if((dLinkedList->head) != NULL){
    getHead(dLinkedList);
    while(deleteForward(dLinkedList)){};
  }
  free(dLinkedList);
}


void* getHead(DLinkedList* dLinkedList){
    if((dLinkedList->head) == NULL){
      return NULL;
    }else{
      dLinkedList->current = dLinkedList->head;
      return (dLinkedList->head)->data;
    }
    //Your code here.
}



void* getTail(DLinkedList* dLinkedList){
    if((dLinkedList->tail) == NULL){
      return NULL;
    }else{
      dLinkedList->current = dLinkedList->tail;
      return (dLinkedList->tail)->data;
    }
}



void* getCurrent(DLinkedList* dLinkedList){
    if((dLinkedList->current) == NULL){
        return NULL;
    }else {
        return (dLinkedList->current)->data;
    }
}



void* getNext(DLinkedList* dLinkedList){
  if((dLinkedList->current) == NULL){
    return NULL;
  }else if((dLinkedList->current)->next == NULL){
    dLinkedList->current = NULL;
    return NULL;
  }else {
    dLinkedList->current = (dLinkedList->current)->next;
    return dLinkedList->current->data;
  }
}



void* getPrevious(DLinkedList* dLinkedList){
  if(dLinkedList->current == NULL){
    return NULL;
  }else if((dLinkedList->current)->previous == NULL){
    dLinkedList->current = NULL;
    return NULL;
  }else {
    dLinkedList->current = (dLinkedList->current)->previous;
    return dLinkedList->current->data;
  }
}


int getSize(DLinkedList* dLinkedList){
  return dLinkedList->size;
}




