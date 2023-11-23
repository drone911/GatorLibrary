#include <iostream>
#include <string>
#include <chrono>

#include "gatorLibrary.h"
#include "reservations.h"

#define BOOK_NOT_FOUND(x) ("Book " + to_string(x) + " not found in the Library")

using namespace std;

BookData::BookData(string bookName, string authorName, bool available) : bookName(bookName), authorName(authorName), available(available), borrowedBy(-1), reservations(nullptr)
{
}
BookData::~BookData()
{
    if (reservations)
    {
        delete reservations;
    }
}

BookNode::BookNode(int bookId, BookData *bookData, BookNode *parent = nullptr) : bookId(bookId), bookData(bookData), parent(parent), left(nullptr), right(nullptr), color(Color::RED)
{
}

GatorLibrary::GatorLibrary()
{
    nullNode = new BookNode(0, nullptr);
    nullNode->color = Color::BLACK;
    head = nullNode;
}

void GatorLibrary::balanceAfterInsert(BookNode *nodeToBalance)
{
    while (nodeToBalance != head && nodeToBalance->parent->color == Color::RED)
    {
        if (nodeToBalance->parent == nodeToBalance->parent->parent->left)
        {
            BookNode *parentSibling = nodeToBalance->parent->parent->right;
            if (parentSibling != nullNode && parentSibling->color == Color::RED)
            {
                setColor(nodeToBalance->parent, Color::BLACK);
                setColor(parentSibling, Color::BLACK);
                setColor(nodeToBalance->parent->parent, Color::RED);
                nodeToBalance = nodeToBalance->parent->parent;
            }
            else
            {
                if (nodeToBalance == nodeToBalance->parent->right)
                {
                    nodeToBalance = nodeToBalance->parent;
                    rotateLeft(nodeToBalance);
                }
                setColor(nodeToBalance->parent, Color::BLACK);
                setColor(nodeToBalance->parent->parent, Color::RED);
                rotateRight(nodeToBalance->parent->parent);
            }
        }
        else
        {
            BookNode *parentSibling = nodeToBalance->parent->parent->left;
            if (parentSibling != nullNode && parentSibling->color == Color::RED)
            {
                setColor(nodeToBalance->parent, Color::BLACK);
                setColor(parentSibling, Color::BLACK);
                setColor(nodeToBalance->parent->parent, Color::RED);
                nodeToBalance = nodeToBalance->parent->parent;
            }
            else
            {
                if (nodeToBalance == nodeToBalance->parent->left)
                {
                    nodeToBalance = nodeToBalance->parent;
                    rotateRight(nodeToBalance);
                }
                setColor(nodeToBalance->parent, Color::BLACK);
                setColor(nodeToBalance->parent->parent, Color::RED);
                rotateLeft(nodeToBalance->parent->parent);
            }
        }
    }
    head->color = Color::BLACK;
}

bool GatorLibrary::checkChild(BookNode *node)
{
    return (node == nullptr);
}

bool GatorLibrary::checkParent(BookNode *node)
{
    return (node->parent != nullptr);
}

bool GatorLibrary::checkGrandparent(BookNode *node)
{
    return (checkParent(node) && checkParent(node->parent));
}

BookNode *GatorLibrary::getGrandparent(BookNode *node)
{
    if (checkGrandparent(node))
    {
        return node->parent->parent;
    }
    return nullptr;
}

void GatorLibrary::printNode(BookNode *node)
{
    BookData *bookData = node->bookData;
    cout << "BookID = " << node->bookId << endl
         << "Title = \"" << bookData->bookName << "\"" << endl
         << "Author = \"" << bookData->authorName << "\"" << endl
         << "Availability = \"" << (bookData->available ? "Yes" : "No") << "\"" << endl
         << "BorrowedBy = " << ((bookData->borrowedBy != -1) ? to_string(bookData->borrowedBy) : "None") << endl
         << "Reservations = " << ((bookData->reservations) ? "[" + to_string(*bookData->reservations) + "]" : "[]") << endl
         << endl;
}
BookNode *GatorLibrary::findNode(int bookId)
{
    BookNode *current = head;
    if (current == nullNode)
    {
        return nullptr;
    }
    while (true)
    {
        if (current->bookId == bookId)
        {
            return current;
        }
        else if (current->bookId > bookId)
        {
            if (current->left != nullNode)
            {
                current = current->left;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            if (current->right != nullNode)
            {
                current = current->right;
            }
            else
            {
                return nullptr;
            }
        }
    }
}

void GatorLibrary::deleteBookNode(BookNode *nodeToDelete)
{
    Reservations *reservations = nodeToDelete->bookData->reservations;
    if (reservations)
    {
        if (!reservations->heap.empty())
        {
            if (reservations->heap.size() == 1)
            {
                cout << "Book " << nodeToDelete->bookId << " is no longer available. Reservation made by Patron " << to_string(*reservations) << " has been cancelled!" << endl
                     << endl;
            }
            else
            {
                cout << "Book " << nodeToDelete->bookId << " is no longer available. Reservations made by Patrons " << to_string(*reservations) << " have been cancelled!" << endl
                     << endl;
            }
        }
        else
        {
            cout << "Book " << nodeToDelete->bookId << " is no longer available" << endl
                 << endl;
        }
        delete reservations;
    }
    else
    {
        cout << "Book " << nodeToDelete->bookId << " is no longer available" << endl
             << endl;
    }
    delete nodeToDelete;
}

void GatorLibrary::rotateLeft(BookNode *node)
{
    BookNode *rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullNode)
    {
        rightChild->left->parent = node;
    }

    rightChild->parent = node->parent;

    if (node->parent == nullptr)
    {
        head = rightChild;
    }
    else if (node->bookId < node->parent->bookId)
    {
        node->parent->left = rightChild;
    }
    else
    {
        node->parent->right = rightChild;
    }

    rightChild->left = node;
    node->parent = rightChild;
}

void GatorLibrary::rotateRight(BookNode *node)
{
    BookNode *leftChild = node->left;
    if (leftChild == nullNode)
    {
        return;
    }
    node->left = leftChild->right;

    if (leftChild->right != nullNode)
    {
        leftChild->right->parent = node;
    }

    leftChild->parent = node->parent;

    if (node->parent == nullptr)
    {
        head = leftChild;
    }
    else if (node->bookId < node->parent->bookId)
    {
        node->parent->left = leftChild;
    }
    else
    {
        node->parent->right = leftChild;
    }

    leftChild->right = node;
    node->parent = leftChild;
}

void GatorLibrary::insertBook(int bookId, string bookName, string authorName, bool available)
{
    BookNode *newNode = new BookNode(bookId, new BookData(bookName, authorName, available));
    newNode->left = nullNode;
    newNode->right = nullNode;
    if (head == nullNode)
    {
        head = newNode;
        newNode->color = Color::BLACK;
        return;
    }

    // Insert the new Red node at the appropriate positon
    BookNode *current = head;

    while (true)
    {

        if (current->bookId > newNode->bookId)
        {

            if (current->left != nullNode)
            {
                current = current->left;
            }
            else
            {
                newNode->parent = current;
                current->left = newNode;
                break;
            }
        }
        else
        {
            if (current->right != nullNode)
            {
                current = current->right;
            }
            else
            {
                newNode->parent = current;
                current->right = newNode;
                break;
            }
        }
    }
    if (newNode->parent->parent == nullptr)
    {
        return;
    }

    balanceAfterInsert(newNode);
}

void GatorLibrary::printBooks(int startBookId, int endBookId)
{
    BookNode *current = head;
    printBooksInOrder(current, startBookId, endBookId);
}

void GatorLibrary::printBooksInOrder(BookNode *current, int startBookId, int endBookId)
{
    if (current == nullNode)
    {
        return;
    }
    if (current->bookId > startBookId)
    {
        printBooksInOrder(current->left, startBookId, endBookId);
    }
    if (current->bookId >= startBookId && current->bookId <= endBookId)
    {
        printNode(current);
    }
    if (current->bookId < endBookId)
    {
        printBooksInOrder(current->right, startBookId, endBookId);
    }
}

void GatorLibrary::printBookById(int bookId)
{
    BookNode *node = findNode(bookId);
    if (node)
    {
        printNode(node);
    }
    else
    {
        cout << BOOK_NOT_FOUND(bookId) << endl
             << endl;
    }
}
void GatorLibrary::borrowBook(int patronId, int bookId, int patronPriority)
{
    BookNode *node = findNode(bookId);
    if (node->bookData->available == true)
    {
        node->bookData->available = false;
        node->bookData->borrowedBy = patronId;
        cout << "Book " << node->bookId << " Borrowed by Patron " << patronId << endl
             << endl;
    }
    else
    {
        // Get the current count of seconds
        auto currentTimePoint = chrono::system_clock::now();
        auto epoch = currentTimePoint.time_since_epoch();
        auto seconds = chrono::duration_cast<chrono::seconds>(epoch);

        time_t currentTimeInSeconds = seconds.count();

        if (node->bookData->reservations == nullptr)
        {
            node->bookData->reservations = new Reservations();
        }
        node->bookData->reservations->insertReservation(patronPriority, patronId, currentTimeInSeconds);
        cout << "Book " << node->bookId << " Reserved by Patron " << patronId << endl
             << endl;
    }
}
void GatorLibrary::returnBook(int patronId, int bookId)
{
    BookNode *node = findNode(bookId);

    node->bookData->borrowedBy = -1;
    node->bookData->available = true;

    cout << "Book " << bookId << " Returned by Patron " << patronId << endl
         << endl;

    // If reservations are available
    if (node->bookData->reservations && !node->bookData->reservations->heap.empty())
    {
        Reservation nextReservation = node->bookData->reservations->popReservation();

        node->bookData->borrowedBy = nextReservation.patronId;
        node->bookData->available = false;

        cout << "Book " << bookId << " Allotted to Patron " << nextReservation.patronId << endl
             << endl;
    }
}
void GatorLibrary::deleteBook(int bookId)
{
    BookNode *nodeToDelete = findNode(bookId);
    BookNode *balanceAt;
    Color nodeToDeleteOriginalColor = nodeToDelete->color;

    if (nodeToDelete->left == nullNode)
    {
        balanceAt = nodeToDelete->right;
        deleteTransplant(nodeToDelete, nodeToDelete->right);
    }
    else if (nodeToDelete->right == nullNode)
    {

        balanceAt = nodeToDelete->left;
        deleteTransplant(nodeToDelete, nodeToDelete->left);
    }
    else
    {
        // Node has two children, find biggest node in the left subtree
        BookNode *newNodeToDelete = nodeToDelete->left;

        while (newNodeToDelete->right != nullNode)
        {
            newNodeToDelete = newNodeToDelete->right;
        }

        nodeToDeleteOriginalColor = newNodeToDelete->color;
        balanceAt = newNodeToDelete->left;

        if (newNodeToDelete->parent == nodeToDelete)
        {
            if (balanceAt != nullNode)
            {
                balanceAt->parent = newNodeToDelete;
            }
        }
        else
        {
            deleteTransplant(newNodeToDelete, newNodeToDelete->left);
            newNodeToDelete->left = nodeToDelete->left;
            newNodeToDelete->left->parent = newNodeToDelete;
        }

        deleteTransplant(nodeToDelete, newNodeToDelete);
        newNodeToDelete->right = nodeToDelete->right;
        newNodeToDelete->right->parent = newNodeToDelete;
        newNodeToDelete->color = nodeToDelete->color;
    }
    // Balance the tree
    if (nodeToDeleteOriginalColor == Color::BLACK)
    {
        balanceAfterDelete(balanceAt);
    }

    deleteBookNode(nodeToDelete);
}

void GatorLibrary::deleteTransplant(BookNode *nodeToDelete, BookNode *nodeToDeleteChild)
{

    if (nodeToDelete->parent == nullptr)
    {
        head = nodeToDeleteChild;
    }
    else if (nodeToDelete->bookId < nodeToDelete->parent->bookId)
    {
        nodeToDelete->parent->left = nodeToDeleteChild;
    }
    else
    {
        nodeToDelete->parent->right = nodeToDeleteChild;
    }

    if (nodeToDeleteChild != nullptr)
    {
        nodeToDeleteChild->parent = nodeToDelete->parent;
    }
}

// Function to fix the tree after deletion
void GatorLibrary::balanceAfterDelete(BookNode *current)
{
    while (current != head && (current == nullNode || current->color == Color::BLACK))
    {
        if (current == current->parent->left)
        {

            BookNode *sibling = current->parent->right;
            if (sibling == nullNode)
            {
                sibling->color = Color::BLACK;
            }
            if (sibling->color == Color::RED)
            {
                setColor(sibling, Color::BLACK);
                setColor(current->parent, Color::RED);
                rotateLeft(current->parent);
                sibling = current->parent->right;
            }

            if ((sibling->left == nullptr || sibling->left == nullptr || sibling->left->color == Color::BLACK) &&
                (sibling->right == nullptr || sibling->right == nullptr || sibling->right->color == Color::BLACK))
            {
                setColor(sibling, Color::RED);
                current = current->parent;
            }
            else
            {

                if (sibling->right == nullNode || sibling->right->color == Color::BLACK)
                {
                    if (sibling->left != nullNode)
                    {
                        setColor(sibling->left, Color::BLACK);
                    }
                    setColor(sibling, Color::RED);
                    rotateRight(sibling);
                    sibling = current->parent->right;
                }

                setColor(sibling, current->parent->color);
                current->parent->color = Color::BLACK;
                if (sibling->right != nullNode)
                {
                    sibling->right->color = Color::BLACK;
                }
                rotateLeft(current->parent);
                current = head;
            }
        }
        else
        {
            BookNode *sibling = current->parent->left;
            if (sibling == nullNode)
            {
                sibling->color = Color::BLACK;
            }
            if (sibling->color == Color::RED)
            {

                setColor(sibling, Color::BLACK);
                current->parent->color = Color::RED;
                rotateRight(current->parent);
                sibling = current->parent->left;
            }

            if ((sibling->right == nullNode || sibling->right == nullptr || sibling->right->color == Color::BLACK) &&
                (sibling->left == nullNode || sibling->left == nullptr || sibling->left->color == Color::BLACK))
            {
                setColor(sibling, Color::RED);
                current = current->parent;
            }
            else
            {
                if (sibling->left == nullNode || sibling->left->color == Color::BLACK)
                {
                    if (sibling->right != nullNode)
                    {
                        setColor(sibling->right, Color::BLACK);
                    }
                    setColor(sibling, Color::RED);
                    rotateLeft(sibling);
                    sibling = current->parent->left;
                }

                setColor(sibling, current->parent->color);
                current->parent->color = Color::BLACK;
                if (sibling->left != nullNode)
                {
                    setColor(sibling->left, Color::BLACK);
                }
                rotateRight(current->parent);
                current = head;
            }
        }
    }

    if (current != nullNode)
    {
        setColor(current, Color::BLACK);
    }
}

void GatorLibrary::findClosestBook(int bookId)
{

    BookNode *closest = nullptr;
    BookNode *current = head;

    while (true)
    {
        // update closest
        if (closest == nullptr || std::abs(current->bookId - bookId) < std::abs(closest->bookId - bookId))
        {
            closest = current;
        }

        if (bookId < current->bookId)
        {
            if (current->left != nullNode)
            {
                current = current->left;
            }
            else
            {
                break;
            }
        }
        else if (bookId > current->bookId)
        {
            if (current->right != nullNode)
            {
                current = current->right;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    // If we have two nodes with same distance
    if (std::abs(current->bookId - bookId) == std::abs(closest->bookId - bookId) && current->bookId != closest->bookId)
    {
        if (closest->bookId < current->bookId)
        {
            printNode(closest);
            printNode(current);
        }
        else
        {
            printNode(current);
            printNode(closest);
        }
    }
    else if (current->bookId == bookId)
    {
        printNode(current);
    }
    else
    {
        printNode(closest);
    }
}
void GatorLibrary::printColorFlipCount()
{
    cout << "Color Flip Count: " << colorFlipCount << endl
         << endl;
}