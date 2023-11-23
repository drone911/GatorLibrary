# Project Overview
- The ``Gator Library Management System`` is designed to efficiently ``manage books`` and their ``reservations``.
- It is written in ``C++`` which runs natively and also provides a granular control over memory. 
- To manage the books we implement a ``Red Black Tree``. Each node in the tree is a key-value pair (*bookId*, *BookData*).
- *Bookdata* consists of *book name*, *author name*, *availability*, *borrowed by*, and *reservations*.
- Also, reservations are a ``priority queue`` implemented using ``Binary Min heaps``.

# Project Structure
- [**Main.cpp**](https://github.com/drone911/gatorLibrary/blob/master/main.cpp)
    - Main controls the overall run of the program.
    - It creates the instance of our library class GatorLibrary.
    - It reads commands in the input file, parses them, and calls appropriate functions
on the library instance.
    - Furthermore, it manages the output file. It changes the cout buffer to point to the
output file buffer, and thus the library instance writes to a file and not the standard
output.
- [**GatorLibrary.h**](https://github.com/drone911/gatorLibrary/blob/master/gatorLibrary.h) & [**GatorLibrary.cpp**](https://github.com/drone911/gatorLibrary/blob/master/gatorLibrary.cpp):
    - The GatorLibrary class is defined here that stores books using RB tree.
    - The GatorLibrary Class implements our business logic comprising function such as *insertBook*, *printBook*, *borrowBook*, *returnBook*, etc.
    - It also implements private functions to rebalance the tree after inserts or deletes, rotate nodes, print Nodes, check parents Nodes, etc.
    - Structures that abstract a Red Black Tree Node (named *BookNode*) and the data about a book (named *BookData*) are also defined here.
- [**Reservations.h**](https://github.com/drone911/gatorLibrary/blob/master/reservations.h) & [**Reservations.cpp**](https://github.com/drone911/gatorLibrary/blob/master/reservations.cpp):
    - The Reservations class is defined here that stores Reservations using Min-Binary heap.
    - The Reservations class implements our business logic for inserting new reservations and popping the highest priority reservations.
    - It also implements private functions to heapify the heap again after inserts and deletes.
    - A structure to hold a reservation (named *Reservation*) and a function to compare two reservations are also defined
