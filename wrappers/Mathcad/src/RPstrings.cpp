#define NO_CPPFORMAT
#include "RPstrings.h"
#include <cstdio>

// This module implements the following string functions to make life easier
//
// StringToWString(s,ws)                   : Takes a string, s, and converts it to a wide string, ws
// std:string strlstrip(s)                 : Trim string from start
// std:string strrstrip(s)                 : Trim string from end
// std:string strstrip(s)                  : Trim string from both ends
// bool strstartswith(s, other)            : True if s starts with other
// bool strendswith(s,other)               : True if s ends with other
// std:string upper(s)                     : returns Upper case version of s
// std:string lower(s)                     : returns Lower case version of s
// std:string format(strFormat,arglist)    : Uses sprintf to format an argument list according to provided format string
// std:string strjoin(strVec, delim)       : Concatenates a vector of strings together, separated by delim
// vector<string> strsplit(s, delim)       : splits s into a vector of strings using the specified delimiter, delim
// double string2double(s)                 : Converts string s to a double

std::string strjoin(const std::vector<std::string> &strings, const std::string &delim)
{
    // Empty input vector
    if (strings.empty()){return "";}

    std::string output = strings[0];
    for (unsigned int i = 1; i < strings.size(); i++)
    {
        output += format("%s%s",delim.c_str(),strings[i].c_str());
    }
    return output;
}

std::vector<std::string> strsplit(const std::string &s, char del)
{
    std::vector<std::string> v;
    std::string::const_iterator i1 = s.begin(), i2;
    while (true){
        i2 = std::find(i1, s.end(), del);
        v.push_back(std::string(i1, i2));
        if (i2 == s.end())
            break;
        i1 = i2+1;
    }
    return v;
}


#if defined(NO_CPPFORMAT)
std::string format(const char* fmt, ...)
{
    const int size = 512;
    struct deleter{ static void delarray(char* p) { delete[] p; } }; // to use delete[]
    std::shared_ptr<char> buffer(new char[size], deleter::delarray); // I'd prefer unique_ptr, but it's only available since c++11
    va_list vl;
    va_start(vl,fmt);
    int nsize = vsnprintf(buffer.get(),(size_t)size,fmt,vl);
    if(size<=nsize){                                         //fail delete buffer and try again
        buffer.reset(new char[++nsize], deleter::delarray);    //+1 for /0
        nsize = vsnprintf(buffer.get(),(size_t)nsize,fmt,vl);
    }
    va_end(vl);
    return buffer.get();
}
#endif
