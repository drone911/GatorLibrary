#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "gatorLibrary.h"
#include "reservations.h"

#define HELP_MESSAGE "Usage: gatorLibrary /path/to/testcases-file"
#define BOOK_NOT_FOUND(x) ("Book " + to_string(x) + " not found in the Library")

using namespace std;

string readLine(FILE *);
int performOperation(GatorLibrary *, string &);
string stripCharacters(string, char);
void tokenize(string, string tokens[]);
bool toBool(string str);

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

    // Create output file name
    string inputFileName = string(argv[1]);
    string outputFileName;

    size_t append_at = inputFileName.find('.');

    if (append_at == string::npos)
    {
        // Handle case when no extension is present
        outputFileName = inputFileName + "_output_file";
    }
    else
    {
        // Handle case when extension is present
        outputFileName = inputFileName.substr(0, append_at) + "_output_file" + inputFileName.substr(append_at);
    }

    // open the output file in trucate mode
    ofstream outputBufferStream(outputFileName, ios::trunc);

    // Save cout buffer object
    streambuf *coutBuf = cout.rdbuf();
    // Redirect cout to write to the output file
    cout.rdbuf(outputBufferStream.rdbuf());

    // Initialize Library instance
    GatorLibrary *library_instance = new GatorLibrary();

    string operation = readLine(inputFileStream);
    while (operation.length() > 0)
    {
        // 1cout<<operation<<endl;

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

    // Return cout to original value
    std::cout.rdbuf(coutBuf);
    fclose(inputFileStream);
}

int performOperation(GatorLibrary *library_instance, string &operation)
{
    // To hold the tokenized parameters
    string tokens[5];

    if (operation.find("PrintBooks") == 0)
    {
        // tokenize parameter into tokens array
        tokenize(operation.substr(operation.find("(")), tokens);

        // Parse tokens into integers, boolean, etc.
        int startBookId = stoi(tokens[0]);
        int endBookId = stoi(tokens[1]);

        // Call Corresponding Gator Library function
        library_instance->printBooks(startBookId, endBookId);
    }
    else if (operation.find("PrintBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);

        library_instance->printBookById(bookId);
    }
    else if (operation.find("InsertBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);
        string bookName = stripCharacters(tokens[1], '\"');
        string authorName = stripCharacters(tokens[2], '\"');
        bool availibility = toBool(stripCharacters(stripCharacters(tokens[3], ')'), '\"'));

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
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);

        library_instance->deleteBook(bookId);
    }
    else if (operation.find("FindClosestBook") == 0)
    {
        tokenize(operation.substr(operation.find("(")), tokens);
        int bookId = stoi(tokens[0]);

        library_instance->findClosestBook(bookId);
    }
    else if (operation.find("ColorFlipCount") == 0)
    {
        library_instance->printColorFlipCount();
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

void tokenize(string str, string tokens[])
{
    // remove curly brackets
    str.erase(0, 1);
    str.erase(str.length() - 1, 1);

    int pos_start = 0, pos_end;
    int token_index = 0;

    while ((pos_end = str.find(",", pos_start)) != string::npos)
    {
        while (str[pos_start] == ' ')
        {
            pos_start++;
        }
        if (str[pos_start] == '"')
        {
            pos_end = str.find('"', pos_start + 1);
        }
        string token = stripCharacters(str.substr(pos_start, pos_end - pos_start), ' ');
        if (token.length() > 0)
        {
            if (token[0] == '"')
            {
            }
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

string stripCharacters(string str, char toStrip)
{
    while (str[0] == toStrip)
    {
        str.erase(0, 1);
    }

    while (str[str.size() - 1] == toStrip)
    {
        str.erase(str.size() - 1, 1);
    }
    return str;
}
