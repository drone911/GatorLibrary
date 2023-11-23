# Project Overview
The ``Gator Library Management System`` is designed to efficiently ``manage books`` and their
``reservations``. To manage the books we implement a ``Red Black Tree``. *BookId* is the key and book
data (consisting of *book name*, *author name*, *availability*, *borrowed by*, *reservations*) is the
dictionary value. Also, each book node maintains a ``priority queue`` of reservations implemented
using ``Binary Min heaps``.

# Project Structure
- **Main.cpp**
    - Main controls the overall run of the program.
    - It creates the instance of our library class GatorLibrary.
    - It reads commands in the input file, parses them, and calls appropriate functions
on the library instance.
    - Furthermore, it manages the output file. It changes the cout buffer to point to the
output file buffer, and thus the library instance writes to a file and not the standard
output.
- **GatorLibrary.h & GatorLibrary.cpp**
    - The GatorLibrary class is defined here that stores books using RB tree.
    - The GatorLibrary Class implements our business logic comprising function such as *insertBook*, *printBook*, *borrowBook*, *returnBook*, etc.
    - It also implements private functions to rebalance the tree after inserts or deletes, rotate nodes, print Nodes, check parents Nodes, etc.
    - Structures that abstract a Red Black Tree Node (named *BookNode*) and the data about a book (named *BookData*) are also defined here.
- **Reservations.h & Reservations.cpp:**
    - The Reservations class is defined here that stores Reservations using Min-Binary heap.
    - The Reservations class implements our business logic for inserting new reservations and popping the highest priority reservations.
    - It also implements private functions to heapify the heap again after inserts and deletes.
    - A structure to hold a reservation (named *Reservation*) and a function to compare two reservations are also defined