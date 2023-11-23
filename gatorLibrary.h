#pragma once
#include <iostream>
#include <string>

#include "reservations.h"

using namespace std;

// Enum to represent the colors of the red black tree
enum Color
{
    RED,
    BLACK
};

// Structure to hold book data: name, authorname, availibility, borrowed by, and a pointer to reservations
struct BookData
{
    string bookName;
    string authorName;
    bool available;
    int borrowedBy;
    Reservations *reservations;

    // constructor
    BookData(string, string, bool);
    // destructor
    ~BookData();
};

// Structure that represents a node in the red black tree. The key is bookId, data is bookData, and its color is enum color (BLACK, RED). Also contains pointers to left, right, and parent nodes.
struct BookNode
{
    int bookId;
    BookData *bookData;
    BookNode *parent;
    BookNode *left;
    BookNode *right;
    Color color;
    // default constructor
    BookNode(){};
    // constructor
    BookNode(int bookId, BookData *data, BookNode *parent);
};

// Class that contains our business logic for the Gator Library. Books are represented using BookNode and the root of the Red Black is at head. colorFlipCount maintains the count of color flips, and nullNode represents the black external nodes.
class GatorLibrary
{
private:
    BookNode *head;
    BookNode *nullNode;
    unsigned int colorFlipCount = 0;

    // Private method to check if child is present or not
    bool checkChild(BookNode *node);
    // Private method to check if parent is present or not
    bool checkParent(BookNode *node);
    // Private method to check if grandfather is present or not
    bool checkGrandparent(BookNode *node);
    // Private method to retrieve the grandfather node
    BookNode *getGrandparent(BookNode *node);

    // Private Method to Perform rebalancing after insert
    void balanceAfterInsert(BookNode *nodeToBalance);
    // Private Method to perform left rotation on the node
    void rotateLeft(BookNode *node);
    // Private Method to perform right rotation on the node
    void rotateRight(BookNode *node);

    // Private method to find a node iteratively
    BookNode *findNode(int bookId);

    // Private method to actually print the book data and reservations
    void printNode(BookNode *node);
    // Private method that prints books in a range by performing in-order traversal on the nodes in the range.
    void printBooksInOrder(BookNode *currentNode, int startBookId, int endBookId);

    // Private method to delete a book node from memory and empty its reservations
    void deleteBookNode(BookNode *nodeToDelete);
    // Private method to sever the nodeToDelete from its parent and child, and attach the child of that node with its grandparent
    void deleteTransplant(BookNode *nodeToDelete, BookNode *nodeToDeleteChild);
    // Private method to perform balancing of the tree at current node after delete
    void balanceAfterDelete(BookNode *current);

    // Increase color flip count when the Color toggles
    void setColor(BookNode *node, Color newColor)
    {
        if (node == nullptr || node->color == newColor)
        {
            return;
        }
        node->color = newColor;
        colorFlipCount++;
    }

public:
    // constructor
    GatorLibrary();

    void insertBook(int bookId, string bookName, string authorName, bool available);

    void printBookById(int bookId);
    void printBooks(int startBookId, int endBookId);

    void borrowBook(int patronId, int bookId, int patronPriority);
    void returnBook(int patronId, int bookId);

    void deleteBook(int bookId);

    void findClosestBook(int bookId);

    void printColorFlipCount();
};