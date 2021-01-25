#include <iostream>
#include <cstdlib>
using namespace std;
 
#define SIZE 100
 
class Queue
{
    int *arr;  
    int capacity;
    int head;
    int tail;
    int count;
 
public:
    Queue(int size = SIZE);
    ~Queue();
 
    int dequeue();
    void enqueue(int x);
    int size();
    bool isEmpty();
    bool isFull();
};