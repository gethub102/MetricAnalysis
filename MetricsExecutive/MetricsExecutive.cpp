/////////////////////////////////////////////////////////////////////
//  MetricsExecutive.cpp - Analyzes C++ language                   //
//  ver 1.0                                                        //
//  Language:      C++, Visual Studio 2015                         //
//  Platform:      Macbook win10                                   //
//  Application:   Parser component, OOD Project#2                 //
//  Author:        Webnin Li, Syracuse University,                 //
//                 wli102@syr.edu                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is the entrance for this whole solutioin.
* It can analysis single or multiple files with specified
* path, or file name. The class, namespace, function, if, 
* while, try, catch, else and operator==, operator=, operator[],
* and so on will be analysised. The analysis information includes
* complextiy of scope and funciton lines, and their structure.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
* ActionsAndRules.h, ConfigureParser.h, 
* FileSystem.h, FileMgr.h
* Build commands
* - devenv Project2HelperCode.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.5 : 10 Mar 2016
* - fixed the main() lines less than 50
* ver 1.4 : 10 Mar 2016
* - demonstrate requirements(what I do)
* ver 1.3 : 10 Mar 2016
* - adpat to add multiple patterns for searching
* ver 1.2 : 10 Mar 2016
* - modified the Parser constructor parser() to parser(AbstractSyntaxTree)
* ver 1.1 : 10 Mar 2016
* - fixed fileMgr bug.
* ver 1.0 : 10 Mar 2014
* - first release by Li
*/

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace Scanner;
using namespace Utilities;

/* ---get the path of file--- */
DataStore getPath(std::string path, DataStore& ds, std::vector<std::string> pattern) {
	FileMgr fm(path, ds); // path and datastore
	auto iter = pattern.begin();
	while (iter != pattern.end()) {
		fm.addPattern(*iter);
		iter++;
	}
	//fm.addPattern("*.partial");
	fm.search();
	return ds;
}
/*-- search the pattern command-- */
void searchPatternCommand(std::vector<std::string>& pattern, int argc, char** argv) {
	std::string patterncommand = "-p";
	pattern.push_back("*.*");
	bool first = true;
	for (int j = 1; j < (argc - 1); j++)
	{
		try
		{
			if (argv[j] == patterncommand) {
				if (first == true)
				{
					first = false;
					pattern.pop_back();
				}
				pattern.push_back(argv[j + 1]);
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "search pattern: " << e.what() << std::endl;
		}
	}
}
/* demonstrate the satisfaction for requirements of this project */
void testRequirement() {
	std::cout << "\n Requriement 1, 2: Visual Studio 2015 and STL is used in this project.\n";
	std::cout << "\n Requirement 3: Tokenizer and SemiExp package from project 1 are used for this project.\n";
	std::cout << "\n Requirement 4: Parser class has using relationship with IRule.";
	std::cout << "\n      IRule class has using relationship with IAction.";
	std::cout << "\n      Rules are added into Parser to test scope and function.";
	std::cout << "\n      Actions are added into Rules to perform actions.\n";
	std::cout << "\n Requirement 5: ActionAndRule package has defined rules class for detecting: ";
	std::cout << "\n      the beginning and end scope";
	std::cout << "\n      the Funcation defination";
	std::cout << "\n      the Special key word, like if(), else, try, while(), and so on";
	std::cout << "\n      the class and namespace";
	std::cout << "\n      different actions have been added to different rules. AST is built by these actions.";
	std::cout << "\n      the tree will demonstrate scope structure and every node's type and lines.";
	std::cout << "\n      This AST structure will be dispalyed in following below \n";
	std::cout << "\n Requirement 6: every sinle Abstract Syntax Tree is stored in AST class(AbstractSyntaxTree.h) ";
	std::cout << "\n      This tree's node is built and modified in ActionAndRule package with the public member function doAction()";
	std::cout << "\n      MetricsAnalysis::analysisComplexity(element* elem, int& countComplexity) analysis function complexity.";
	std::cout << "\n      AST::void pushToTree(element* elem) push tree from ActionAndRule to AST class. \n";
	std::cout << "\n Requirement 7: FileMgr package provides functions for searching files and directories with specified patterns. ";
	std::cout << "\n Usage:  -f fullpath //searching specify file";
	std::cout << "\n         -d directory //searching the files in this direcotry";
	std::cout << "\n         -p pattern //add the files pattern of searching \n";
	std::cout << "\n Requirement 8: MetricsAnalysis package provides the interface for";
	std::cout << "\n      MetricsAnalysis::analysisComplexity(element* elem, int& countComplexity) analysising complexity of function node.";
	std::cout << "\n      MertricAnalysis::displayStructureAndSummaryFuction(AST<element> abstractSyntaxtTree)  display function in specified package.";
	std::cout << "\n      The display is following below \n";
	std::cout << "\n Requirement 9: MetricsExecutive package is build for the entrance of whole project. ";
	std::cout << "\n      The commandline in run.bat is: ";
	std::cout << "\n      MetricsExecutive.exe -p *.cpp -p *.h -d ../TEST -f ../SpecifyTest/SpecifyTest.cpp\n";
	std::cout << "\n Requirement 10: Every package has its own test stub, and MetricsExecutive combines them together to fulfil Parser.\n\n";
}
/* print the whole test title */
void printWholeTestTitle() {
	Util::Title("Testing Parser Class", '=');
	Utilities::putline();
}
/* judge if the argument number not less than 2 */
int getNumberOfArguments(int argc) {
	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	else
		return 0;
}

/* handle multiple path and specified files, and add multiple patterns */
int handlePathAndPattern(int& i, int argc, char* argv[], 
	DataStore& tmpPathDstore, std::vector<std::string>& pattern, 
	std::string commandD, std::string commandF, std::string commandP) {

	if (argv[i] == commandD)
	{
		i++;
		searchPatternCommand(pattern, argc, argv);
		getPath(argv[i], tmpPathDstore, pattern);
		return 0;
	}

	if (argv[i] == commandF)
	{
		i++;
		tmpPathDstore.save(argv[i]);
		return 0;
	}

	if (argv[i] == commandP)
	{
		i++;
		return 1;
	}
}

#ifdef TEST_METRICSEXECUTIVE

int main(int argc, char* argv[]) {
	printWholeTestTitle();
	if (getNumberOfArguments(argc) == 1)
		return 1;
	AST<element> abstractSyntaxtTree; //AST 
	std::string commandD = "-d", commandF = "-f", commandP = "-p";
	for (int i = 1; i < argc; ++i) {
		DataStore tmpPathDstore;
		std::vector<std::string> pattern;
		if (handlePathAndPattern(i, argc, argv, tmpPathDstore, pattern, commandD, commandF, commandP) == 1)
			continue;
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		std::string msg = "Processing file " + fileSpec;
		Util::title(msg);
		Utilities::putline();
		for (auto fs : tmpPathDstore) {
			ConfigParseToConsole configure;
			Parser* pParser = configure.Build(abstractSyntaxtTree);
			element* functionEle = new element;
			abstractSyntaxtTree.pushToFilefunction(functionEle); //add a node for hold the function of the single file
			functionEle->filepathandname = fs;
			functionEle->filename = FileSystem::Path::getName(fs);
			try {
				if (pParser) {
					if (!configure.Attach(fs)) {
						std::cout << "\n  could not open file " << fileSpec << std::endl;
						continue;
					}
				}
				else {
					std::cout << "\n\n  Parser not built\n\n";
					return 1;
				}
				while (pParser->next())
					pParser->parse(abstractSyntaxtTree);
				abstractSyntaxtTree.getTree().back()->filepathandname = fs; //get ful path and name
				abstractSyntaxtTree.getTree().back()->filename = FileSystem::Path::getName(fs);
				std::cout << "\n";
			}
			catch (std::exception& ex) {
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
		}
	}
	testRequirement();
	MertricAnalysis displayAnalysisTree;	
	displayAnalysisTree.displayStructureAndSummaryFuction(abstractSyntaxtTree);
	std::cout << "\n";
	return 0;
}



#endif