/*
 * File: utils.hpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <direct.h>
#include <ctime>

namespace fileUtils {
	//read a file as txt
	std::string read_txt(const char* file_path);
	std::string read_txt(const WCHAR* file_path);

	//write string to a file
	bool write_txt(const char* file_path, std::string str);
	bool write_txt(const WCHAR* file_path, std::string str);

	//make directory
	int mkdir(const char* dir);

	//select a file in explorer
	bool selectOpenFileInExplorer(WCHAR* FileName, LPCWSTR FilterSpec, LPCWSTR DefExt, LPCWSTR Title = L"Open....", LPCWSTR InitialPath = L".");
	bool selectSaveFileInExplorer(WCHAR* FileName, LPCWSTR FilterSpec, LPCWSTR DefExt, LPCWSTR Title = L"Save as....", LPCWSTR InitialPath = L".");
}

namespace stringUtils {
	//split str by sep
	std::vector<std::string> split(std::string str, char sep);

	//{a0,a1,a2,...}->'a0,a1,a2,...'
	std::string array_to_string(size_t* array, size_t size, size_t linefeed = 0);
	std::string array_to_string(double* array, size_t size, size_t linefeed = 0);

	//get the size of array (string_to_array(array).size())
	size_t count(std::string str, char chr);

	//string "1, 2, 3.0, 3.1"->double{1,2,3.0,3.1}
	void string_to_array(std::string str, double* array, size_t size);

	//string "1, 2, 3, 3"->size_t {1,2,3,3}
	void string_to_array(std::string str, size_t* array, size_t size);

	//get time as string ('yyyymmdd_hhmmss')
	std::string getTime();
}