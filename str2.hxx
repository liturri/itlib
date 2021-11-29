#ifndef __STR2_HXX__
#define __STR2_HXX__

#include <sstream>
#include <string>
#include <vector>

// split string into vector<strings> by splitChars
std::vector<std::string> strSplit(const std::string &buff, const std::string &splitChars = " ");

// trim from start (in place)
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

// Return if string ends with second string
bool ends_with(std::string const &s, std::string const &ends);

// Return if string starts with second string
bool starts_with(std::string const &s, std::string const &starts);

// sprintf like
template <typename... Args>
std::string strprintf(const char *format, Args... args)
{
   int length = std::snprintf(nullptr, 0, format, args...);

   char *buf = new char[length + 1];
   std::snprintf(buf, length + 1, format, args...);

   std::string str(buf);
   delete[] buf;
   return str;
}

// Join Args with ','
template <typename Value, typename... Values>
std::string join_args(Value v, Values... vs)
{
   std::ostringstream oss;
   using expander = int[];
   oss << v; // first
   (void)expander{0, (oss << "," << vs, void(), 0)...};
   return oss.str();
}

template <typename InputIt>
std::string IterJoin(InputIt first, InputIt last, const std::string &separator = ",")
{
   std::ostringstream result;
   if (first != last)
   {
      result << *first;
      while (++first != last)
      {
         result << separator << *first;
      }
   }
   return result.str();
}
#endif // __STR2_HXX__
