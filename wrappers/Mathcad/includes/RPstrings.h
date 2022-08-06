
// This module implements the following string functions to make life easier
// Borrowed and modified from https://github.com/CoolProp/CoolProp/blob/master/include/CPstrings.h
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
//
#ifndef REFPROP_STRINGS_H
#define REFPROP_STRINGS_H

    #include <iterator>
    #include <algorithm>
    #include <functional>
    #include <cctype>

#if !defined(NO_CPPFORMAT)
    #ifndef FMT_HEADER_ONLY
    #define FMT_HEADER_ONLY
    #endif
    #include "fmt/format.h" // For addition of the string formatting functions and macros from cppformat
    #include "fmt/printf.h" // For sprintf
    #undef FMT_HEADER_ONLY
#else
    #include <vector>
    #include <string>
    #include <memory>
    #include <stdarg.h>
#endif

//    #include "Exceptions.h"

    /// Copy string to wstring
    /// Dangerous if the string has non-ASCII characters; from http://stackoverflow.com/a/8969776/1360263 
    inline void StringToWString(const std::string &s, std::wstring &ws)
    {
        ws = std::wstring(s.begin(), s.end());
    }

    /// The following code for the trim functions was taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    // trim from start
    inline std::string &strlstrip(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }
    // trim from end
    inline std::string &strrstrip(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }
    // trim from both ends
    inline std::string &strstrip(std::string &s) {
            return strlstrip(strrstrip(s));
    }
    /// Simple string function to check for end of string being equal to given string
    inline bool strendswith(const std::string &s1, const std::string &s2){
        // Impossible to match a string longer than the given string
        if (s2.size() > s1.size()){ return false; }
        long lhs = static_cast<long>(s1.rfind(s2));
        long rhs = static_cast<long>(s1.size()) - static_cast<long>(s2.size());
        return lhs == rhs;
    }


#if defined(NO_CPPFORMAT)
    // Missing string formatting function, this old guy is needed for ancient gcc compilers on PowerPC for VxWorks
    inline std::string format(const char* fmt, ...);
#else
    // Missing std::string formatting function - provided by the cppformat library
    inline std::string format(const char *format, fmt::ArgList args) {
      return fmt::sprintf(format, args);
    }
    FMT_VARIADIC(std::string, format, const char *)
#endif


    inline std::string upper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
    
    inline std::string lower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    // Missing string split and string join - like in Python
    std::vector<std::string> strsplit(const std::string &s, char del);
    std::string strjoin(const std::vector<std::string> &strings, const std::string &delim);

    /// A convenience function that return true if a string begins with the given other string
    inline bool strstartswith(const std::string &s, const std::string &other){ return s.find(other) == 0; };

    /**
     * @brief Convert a number encoded as a string to a double
     * @param s The string to be converted
     *
     * @note 
     */
    inline double string2double(const std::string &s){
        std::string mys = s; //copy
        // replace D with e (FORTRAN style scientific definition)
        if (mys.find("D") != std::string::npos){ std::size_t pos = mys.find("D"), len = 1; mys.replace(pos,len,"e"); }
        // replace d with e (FORTRAN style scientific definition)
        if (mys.find("d") != std::string::npos){ std::size_t pos = mys.find("d"), len = 1; mys.replace(pos,len,"e"); }
        
        const char * cs = mys.c_str();
        char* pEnd;
        double val = strtod(cs, &pEnd);
        if ((pEnd - &(cs[0])) != (int)s.size() ){
            // Found a character that is not able to be converted to number
            return 0;
        }
        else{
            return val;
        }
    }

#endif
