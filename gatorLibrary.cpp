#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#define HELP_MESSAGE "Usage: gatorLibrary /path/to/testcases-file"
#define BOOK_NOT_FOUND(x) ("Book " + to_string(x) + "not found in the Library")

using namespace std;

class GatorLibrary;

class BookNode;
class BookData;

class Reservations;

string readLine(FILE *);
int performOperation(GatorLibrary *, string &);

struct Reservation
{
    int priority;
    int patronId;
    time_t timeOfReservation;
    Reservation(int priority, int patronId, time_t timeOfReservation) : priority(priority), patronId(patronId), timeOfReservation(timeOfReservation)
    {
    }
};

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
        return n1.patronId - n2.patronId;
    }
}

class Reservations
{

    void heapifyDown(int index)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int i = index;

        if (leftChild < heap.size() && compareReservation(heap[leftChild], heap[i]))
        {
            i = leftChild;
        }
        if (rightChild < heap.size() && compareReservation(heap[rightChild], heap[i]))
        {
            i = rightChild;
        }
        if (i != index)
        {
            swap(heap[i], heap[index]);
            heapifyDown(i);
        }
    }
    void heapifyUp(int index)
    {
        int i = index - 1;
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

public:
    vector<Reservation> heap;

    void insertReservation(int priority, int patronId, time_t timeOfReservation)
    {
        heap.push_back(Reservation(priority, patronId, timeOfReservation));
        heapifyUp(heap.size() - 1);
    }

    Reservation popReservation()
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
    friend string to_string(const Reservations &reservations)
    {
        if (reservations.heap.empty())
        {
            return "[]";
        }
        string result = "[";
        for (Reservation reservation : reservations.heap)
        {
            result += to_string(reservation.patronId);
            result += ',';
        }
        result[result.size() - 1] = ']';
        return result;
    }
};

struct BookData
{
    string bookName;
    string authorName;
    bool available;
    int borrowedBy;
    Reservations *reservations;

    BookData(string bookName, string authorName, bool available) : bookName(bookName), authorName(authorName), available(available), borrowedBy(-1), reservations(nullptr)
    {
    }
};

enum Color
{
    RED,
    BLACK
};

struct BookNode
{
    int bookId;
    BookData *bookData;
    BookNode *parent;
    BookNode *left;
    BookNode *right;
    Color color;
    BookNode(int bookId, BookData *bookData, BookNode *parent = nullptr) : bookId(bookId), bookData(bookData), parent(parent), left(nullptr), right(nullptr), color(Color::RED)
    {
    }
};

class GatorLibrary
{
    BookNode *head;

    void balanceAfterInsert(BookNode *nodeToBalance)
    {
        if (!checkParent(nodeToBalance))
        {
            // We are root, so we don't need to rebalance further
            return;
        }
        if (!checkGrandparent(nodeToBalance))
        {
            // We are a level below root, so we don't need to rebalance further
            return;
        }
        BookNode *parent = nodeToBalance->parent;
        BookNode *grandparent = getGrandparent(nodeToBalance);

        // Characterize the imbalance
        char X, Y, z;

        if (grandparent->bookId > parent->bookId)
        {
            X = 'L';
            if (grandparent->right == nullptr || grandparent->right->color == Color::BLACK)
            {
                z = 'b';
            }
            else
            {
                z = 'r';
            }
        }
        else
        {
            X = 'R';
            if (grandparent->left == nullptr || grandparent->left->color == Color::BLACK)
            {
                z = 'b';
            }
            else
            {
                z = 'r';
            }
        }
        if (parent->bookId > nodeToBalance->bookId)
        {
            Y = 'L';
        }
        else
        {
            Y = 'L';
        }
        if (z == 'r')
        {
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            if (checkParent(grandparent))
            {
                balanceAfterInsert(grandparent->parent);
            }
        }
        else
        {
            if (X == 'L')
            {
                if (Y == 'L')
                {
                    LLRotate(nodeToBalance, parent, grandparent);
                }
                else
                {
                    LRRotate(nodeToBalance, parent, grandparent);
                }
            }
            else
            {
                if (Y == 'L')
                {
                    RLRotate(nodeToBalance, parent, grandparent);
                }
                else
                {
                    RRRotate(nodeToBalance, parent, grandparent);
                }
            }
        }
    }
    bool checkChild(BookNode *node)
    {
        return (node == nullptr);
    }
    bool checkParent(BookNode *node)
    {
        return (node->parent != nullptr);
    }

    bool checkGrandparent(BookNode *node)
    {
        return (checkParent(node) && checkParent(node->parent));
    }
    BookNode *getGrandparent(BookNode *node)
    {
        if (checkGrandparent(node))
        {
            return node->parent->parent;
        }
        return nullptr;
    }
    void LLRotate(BookNode *node, BookNode *parent, BookNode *grandparent)
    {
        // Check if grandparent is root or not
        if (checkParent(grandparent))
        {
            if (grandparent->parent->bookId > grandparent->bookId)
            {
                grandparent->parent->left = parent;
            }
            else
            {
                grandparent->parent->right = parent;
            }
            parent->parent = grandparent->parent;
            grandparent->parent = nullptr;
        }
        grandparent->parent = parent;
        grandparent->left = parent->right;

        parent->right = grandparent;

        parent->color = Color::BLACK;
        grandparent->color = Color::RED;
    }
    void LRRotate(BookNode *node, BookNode *parent, BookNode *grandparent)
    {
        // Check if grandparent is root or not
        if (checkParent(grandparent))
        {
            if (grandparent->parent->bookId > grandparent->bookId)
            {
                grandparent->parent->left = node;
            }
            else
            {
                grandparent->parent->right = node;
            }
            node->parent = grandparent->parent;
            grandparent->parent = nullptr;
        }
        parent->right = node->left;
        grandparent->left = node->right;

        node->left = parent;
        node->right = grandparent;

        parent->parent = node;
        grandparent->parent = node;

        node->color = Color::BLACK;
        grandparent->color = Color::RED;
    }
    void RLRotate(BookNode *node, BookNode *parent, BookNode *grandparent)
    {

        // Check if grandparent is root or not
        if (checkParent(grandparent))
        {
            if (grandparent->parent->bookId > grandparent->bookId)
            {
                grandparent->parent->left = node;
            }
            else
            {
                grandparent->parent->right = node;
            }
            node->parent = grandparent->parent;
            grandparent->parent = nullptr;
        }
        parent->left = node->right;
        grandparent->right = node->left;

        node->right = parent;
        node->left = grandparent;

        parent->parent = node;
        grandparent->parent = node;

        node->color = Color::BLACK;
        grandparent->color = Color::RED;
    }
    void RRRotate(BookNode *node, BookNode *parent, BookNode *grandparent)
    {
        // Check if grandparent is root or not
        if (checkParent(grandparent))
        {
            if (grandparent->parent->bookId > grandparent->bookId)
            {
                grandparent->parent->left = parent;
            }
            else
            {
                grandparent->parent->right = parent;
            }
            parent->parent = grandparent->parent;
            grandparent->parent = nullptr;
        }
        grandparent->parent = parent;
        grandparent->right = parent->left;

        parent->left = grandparent;

        parent->color = Color::BLACK;
        grandparent->color = Color::RED;
    }
    void printNode(BookNode *node)
    {
        BookData *bookData = node->bookData;

        cout << "BookId = " << node->bookId << endl
             << "Title = \"" << bookData->bookName << "\"" << endl
             << "Author = \"" << bookData->authorName << "\"" << endl
             << "Availability = \"" << (bookData->available ? "Yes" : "No") << "\"" << endl
             << "BorrowedBy = " << ((bookData->borrowedBy) ? to_string(bookData->borrowedBy) : "None") << "\"" << endl
             << "Reservations = " << ((bookData->reservations) ? to_string(*bookData->reservations) : "[]") << "\"" << endl;
    }
    BookNode *findNode(int bookId)
    {
        BookNode *current = head;
        while (true)
        {
            if (current->bookId == bookId)
            {
                return current;
            }
            else if (current->bookId > bookId)
            {
                if (current->left != nullptr)
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
                if (current->right != nullptr)
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

public:
    GatorLibrary()
    {
        head = nullptr;
    }
    void insertBook(int bookId, string bookName, string authorName, bool available)
    {
        cout << bookId << "-" << endl;
        cout << bookName << "-" << endl;
        cout << authorName << "-" << endl;
        cout << available << "-\n\n"
             << endl;

        BookNode *newNode = new BookNode(bookId, new BookData(bookName, authorName, available));

        if (head == nullptr)
        {
            newNode->color = Color::BLACK;
            head = newNode;
            return;
        }

        // Insert the new Red node at the appropriate positon
        BookNode *current = head;

        while (true)
        {
            if (current->bookId > newNode->bookId)
            {
                if (current->left != nullptr)
                {
                    current = current->left;
                    break;
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
                if (current->right != nullptr)
                {
                    current = current->right;
                    break;
                }
                else
                {
                    newNode->parent = current;
                    current->right = newNode;
                    break;
                }
            }
        }
        if (newNode->parent->color == Color::RED)
        {
            balanceAfterInsert(newNode);
        }
    }

    void printBooks(int startBookId, int endBookId)
    {
        for (int bookId = startBookId; bookId <= endBookId; bookId++)
        {
            printBookById(bookId);
        }
    }

    void printBookById(int bookId)
    {
        BookNode *node = findNode(bookId);
        if (node)
        {
            printNode(node);
        }
        else
        {
            cout << BOOK_NOT_FOUND(bookId) << endl;
        }
    }
    void borrowBook(int patronId, int bookId, int patronPriority)
    {
        BookNode *node = findNode(bookId);
        if (node->bookData->available == true)
        {
            node->bookData->available = false;
            node->bookData->borrowedBy = patronId;
            cout << "Book " << node->bookId << " Borrowed by Patron " << patronId << endl;
        }
        else
        {
            // Get the count of seconds
            auto currentTimePoint = chrono::system_clock::now();
            auto epoch = currentTimePoint.time_since_epoch();
            auto seconds = chrono::duration_cast<chrono::seconds>(epoch);

            time_t currentTimeInSeconds = seconds.count();

            if (node->bookData->reservations == nullptr)
            {
                node->bookData->reservations = new Reservations();
            }
            node->bookData->reservations->insertReservation(patronPriority, patronId, currentTimeInSeconds);
            cout << "Book " << node->bookId << " Reserved by Patron " << patronId << endl;
        }
    }
    void returnBook(int patronId, int bookId)
    {
        BookNode *node = findNode(bookId);

        node->bookData->borrowedBy = -1;
        node->bookData->available = true;

        cout << "Book " << bookId << " Returned by Patron " << patronId << endl;

        // If reservations are available
        if (!node->bookData->reservations->heap.empty())
        {
            Reservation nextReservation = node->bookData->reservations->popReservation();

            node->bookData->borrowedBy = nextReservation.patronId;
            node->bookData->available = false;

            cout << "Book " << bookId << " Allotted to Patron " << nextReservation.patronId << endl;
        }
    }
};

int main(int argc, char *argv[])
{

    // Check Input Args
    if (argc < 2)
    {
        cout << HELP_MESSAGE;
        return 1;
    }

    // Read input file
    FILE *inputFileStream = fopen(argv[1], "r");
    if (inputFileStream == NULL)
    {
        cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Initialize Library instance
    GatorLibrary *library_instance = new GatorLibrary();

    string operation = readLine(inputFileStream);
    while (operation.length() > 0)
    {
        cout << operation << endl;
        const int terminate = performOperation(library_instance, operation);

        if (terminate == 1)
        {
            delete library_instance;
            fclose(inputFileStream);
            return 0;
        }
        // Terminate with error
        if (terminate == -1)
        {
            delete library_instance;
            fclose(inputFileStream);
            return 1;
        }

        // Read next operation
        operation = readLine(inputFileStream);
    }

    fclose(inputFileStream);
}

string readLine(FILE *inputFileStream)
{
    string buffer;
    int ch;
    while ((ch = fgetc(inputFileStream)) != EOF)
    {
        if ((char)ch == '\n')
        {
            break;
        }
        buffer += ch;
    }
    return buffer;
}

string stripCharacters(string str, char toStrip)
{
    int space_pos = str.find(toStrip);
    while (space_pos != string::npos)
    {
        str.erase(space_pos, 1);
        space_pos = str.find(toStrip, space_pos + 1);
    }
    return str;
}

void tokenize(string str, string tokens[])
{
    // remove curly brackets
    str.erase(0, 1);
    str.erase(str.length() - 1, 1);

    int pos_start = 0, pos_end;
    int token_index = 0;

    while ((pos_end = str.find(",", pos_start)) != string::npos)
    {
        string token = stripCharacters(str.substr(pos_start, pos_end - pos_start), ' ');
        if (token.length() > 0)
        {
            tokens[token_index] = token;
            token_index += 1;
        }
        pos_start = pos_end + 1;
    }
    tokens[token_index] = stripCharacters(str.substr(pos_start, pos_end - pos_start), ' ');
}

bool toBool(string str)
{
    if (str == "Yes")
    {
        return true;
    }
    return false;
}

int performOperation(GatorLibrary *library_instance, string &operation)
{
    // To hold the tokenized parameters
    string tokens[5];

    if (operation.find("PrintBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);

        library_instance->printBookById(bookId);
    }
    else if (operation.find("PrintBooks") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int startBookId = stoi(tokens[0]);
        int endBookId = stoi(tokens[1]);

        library_instance->printBooks(startBookId, endBookId);
    }
    else if (operation.find("InsertBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);
        string bookName = stripCharacters(tokens[1], '\"');
        string authorName = stripCharacters(tokens[2], '\"');
        bool availibility = toBool(stripCharacters(tokens[3], '\"'));

        library_instance->insertBook(bookId, bookName, authorName, availibility);
    }
    else if (operation.find("BorrowBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int patronId = stoi(tokens[0]);
        int bookId = stoi(tokens[1]);
        int patronPriority = stoi(tokens[2]);

        library_instance->borrowBook(patronId, bookId, patronPriority);
    }
    else if (operation.find("ReturnBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int patronId = stoi(tokens[0]);
        int bookId = stoi(tokens[1]);

        library_instance->returnBook(patronId, bookId);
    }
    else if (operation.find("DeleteBook") == 0)
    {
    }
    else if (operation.find("FindClosestBook") == 0)
    {
    }
    else if (operation.find("ColorFlipCount") == 0)
    {
    }
    else if (operation.find("Quit") == 0)
    {
        cout << "Program Terminated!!\n";
        return 1;
    }
    else
    {
        cerr << "Operation {" << operation << "} not found" << endl;
        return -1;
    }
    return 0;
}