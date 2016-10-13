/////////////////////////////////////////////////////////////////////
// Utilities.cpp - small, generally usefule, helper classes        //
// ver 1.1                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Webnin Li, Syracuse University,                    //
//              wli102@syr.edu                                     //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* generate usefull funcion for other packages
* such as print the title with specified underline 
*
* Public Interface:
* =================
* title(const std::string& src)
* Title(const std::string& src, char underline)
* putline()
*
* Build Process:
* ==============
* Build commands 
* - devenv Project2HelperCode.sln /rebuild debug
* 
* Maintenance History:
* --------------------
*
* ver 1.0 : 11 Jun 2014
* - first release by Jim
*/

#include <cctype>
#include <iostream>
#include "Utilities.h"

using namespace Utilities;
/* print the title with = */
std::function<void(const std::string&)> Title =
  [](auto src) { StringHelper::Title(src, '='); };
/* print the title with - */
std::function<void(const std::string&)> title =
  [](auto src) { StringHelper::Title(src, '-'); };
/* print the title with - */
void StringHelper::title(const std::string& src)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, '-');
}
/* print the title with specified underline */
void StringHelper::Title(const std::string& src, char underline)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, underline);
}
/* split the statement */
std::vector<std::string> StringHelper::split(const std::string& src)
{
  std::vector<std::string> accum;
  std::string temp;
  size_t index = 0;
  do
  {
    while ((isspace(src[index]) || src[index] == ',') && src[index] != '\n')
    {
      ++index;
      if (temp.size() > 0)
      {
        accum.push_back(temp);
        temp.clear();
      }
    }
    if(src[index] != '\0')
      temp += src[index];
  } while (index++ < src.size());
  if(temp.size() > 0)
    accum.push_back(temp);
  return accum;
}

/* print the end line */
void Utilities::putline()
{
  std::cout << "\n";
}

#ifdef TEST_UTILITIES

int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test StringHelper::split(std::string)");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;
  
  std::vector<std::string> result = StringHelper::split(test);
  
  std::cout << "\n";
  for (auto item : result)
  {
    if (item == "\n")
      std::cout << "\n  " << "newline";
    else
      std::cout << "\n  " << item;
  }
  std::cout << "\n";

  title("test std::string Converter<T>::toString(T)");

  std::string conv1 = Converter<double>::toString(3.1415927);
  std::string conv2 = Converter<int>::toString(73);
  std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

  std::cout << "\n  Converting from values to strings: ";
  std::cout << conv1 << ", " << conv2 << ", " << conv3;
  putline();

  title("test T Converter<T>::toValue(std::string)");

  std::cout << "\n  Converting from strings to values: ";
  std::cout << Converter<double>::toValue(conv1) << ", ";
  std::cout << Converter<int>::toValue(conv2) << ", ";
  std::cout << Converter<std::string>::toValue(conv3);

  std::cout << "\n\n";
  return 0;
}
#endif
