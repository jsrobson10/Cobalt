
#ifndef LEXER_H
#define LEXER_H

#include <string>

#include "StringReader.h"
#include "error/CompileError.h"

class Component;
class Function;
class Class;
class Variable;
class Import;
class FunctionCall;
class Code;
class Enum;
class Interface;

enum ComponentType
{
	CODE, FUNCTION, CLASS, VARIABLE, FUNCTION_CALL, IMPORT, ENUM, INTERFACE
};

class Lexer
{
	std::wstring code;
	
public:
	
	Lexer(std::wstring code);

	void process();
};

class Component
{
public:
	
	Component* next = NULL;
	ComponentType type;
};

class Code : public Component
{
public:
		
	Variable* assign;
	
};

class Function : public Component
{
	Code* code;
	Variable* arguments;
	std::wstring name;
	std::wstring type;
};

class Class : public Component
{
	Variable* variables;
	Function* methods;
	Class* parent;
	Interface* interfaces;
};

class Variable : public Component
{
public:
	
	bool isConst;
	std::wstring type;
	std::wstring name;
};

class FunctionCall : public Component
{
	
};

class Import : public Component
{
	
};

class Enum : public Component
{
	
};

class Interface : public Component
{
	Function* methods;
	Variable* veriables;
};

#endif
