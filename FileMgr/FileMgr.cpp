/////////////////////////////////////////////////////////////////////
//  FileMgr.cpp - find files matching specified patterns           //
//               on a specified path                               //
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
* This package provides test module for FileMgr.h, which will 
* process for searching specified directory recursively find 
* all the file in this directory.
*
* Build Process:
* --------------
* Required Files
* ScopeStack.h, MetricAnalysis.h
* Build commands
* - devenv Project2HelperCode.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2016
* - first release by Jim
*/
#include "FileMgr.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
  DataStore ds;
  FileMgr fm(".", ds); // path and datastore
  //fm.addPattern("*.h");
  //fm.addPattern("*.partial");
  fm.search();
  for (auto fs : ds)
  {
    std::cout << "\n  " << fs;
  }
  std::cout << "\n\n";
  return 0;
}
#endif
