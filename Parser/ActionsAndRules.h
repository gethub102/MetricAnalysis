#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Macbook, Win10                                  //
//  Application:   Prototype for CSE687 Pr2                        //
//  Author:        Wenbin LI, Syracuse University                  //
//                 wli102@syr.edu                                  //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module defines several action classes.  Its classes provide
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable.

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
	parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
	- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
	  ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
	  ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands 
	- devenv Project2HelperCode.sln /rebuild debug

  Maintenance History:
  ====================
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
	of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../MetricAnalysis/MetricAnalysis.h"


///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
/* class for store the element */
class Repository  // application specific
{
	ScopeStack<element*> stack;
	Scanner::Toker* p_Toker;
	MertricAnalysis* p_metricAnalysis;
public:
	//get p_metricAnalysis pointer
	MertricAnalysis* metricAnalysis() { 
		return p_metricAnalysis;
	}
	//set toker pointer
	Repository(Scanner::Toker* pToker)  
	{
		p_Toker = pToker;
	}
	//get stack
	ScopeStack<element*>& scopeStack() 
	{
		return stack;
	}
	//get toker pointer
	Scanner::Toker* Toker() 
	{
		return p_Toker;
	}
	//get line number
	size_t lineCount() 
	{
		return (size_t)(p_Toker->currentLineCount());
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	//BeginningOfScope rule test
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		if (pTc->find("{") < pTc->length())
		{
			//std::cout << "  BeginningOfScope match!\n";
			doActions(pTc, abstractSyntaxtTree);
			return true;
		}
		return true; // if did not find {, stop check this token.
	}
	std::string ruleName() {
		return "BeginningOfScope";
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
	bool first = true;
public:
	//constructor
	HandlePush(Repository* pRepos) 
	{
		p_Repos = pRepos;
	}
	//BeginningOfScope do action
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree)  
	{
		if (first == true) {
			first = false;
			element* nameSpace = new element;
			nameSpace->type = "Global-NameSpace";
			nameSpace->name = "Root";
			nameSpace->startLineCount = p_Repos->lineCount();
			p_Repos->scopeStack().push(nameSpace);
			abstractSyntaxtTree.pushToTree(nameSpace);

		}
		//std::cout << "\n--BeginningOfScope rule";
		element *elem = new element;
		elem->type = "unknown";
		elem->name = "anonymous";
		elem->startLineCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	//EndOfScope rule test
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		if (pTc->find("}") < pTc->length())
		{
			//std::cout << "  EndOfScope match!\n";
			doActions(pTc, abstractSyntaxtTree);
			return false;
		}
		return true;
	}
	// return the rule's name
	std::string ruleName() { 
		return "EndOfScope";
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	//constructor
	HandlePop(Repository* pRepos) 
	{
		p_Repos = pRepos;
	}
	//EndOfScope action
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		//std::cout << "\n--EndOfScope rule";
		if (p_Repos->scopeStack().size() == 0)
			return;
		//p_Repos->scopeStack().top().endLineCount = p_Repos->lineCount();
		
		p_Repos->scopeStack().top()->endLineCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->lineCount = p_Repos->scopeStack().top()->endLineCount - p_Repos->scopeStack().top()->startLineCount + 1;
		p_Repos->scopeStack().top()->complexity = p_Repos->metricAnalysis()->analysisComplexity(p_Repos->scopeStack().top(), p_Repos->scopeStack().top()->complexity); //update the complexity
		element* elem = p_Repos->scopeStack().pop(); 
		
		if (elem->type == "function")
		{
			if (elem->complexity > 10)
				abstractSyntaxtTree.pushToNoticedNode(elem);
			
			abstractSyntaxtTree.getFilefunction().back()->addChild(elem); //add function to top AST Filefunction node 
		}
	}
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	/* judge if this is a special key word */
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	//FunctionDefinition rule test
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc, abstractSyntaxtTree);
				return false;
			}
		}
		return true;
	}
	std::string ruleName() {
		return "FunctionDefinition";
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	/* constructor of PushFunction */
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	/* FunctionDefinition action */
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) //PushFunction
	{
		std::string name = (*pTc)[pTc->find("(") - 1];
		if (name == "[]" || name == "=" || name == "+" || name == "+=" || name == "==")
		{
			if ((*pTc)[pTc->find("(") - 2] == "=")
				name = "lambda";
			else 
				name = (*pTc)[pTc->find("(") - 2] + name;
		}
		if (name == "]")
			name = "lambda";
		p_Repos->scopeStack().top()->name = name;
		p_Repos->scopeStack().top()->type = "function";
		p_Repos->scopeStack().top()->startLineCount = p_Repos->lineCount();
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
public:
	// constructor
	PrintFunction(Repository* pRepos) 
	{
		p_Repos = pRepos;
	}
	/* FunctionDefinition action */
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree)
	{
		std::cout << "\n  FuncDef: " << pTc->show();// .c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		size_t len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (size_t i = 0; i < len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect special key word

class SpecialKeyWord : public IRule{
public:
	/* test if this is a special key word */
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	/* SpecialKeyWord rule test */
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "  SpecialKeyWord match!\n";
				doActions(pTc, abstractSyntaxtTree);
				return false;
			}
		}
		return true;
	}
};

class PushSpecialKeyWord : public IAction {
	Repository* p_Repos;
public:
	//constructor
	PushSpecialKeyWord(Repository* pRepos) { 
		p_Repos = pRepos;
	}
	/* SpecialKeyWord action */
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) {
		ITokCollection& tc = *pTc;
		size_t len = tc.find("(");
		p_Repos->scopeStack().top()->name = "  ";
		p_Repos->scopeStack().top()->type = tc[len - 1];
		p_Repos->scopeStack().top()->startLineCount = p_Repos->lineCount();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect special key word

class KeyWordWithoutParentheses : public IRule {
public:
	/* test if this is a Special Key Word Without Parentheses */
	bool isSpecialKeyWordWithoutParentheses(const std::string& tok)
	{
		const static std::string keys[]
			= { "do", "try", "else" };
		for (int i = 0; i < 3; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	/* KeyWordWithoutParentheses rule test  */ 
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.length() - 1;
			while (tc[len - 1] == "\n")
				len--;
			if (isSpecialKeyWordWithoutParentheses(tc[len - 1]))
			{
				//std::cout << "  KeyWordWithoutParentheses match!\n";
				doActions(pTc, abstractSyntaxtTree);
				return false;
			}
		}
		return true;
	}
};

class PushKeyWordWithoutParentheses : public IAction {
	Repository* p_Repos;
public:
	//constrcutor
	PushKeyWordWithoutParentheses(Repository* pRepos) { 
		p_Repos = pRepos;
	}
	/* KeyWordWithoutParentheses action */
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) {
		ITokCollection& tc = *pTc;
		size_t lenTry = tc.find("try");
		size_t lenDo = tc.find("do");
		size_t lenElse = tc.find("else");
		size_t len = 0;
		if (lenElse < tc.length())
			len = lenElse;
		else if (lenTry < tc.length())
			len = lenTry;
		else
			len = lenDo;
		p_Repos->scopeStack().top()->name = "  ";
		p_Repos->scopeStack().top()->type = tc[len];
		p_Repos->scopeStack().top()->startLineCount = p_Repos->lineCount();
	}
};


class NameSpaceAndClass : public IRule {
public:
	/* judge if the name is class or namespace */
	bool isNameSpaceOrClass(const std::string& tok)
	{
		const static std::string keys[]
			= { "namespace", "class" };
		for (int i = 0; i < 2; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	/* NameSpaceAndClass rule test */ 
	bool doTest(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) 
	{
		ITokCollection& tc = *pTc;

		if (tc[tc.length() - 1] == "{" && (tc.find("class") < tc.length() || tc.find("namespace") < tc.length()))
		{
			doActions(pTc, abstractSyntaxtTree);
			return false;
		}
		return true;
	}
};

class PushNameSpaceAndClass : public IAction {
	Repository* p_Repos;
public:
	PushNameSpaceAndClass(Repository* pRepos) { // constructor
		p_Repos = pRepos;
	}
	/* NameSpaceAndClass action function */
	void doAction(ITokCollection*& pTc, AST<element>& abstractSyntaxtTree) {
		ITokCollection& tc = *pTc;
		size_t lenClass = tc.find("class");
		size_t lenNameSpace = tc.find("namespace");
		size_t len = 0;
		if (lenClass < tc.length())
			len = lenClass;
		else
			len = lenNameSpace;
		p_Repos->scopeStack().top()->type = ((lenClass < tc.length())? "class" : "namespace");
		p_Repos->scopeStack().top()->name = tc[len + 1];
		p_Repos->scopeStack().top()->startLineCount = p_Repos->lineCount();
	}
};

#endif
