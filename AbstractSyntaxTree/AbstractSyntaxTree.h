/////////////////////////////////////////////////////////////////////
//  AbstractSyntaxTree.h - Store Trees for every single file     //
//  ver 1.0                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2015                                 //
//  Platform:      Macbook, Win10                                  //
//  Application:   Prototype for CSE687 Pr2                        //
//  Author:        Wenbin LI, Syracuse University                  //
//                 wli102@syr.edu                                  //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
AbstractSyntaxTree.h is a template class build for 
store trees of every single file to be analysis. 
Their trees are sotre here for the further analysis 
or modify, even if the reposity scope goes out. 
Moreover the if the function's cc is morethan 10, 
it will be stored in this tree class.

Public Interface:
=================
void pushToTree(element* elem);         // store the tree
void pushToNoticedNode(element* elem);  // notice not proper cc node
void pushToFilefunction(element* elem); // push every function node

Build Process:
==============
Required files
- AbstractSyntaxTree.h
Build commands 
- devenv Project2HelperCode.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 10 Mar 06
- first release by LI

*/
#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
/* class for store every single tree of every file */
template <typename element>
class AST {
public:
	std::vector<element*> getTree();
	std::vector<element*> getNoticedNode();
	std::vector<element*> getFilefunction();
	void pushToTree(element* elem);
	void pushToNoticedNode(element* elem);
	void pushToFilefunction(element* elem);
private:
	std::vector<element*> aSTree;
	std::vector<element*> noticedNode;
	std::vector<element*> filefunction;
};

/* get the tree data */
template<typename element>
std::vector<element*> AST<element>::getTree() {
	return aSTree;
}

/* get the noticed tree node */
template<typename element>
inline std::vector<element*> AST<element>::getNoticedNode()
{
	return noticedNode;
}

template<typename element>
inline std::vector<element*> AST<element>::getFilefunction()
{
	return filefunction;
}

/* push element to tree data node */
template<typename element>
inline void AST<element>::pushToTree(element* elem) {
	aSTree.push_back(elem);
}

/* push noticed node to tree data -- noticedNode */
template<typename element>
inline void AST<element>::pushToNoticedNode(element* elem)
{
	noticedNode.push_back(elem);
}

template<typename element>
inline void AST<element>::pushToFilefunction(element * elem)
{
	filefunction.push_back(elem);
}


#endif
