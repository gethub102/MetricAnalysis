 Test files and debug.exe are in x64 directory.


 Requriement 1, 2: Visual Studio 2015 and STL is used in this project.

 Requirement 3: Tokenizer and SemiExp package from project 1 are used for this project.

 Requirement 4: Parser class has using relationship with IRule.
      IRule class has using relationship with IAction.
      Rules are added into Parser to test scope and function.
      Actions are added into Rules to perform actions.

 Requirement 5: ActionAndRule package has defined rules class for detecting:
      the beginning and end scope
      the Funcation defination
      the Special key word, like if(), else, try, while(), and so on
      the class and namespace
      different actions have been added to different rules. AST is built by these actions.
      the tree will demonstrate scope structure and every node's type and lines.
      This AST structure will be dispalyed in following below

 Requirement 6: every sinle Abstract Syntax Tree is stored in AST class(AbstractSyntaxTree.h)
      This tree's node is built and modified in ActionAndRule package with the public member function doAction()
      MetricsAnalysis::analysisComplexity(element* elem, int& countComplexity) analysis function complexity.
      AST::void pushToTree(element* elem) push tree from ActionAndRule to AST class.

 Requirement 7: FileMgr package provides functions for searching files and directories with specified patterns.
 Usage:  -f fullpath //searching specify file
         -d directory //searching the files in this direcotry
         -p pattern //add the files pattern of searching

 Requirement 8: MetricsAnalysis package provides the interface for
      MetricsAnalysis::analysisComplexity(element* elem, int& countComplexity) analysising complexity of function node.
      MertricAnalysis::displayStructureAndSummaryFuction(AST<element> abstractSyntaxtTree)  display function in specified package.
      The display is following below

 Requirement 9: MetricsExecutive package is build for the entrance of whole project.
      The commandline in run.bat is:
      MetricsExecutive.exe -p *.cpp -p *.h -d ../TEST -f ../SpecifyTest/SpecifyTest.cpp

 Requirement 10: Every package has its own test stub, and MetricsExecutive combines them together to fulfil Parser.