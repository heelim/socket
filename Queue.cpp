#include "Queue.h"
 
// Constructor 
Queue::Queue(int size)
{
    arr = new int[size];
    capacity = size;
    head = 0;
    tail = -1;
    count = 0;
}
 
// Destructor
Queue::~Queue()
{
    delete[] arr;
}
 
int Queue::dequeue()
{
    int ret;
    if (isEmpty()) {
        exit(EXIT_FAILURE);
    }
 
    ret = arr[head];
    head = (head + 1) % capacity;
    count--;

    return ret;
}
 
void Queue::enqueue(int item)
{
    if (isFull()) {
        exit(EXIT_FAILURE);
    }
    tail = (tail + 1) % capacity;
    arr[tail] = item;
    count++;
}
 
int Queue::size()
{
    return count;
}
 
bool Queue::isEmpty()
{
    return (size() == 0);
}
 
bool Queue::isFull()
{
    return (size() == capacity);
}
