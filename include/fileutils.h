#pragma once
#include <string>

std::wstring ReadFileFromPath(const wchar_t* filepath);

bool SaveFileToPath(const wchar_t* filepath,wchar_t buffer[]);