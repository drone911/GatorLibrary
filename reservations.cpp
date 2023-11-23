#include <iostream>
#include <string>
#include <vector>
#include "reservations.h"

using namespace std;

int compareReservation(Reservation &n1, Reservation &n2)
{
    if (n1.priority == n2.priority)
    {
        if (n1.timeOfReservation == n2.timeOfReservation)
        {
            return 0;
        }
        else
        {
            return n1.timeOfReservation - n2.timeOfReservation;
        }
    }
    else
    {
        return n1.priority - n2.priority;
    }
}

void Reservations::heapifyDown(int index)
{
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int i = index;

    if (leftChild < heap.size() && compareReservation(heap[leftChild], heap[i]) < 0)
    {
        i = leftChild;
    }
    if (rightChild < heap.size() && compareReservation(heap[rightChild], heap[i]) < 0)
    {
        i = rightChild;
    }
    if (i != index)
    {
        swap(heap[i], heap[index]);
        heapifyDown(i);
    }
}
void Reservations::heapifyUp(int index)
{
    int i = index;
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (compareReservation(heap[i], heap[parent]) < 0)
        {
            swap(heap[i], heap[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }
}

void Reservations::insertReservation(int priority, int patronId, time_t timeOfReservation)
{
    heap.push_back(Reservation(priority, patronId, timeOfReservation));
    heapifyUp(heap.size() - 1);
}

Reservation Reservations::popReservation()
{
    Reservation poppedReservation = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
    {
        heapifyDown(0);
    }
    return poppedReservation;
}
