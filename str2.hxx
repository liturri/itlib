#ifndef STR2_HXX
#define STR2_HXX

#include <string>
#include <vector>

std::vector<std::string> strSplit(const std::string &buff, const std::string &splitChars=" ");
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

// trim from start (copying)
std::string ltrimmed(std::string s);

// trim from end (copying)
std::string rtrimmed(std::string s);

// trim from both ends (copying)
std::string trimmed(std::string s);

// sprintf like
template< typename... Args >
std::string strprintf( const char* format, Args... args ) {
   int length = std::snprintf( nullptr, 0, format, args... );
   // assert( length >= 0 );

   char* buf = new char[length + 1];
   std::snprintf( buf, length + 1, format, args... );

   std::string str( buf );
   delete[] buf;
   return std::move(str);
}

#endif
