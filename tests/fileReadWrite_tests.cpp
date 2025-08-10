#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include "fileutils.h"
#include "resource.h"

void createTestFileUsingPathAndContent(const wchar_t* filePath, const std::wstring& stringContent)
{
    std::wofstream file(filePath); // output file stream is used to "write to" a file
    file << stringContent;  // write the string to the file
}

TEST(FileTests,Read)
{
    std::wstring expectedContent = L"Hello";
    const wchar_t* filePath = L"textfile.txt";

    // create a file using the above path and string content
    createTestFileUsingPathAndContent(filePath,expectedContent);

    // call my implemented function
    std::wstring contentReadFromFile = ReadFileFromPath(filePath);

    // check
    ASSERT_EQ(contentReadFromFile,expectedContent);

}

TEST(FileTests,Write)
{
    const wchar_t* filePath = L"textfile.txt";
    // create a buffer that holds the expected text
    wchar_t buffer[] = L"Hello";

    // call my implemented function
    bool contentWroteToFile = SaveFileToPath(filePath,buffer);

    // check success
    ASSERT_TRUE(contentWroteToFile);

    // give an inavlid path 
    const wchar_t* wrongfilePath = L"//\\[]fd[fopdjfjdfhjdhfoj]";

    // call my implemented function
    bool contentWroteToFileAgain = SaveFileToPath(wrongfilePath,buffer);

    // check fail
    ASSERT_FALSE(contentWroteToFileAgain);
}