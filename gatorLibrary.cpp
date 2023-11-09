#include <iostream>

#define HELP_MESSAGE "Usage: gatorLibrary /path/to/testcases-file"

using namespace std;

class GatorLibrary;
string readLine(FILE *);
int performOperation(GatorLibrary *, string &);

class GatorLibrary
{
 
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
        
        
        if (terminate == 1) {
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

int performOperation(GatorLibrary *library_instance, string &operation)
{
    if (operation.find("PrintBook") == 0)
    {
    }
    else if (operation.find("PrintBooks") == 0)
    {
    }
    else if (operation.find("InsertBook") == 0)
    {
    }
    else if (operation.find("BorrowBook") == 0)
    {
    }
    else if (operation.find("ReturnBook") == 0)
    {
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
        cout<<"Program Terminated!!\n";
        return 1;
    }
    else
    {
        cerr << "Operation {" << operation << "} not found" << endl;
        return -1;
    }
    return 0;
}