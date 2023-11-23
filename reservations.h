#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// Structure to hold a reservation. A reservation node in the heap stores the priority, patronId, and the timeOfReservation (in epoch) 
struct Reservation
{
    int priority;
    int patronId;
    time_t timeOfReservation;
    Reservation(int priority, int patronId, time_t timeOfReservation) : priority(priority), patronId(patronId), timeOfReservation(timeOfReservation)
    {
    }
};

// Function to compare two reservations. Using priority as the compare key and tie breaking using timeOfReservation 
int compareReservation(Reservation &n1, Reservation &n2);

// Class that implements the priority queue data structure, which holds the reservations in a min binary heap
class Reservations
{
private:
    // Private method that heapify's the heap in the downward direction. Used when we insert a node at the start of the heap
    void heapifyDown(int index);

    // Private method that heapify's the heap in the downward direction. Used when we insert a node at the end of the heap
    void heapifyUp(int index);

public:
    vector<Reservation> heap;


    void insertReservation(int priority, int patronId, time_t timeOfReservation);
    Reservation popReservation();

    // A friend function that overrides to_string for this class. We can directly give Reservations class object to cout this way
    friend string to_string(const Reservations &reservations)
    {
        if (reservations.heap.empty())
        {
            return "[]";
        }
        string result = "";
        for (auto reservation = reservations.heap.begin(); reservation != reservations.heap.end(); ++reservation)
        {
            result += to_string((*reservation).patronId);
            result += ", ";
        }
        result.erase(result.size() - 2, 2);
        return result;
    }
};