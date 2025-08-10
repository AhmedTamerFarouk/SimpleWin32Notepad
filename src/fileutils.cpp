#include <string>
#include <fstream>
#include <sstream>

std::wstring ReadFileFromPath(const wchar_t* filepath)
{
    std::wifstream file(filepath);          // Input file stream is an object used to "read from" a file. 
                                            // ie Open file for reading wide (Unicode) characters
    if (!file)                              // could not open file ? return an empty wide string
    {
        return L"";                        
    }

    std::wstringstream wss;                 // string stream is an object used to read from or write to a string stored in memory.
    wss << file.rdbuf();                    // Read entire file contents into the wss
    return wss.str();                       // convert the wss into a string & return it
}

bool SaveFileToPath(const wchar_t* filepath,wchar_t buffer[])
{
    std::wofstream file(filepath);          // Output file stream is an object used to "write to" a file. 

    if (!file)                              // could not open file ? return false
    {
        return false;                        
    }

    file << buffer;                     // write the content of the buffer to the file
    return true;                       // convert the wss into a string & return it
}
