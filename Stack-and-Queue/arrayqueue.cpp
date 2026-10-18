#include "queue.h"
#include <iostream>
using namespace std;
// Constructor implementation
ArrayQueue::ArrayQueue(int initial_capacity)
{
    // TODO: Initialize data members (data, capacity, front_idx, rear_idx)
    // TODO: Allocate memory for the array with the specified initial capacity
    capacity = initial_capacity;
    data = new int[capacity];
    front_idx = 0;
    rear_idx=0;
}

// Destructor implementation
ArrayQueue::~ArrayQueue()
{
    // TODO: Free the dynamically allocated memory for the array
    delete[] data;
}

// Enqueue implementation (add an item to the rear of the queue)
void ArrayQueue::enqueue(int item)
{
    // TODO: Check if the array is full
    // TODO: If full, resize the array to double its current capacity
    if((rear_idx+capacity+1)%capacity==front_idx){
        resize(capacity*2);
    }
    // TODO: Add the new element to the rear of the queue
    data[rear_idx] = item;
    rear_idx=(rear_idx+1)%capacity;
}

// Dequeue implementation (remove an item from the front of the queue)
int ArrayQueue::dequeue()
{
    // TODO: Check if the queue is empty, display error message if it is
    if(front_idx==rear_idx){
        cout<<"Error! queue is already empty"<<endl;
        return -1;
    }
    // TODO: Decrement current_size and return the element at the front of the queue
    int val = data[front_idx];
    // TODO: Update front index
    front_idx=(front_idx+1)%capacity;
    // TODO: If the array is less than 25% full, resize it to half its current capacity (but not less than 2)
    int size = (capacity+rear_idx-front_idx) %capacity;
    if(capacity/2 >= 2 && size<capacity/4){
        resize(capacity/2);
    }
    // TODO: Return the dequeued element
    return val;
}

// Clear implementation
void ArrayQueue::clear()
{
    // TODO: Reset the queue to be empty (reset capacity, front_idx, rear_idx, data)
    delete[] data;
    capacity=2;
    data = new int[capacity];
    front_idx=0;
    rear_idx=0;
}

// Size implementation
int ArrayQueue::size() const
{
    // TODO: Return the number of elements currently in the queue
    return (rear_idx-front_idx+capacity)%capacity;
}

// Front implementation
int ArrayQueue::front() const
{
    // TODO: Check if the queue is empty, display error message if it is
    if(size() == 0){
        cout<<"Error! queue is already empty"<<endl;
        return -1;
    }
    // TODO: Return the element at the front of the queue without removing it
    return data[front_idx];
}

// Back implementation (get the element at the back of the queue)
int ArrayQueue::back() const
{
    // TODO: Check if the queue is empty, display error message if it is
    if(size() == 0){
        cout<<"Error! queue is already empty"<<endl;
        return -1;
    }
    // TODO: Return the element at the back of the queue without removing it
    return data[rear_idx-1];
}

// Empty implementation
bool ArrayQueue::empty() const
{
    // TODO: Return whether the queue is empty (current_size == 0)
    return size()==0;
}

// Print implementation
string ArrayQueue::toString() const
{
    // TODO: Convert queue to a string representation in the format: <elem1, elem2, ..., elemN|
    string str = "<";
    int siz = size();
    for(int i = 0 ; i<siz;i++){
        int idx = (i+front_idx)%capacity;
        str+=to_string(data[idx]);
        if(i == siz-1){ continue;}
        str+=",";
    }
    str+="|";
    return str;
}

// Resize implementation
void ArrayQueue::resize(int new_capacity)
{
    // TODO: Create a new array with the new capacity
    int*temp = new int[new_capacity] ;
    // TODO: Copy elements from the old array to the new array
    int siz = size();
    for(int i = 0 ; i < siz ; i++){
        int idx = (front_idx+i) %capacity;
        temp[i]=data[idx];
    }
    // TODO: Delete the old array
    delete[] data;
    data = temp;
    // TODO: Update the data pointer and capacity
    capacity=new_capacity;
    // TODO: Update front and rear indices
    front_idx=0;
    rear_idx=siz;
}

int ArrayQueue::getCapacity() const
{
    // TODO: Return the current capacity of the queue
    return capacity;
}