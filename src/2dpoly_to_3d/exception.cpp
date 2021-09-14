/*
 * File: exception.cpp
 * --------------------
 * exception class for graph
 * 
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */
#include "exception.hpp"

namespace graph {
	graphException::graphException(const char* type, const char* msg, bool unexpected) : 
		type(type), msg(msg), unexpected(unexpected) { }

	//get parameters
	const char* graphException::getType() { return type; }
	const char* graphException::what() { return msg; }
	bool graphException::getUnexpected() { return unexpected; }

	//get formatted error message
	std::string graphException::getErrorMsg()
	{
		std::string str = "ERROR: " + std::string(type) + "::" + std::string(msg);
		return str;
	}

	//print error message
	void graphException::print() {
		printf("%s\n", getErrorMsg().c_str());
	}
}