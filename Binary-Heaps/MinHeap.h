#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <climits>

#define MAX_CAPACITY 1000  // Defines the maximum capacity of the heap

class MinHeap
{
private:
    int heap[MAX_CAPACITY];  // Array to store heap elements
    int size;                // Current number of elements in the heap

    /**
     * Sifts up the node at index i to maintain heap property.
     */
    void siftUp(int i)
    {
        /*(*Write your code here**/
        if(i==0) return;
        int parent_idx  = (i-1)/2;
        if(heap[i]<heap[parent_idx]){
            int temp = heap[i];
            heap[i] = heap[parent_idx];
            heap[parent_idx] = temp;
            siftUp(parent_idx);
        }

    }

    /**
     * Sifts down the node at index i to maintain heap property.
     */
    void siftDown(int i)
    {
        /**Write your code here**/
        if(i>size-1) return;
        int left_idx = 2*i+1;
        int right_idx = 2*i+2;
        int smallest_idx = i;
        if(left_idx<size-1 && heap[smallest_idx] > heap[left_idx]){
            smallest_idx = left_idx;
        }
        if(right_idx < size && heap[smallest_idx] > heap[right_idx]){
            smallest_idx = right_idx;
        }
        if(smallest_idx != i) {
            int temp = heap[smallest_idx];
            heap[smallest_idx]= heap[i];
            heap[i] = temp;
            siftDown(smallest_idx);
        }
    }

public:
    // Constructor initializes an empty heap
    MinHeap() : size(0) {}

    /**
     * Inserts a new element x into the heap.
     */
    void insert(int x)
    {
        /**Write your code here**/
        heap[size] = x;
        siftUp(size);
        size++;
    }

    /**
     * Returns the minimum element without removing it.
     */
    int findMin()
    {
        /**Write your code here**/
        if(size == 0) return -1;
        return heap[0];
    }

    /**
     * Removes and returns the minimum element from the heap.
     */
    int extractMin()
    {

        /**Write your code here**/
        if(size==0) return -1;
        int min = heap[0];
        heap[0] = heap[size-1];
        siftDown(0);
        size--;
        return min;
    }

    /**
     * Returns the number of elements in the heap.
     */
    int getSize()
    {
        /**Write your code here**/
        return size;
    }

    /**
     * Checks if the heap is empty.
     * Returns true if empty, false otherwise.
     */
    bool isEmpty()
    {
        /**Write your code here**/
        return size==0;
    }

    /**
     * Decreases the value of the element at index i to newValue.
     */
    void decreaseKey(int i, int newValue)
    {
        /**Write your code here**/
        if(i>size-1) return ;
        heap[i] = newValue;
        siftUp(i);
    }

    /**
     * Deletes the element at index i.
     */
    void deleteKey(int i)
    {
        /**Write your code here**/
        if(i > size-1 || i<0) return ;
        heap[i] = heap[size-1];
        size--;
        if(i < size){
            siftUp(i);
            siftDown(i);
        }
    }

    /**
     * Prints the heap's content to the output file.
     * Format: "elem1 elem2 elem3 ..." (space-separated)
     */
    void printHeap(std::ofstream &outfile)
    {
        /**Write your code here**/
        if(size == 0 ) return;
        for(int i = 0 ; i < size ; i++){
            if(i == size-1) {
                outfile<<heap[i]<<std::endl;
                continue;
            }
            outfile<<heap[i]<<" ";
        }

    }

    /**
     * Checks whether the Min Heap property is preserved.
     * Returns true if valid, false otherwise.
     */
    bool isValidMinHeap()
    {
        /**Write your code here**/
        for(int i = 0 ; i < size ; i++){
            int left_child = i*2+1;
            int right_child = i*2+2;
            if(left_child<size && heap[left_child] < heap[i]){
                return false;
            }
            if(right_child<size && heap[right_child] < heap[i]){
                return false;
            }
        }
        return true;
    }

    /**
     * Builds a heap from an unsorted array using bottom-up heapify.
     */
    void heapify(int arr[], int n)
    {
        /**Write your code here**/
        for(int i = 0  ; i < n ; i++){
            heap[i] = arr[i];
        }
        size = n;
        for(int i = (size-2)/2 ; i>=0 ; i--){
            siftDown(i);
        }
    }

    /**
     * Returns all elements in sorted (ascending) order.
     * The original heap should remain UNCHANGED after this operation.
     */
    void heapSort(std::ofstream &outfile)
    {
        /**Write your code here**/
        if(size == 0 ) return;
        MinHeap tempHeap;
        tempHeap.heapify(this->heap,this->size);
        for(int i = 0 ; i < size;i++){
            outfile<<tempHeap.extractMin()<<" ";
        }
        outfile<<std::endl;
    }

    /**
     * Replaces the minimum element with a new value x in a single operation.
     * Returns the old minimum value.
     */
    int replaceMin(int x)
    {
        /**Write your code here**/
        int l = heap[0];
        heap[0] = x;
        siftDown(0);
        return l;
    }
};

/*
1 20
1 10
1 15
8
9
3
1 5
8
2
8
10 5 40 30 10 20 25
8
9
11
12 8
8
*/

#endif // MINHEAP_H
