#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.1                                                        //
//                                                                 //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      Macbook, Win10                                  //
//  Author:        Wenbin Li,      Syracuse University             //
//                 wli102@syr.edu                                  //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands 
    - devenv Project2HelperCode.sln /rebuild debug


  Maintenance History:
  ====================
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole(AST<element> abstractSyntaxtTree) {};
  ConfigParseToConsole() {}
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build(AST<element>& abstractSyntaxtTree);
  //Parser* Build();

private:
  // Builder must hold onto all the pieces

  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  SpecialKeyWord* pSpecialKeyWord;
  PushSpecialKeyWord* pPushSpecialKeyWord;
  KeyWordWithoutParentheses* pKeyWordWithoutParentheses;
  PushKeyWordWithoutParentheses* pPushKeyWordWithoutParentheses;
  NameSpaceAndClass* pNameSpaceAndClass;
  PushNameSpaceAndClass* pPushNameSpaceAndClass;

  //Declaration* pDeclaration;
  //ShowDeclaration* pShowDeclaration;
  //Executable* pExecutable;
  //ShowExecutable* pShowExecutable;

  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};


#endif
