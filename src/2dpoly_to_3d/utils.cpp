/*
 * File: utils.cpp
 * --------------------
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#include "utils.hpp"

namespace fileUtils {

	//read a file as txt
	std::string read_txt(const char* file_path) {
		std::string file_str = "";
		std::ifstream fstream(file_path, std::ios::in);
		if (fstream.is_open()) {
			std::stringstream sstr;
			sstr << fstream.rdbuf();
			file_str = sstr.str();
			fstream.close();
		}
		else {
			printf("ERROR: fileUtils::read_txt : %s is not found.\n", file_path);
		}
		return file_str;
	}

	std::string read_txt(const WCHAR* file_path) {
		std::string file_str = "";
		std::ifstream fstream(file_path, std::ios::in);
		if (fstream.is_open()) {
			std::stringstream sstr;
			sstr << fstream.rdbuf();
			file_str = sstr.str();
			fstream.close();
		}
		else {
			wprintf(L"ERROR: fileUtils::read_txt : %s is not found.\n", file_path);
		}
		return file_str;
	}

	//write string to a file
	bool write_txt(const char* file_path, std::string str) {
		std::ofstream fstream(file_path);
		bool err = !fstream;
		if (err) {
			printf("ERROR: fileUtils::write_txt : can NOT access %s", file_path);
		}
		else {
			fstream << str << std::endl;
		}
		fstream.close();
		return !err;
	}

	bool write_txt(const WCHAR* file_path, std::string str) {
		std::ofstream fstream(file_path);
		bool err = !fstream;
		if (err) {
			wprintf(L"ERROR: fileUtils::write_txt : can NOT access %s", file_path);
		}
		else {
			fstream << str << std::endl;
		}
		fstream.close();
		return !err;
	}

	int mkdir(const char* dir) {
		//already exist = 1
		//success = 0
		//fail = -1
		struct stat statBuf;
		if (stat(dir, &statBuf) == 0) { return 1; }
		return _mkdir(dir);
	}

	//show error info about GetOpenFileName function
	void getDlgError() {
		bool ErrCode = CommDlgExtendedError();
		wchar_t szErrMsg[64];
		if (ErrCode) {
			switch (CommDlgExtendedError())
			{
			case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
			case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
			case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
			case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
			case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
			case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
			case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
			case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
			case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
			case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
			case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
			case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
			case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
			case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
			case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
			default: std::cout << "You cancelled.\n";
			}
			wsprintf(szErrMsg, L"エラーコード : %d", ErrCode);
			MessageBox(NULL, szErrMsg, L"GetOpenFileName", MB_OK);
		}
	}

	//setting for openfilename object
	void setOfn(OPENFILENAME *ofn, WCHAR* FileName,LPCWSTR FilterSpec, LPCWSTR Title, LPCWSTR InitialPath, LPCWSTR DefExt) {
		ofn->lStructSize = sizeof(OPENFILENAME);
		ofn->hwndOwner = GetFocus();
		ofn->lpstrFilter = FilterSpec; //extension filter
		ofn->lpstrCustomFilter = NULL;
		ofn->nMaxCustFilter = 0;
		ofn->nFilterIndex = 0;
		ofn->lpstrFile = FileName; //file path
		ofn->nMaxFile = MAX_PATH;
		ofn->lpstrInitialDir = InitialPath; // Initial directory.
		ofn->lpstrFileTitle = 0;
		ofn->nMaxFileTitle = MAX_PATH;
		ofn->lpstrTitle = Title; //window title
		ofn->lpstrDefExt = DefExt; //default extension
	}

	bool selectOpenFileInExplorer(WCHAR* FileName, LPCWSTR FilterSpec, LPCWSTR DefExt, LPCWSTR Title, LPCWSTR InitialPath) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		setOfn(&ofn, FileName, FilterSpec,
			Title, InitialPath, DefExt);

		ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

		if (!GetOpenFileName((LPOPENFILENAME)&ofn)){
			getDlgError();
			return false; // Failed or cancelled
		}
		else {return true;}
	}

	bool selectSaveFileInExplorer(WCHAR* FileName, LPCWSTR FilterSpec, LPCWSTR DefExt, LPCWSTR Title, LPCWSTR InitialPath) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		setOfn(&ofn, FileName, FilterSpec,
			Title, InitialPath, DefExt);

		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
		
		if (!GetSaveFileName((LPOPENFILENAME)&ofn)) {
			getDlgError();
			return false; // Failed or cancelled
		}
		else { return true; }
	}

}

namespace stringUtils{
	//split str by sep
	std::vector<std::string> split(std::string str, char sep) {
		std::vector<std::string> vec;
		std::string s;
		std::stringstream sstr{ str };
		while (std::getline(sstr, s, sep)) {
			vec.push_back(s);
		}
		return vec;
	}

	//{a0,a1,a2,...}->'a0,a1,a2,...'
	std::string array_to_string(size_t* array, size_t size, size_t linefeed) {
		std::string str = std::to_string((int)array[0]) + ",";
		for (size_t i = 1; i < size; i++) {
			if (linefeed > 0 && i % linefeed == 0) {
				str += "\n";//insert linefeed
			}
			str += std::to_string((int)array[i]) + ",";
		}
		str.pop_back();
		return str;
	}

	std::string array_to_string(double* array, size_t size, size_t linefeed) {
		std::string str = std::to_string(array[0]) + ",";
		for (size_t i = 1; i < size; i++) {
			if (linefeed > 0 && i % linefeed == 0) {
				str += "\n";//insert linefeed
			}
			str += std::to_string(array[i])+",";
		}
		str.pop_back();
		return str;
	}

	size_t count(std::string str, char chr) {
		return std::count(str.begin(), str.end(), chr);
	}

	//string "1, 2, 3.0, 3.1"->double{1,2,3.0,3.1}
	void string_to_array(std::string str, double* array, size_t size) {
		std::vector<std::string> vec;
		vec = split(str, ',');
		for (size_t i = 0; i < size; i++) {
			array[i] = std::stod(vec[i]);
		}
	}

	//string "1, 2, 3, 3"->size_t {1,2,3,3}
	void string_to_array(std::string str, size_t* array, size_t size) {
		std::vector<std::string> vec;
		vec = split(str, ',');
		for (size_t i = 0; i < size; i++) {
			array[i] = (size_t)(std::stoi(vec[i]));
		}
	}

	//get time as string ('yyyymmdd_hhmmss')
	std::string getTime() {
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &timeinfo);
		std::string str(buffer);
		return str;
	}

}