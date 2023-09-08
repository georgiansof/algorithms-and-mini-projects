#ifndef __PRIORITY_QUEUE_H
#define __PRIORITY_QUEUE_H

#include "globals.h"
#include <stdbool.h>

typedef struct pair {
    int cost;
    int index;
} pair;

typedef struct MinHeap {
    pair *arr;
    int size;
    int capacity;
} MinHeap;


typedef struct priority_queue {
    MinHeap *heap;
} priority_queue;

int get_parent_index(int i) {return (i-1)/2;}
int get_left_child(int i) {return (2*i) + 1;}
int get_right_child(int i) {return (2*i) + 2;}
int get_min(MinHeap *heap) {
    return heap->arr[0].index;
}

MinHeap* insert_minheap(MinHeap *heap, pair element) {
    if(heap->size == heap->capacity) {
        fprintf(stderr, "Heap is already full! Increase capacity.\n");
        return heap;
    }
    heap->size ++;
    heap->arr[heap->size - 1] = element;

    int current = heap->size - 1;
    while(current > 0 && heap->arr[get_parent_index(current)].cost > heap->arr[current].cost) {
        pair temp = heap->arr[get_parent_index(current)];
        heap->arr[get_parent_index(current)] = heap->arr[current];
        heap->arr[current] = temp;

        current = get_parent_index(current);
    }
    return heap;
}

void priority_queue_push(priority_queue *pq, int cost, int index) {
    pair elem;
    elem.cost = cost;
    elem.index = index;
    insert_minheap(pq->heap,elem);
}

bool priority_queue_empty(priority_queue *pq) {
    if(pq->heap->size==0) return true;
    return false;
}

int pq_peek(priority_queue *pq) {
    if(pq->heap->size > 0) return pq->heap->arr[0].index;
    
    fprintf(stderr,"Trying to peek while priority queue is empty!\n");
    exit(EXIT_FAILURE);
}


MinHeap* init_minheap(int capacity) {
    MinHeap* minheap = (MinHeap*) calloc(1,sizeof(MinHeap));
    minheap->arr = (pair*) calloc(capacity, sizeof(pair));
    minheap->capacity = capacity;
    minheap->size = 0;
    return minheap;
}

void free_minheap(MinHeap *heap) {
    if(!heap) 
        return;
    free(heap->arr);
    free(heap);
}

MinHeap* heapify(MinHeap *heap, int index) {
    if(heap->size <= 1) {
        return heap;
    }

    int left = get_left_child(index);
    int right = get_right_child(index);

    int smallest = index;

    if(left < heap->size && heap->arr[left].cost < heap->arr[index].cost)
        smallest = left;
    
    if(right < heap->size && heap->arr[right].cost < heap->arr[smallest].cost)
        smallest = right;

    if(smallest != index) {
        pair temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        heap = heapify(heap, smallest); 
    }
    return heap;
}

MinHeap* delete_minimum(MinHeap *heap) {
    if(!heap || heap->size==0) {
        return heap;
    }

    int size = heap->size;
    pair last_element = heap->arr[size-1];

    heap->arr[0] = last_element;
    heap->size --;
    size--;
    heap = heapify(heap, 0);
    return heap;
}

void pq_pop(priority_queue *pq) {
    delete_minimum(pq->heap);
}
#endif