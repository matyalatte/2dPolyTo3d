/*
 * File: exception.h
 * --------------------
 * exception class for graph
 * 
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include <stdio.h>
#include <string>

namespace graph{
	class graphException : public std::exception {
	private:
		const char* type;
		const char* msg;
		bool unexpected;

	public:
		graphException(const char* type, const char* msg, bool unexpected=true);

		//get parameters
		const char* getType();
		bool getUnexpected();
		const char* what();
		
		//get formatted error message
		std::string getErrorMsg();

		//print error message
		void print();
	};
}