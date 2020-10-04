// #define  EXEC_STR2

#include "str2.hxx"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iostream>

#include "benchmark.hxx"

using std::string;
using std::cout;
using std::endl;


std::vector<string> strSplit(const string &buff, const string &splitChars)
{
   std::vector<string> vec;
   std::size_t basePos=0;
   while (true)
   {
      auto splitPos=buff.find(splitChars, basePos);
      if (splitPos==basePos)
      {
         basePos+=splitChars.size();
         continue;
      }              
      if (splitPos == string::npos)
      {
         string auxStr=buff.substr(basePos);
         if (auxStr.size() > 0)
            vec.push_back(auxStr);
         return vec;
      };
      vec.push_back(buff.substr(basePos, splitPos-basePos));
      basePos=splitPos+splitChars.size();
   }
}


// trim from start (in place)
void ltrim(std::string &s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                   std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void rtrim(std::string &s)
{
   s.erase(std::find_if(s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s)
{
   ltrim(s);
   rtrim(s);
}

// trim from start (copying)
std::string ltrimmed(std::string s)
{
   ltrim(s);
   return s;
}

// trim from end (copying)
std::string rtrimmed(std::string s)
{
   rtrim(s);
   return s;
}

// trim from both ends (copying)
std::string trimmed(std::string s)
{
   trim(s);
   return s;
}

// Return if string ends with second string
bool ends_with(std::string const &s, std::string const &ends) {
   if ( s.length() >= ends.length()) {
      return (0 == s.compare (s.length() - ends.length(), ends.length(), ends));
   } else {
      return false;
   };
}

bool starts_with(std::string const &s, std::string const &starts) {
   if ( s.length() >= starts.length()) {
      return (0 == s.compare (0, starts.length(), starts));
   } else {
      return false;
   };
}

#ifdef EXEC_STR2
int main()
{
   string str="Lazy C++ is C++ without header files. Write code in one file and let Lzz generate your header and source files.";
   string split=" ";
   Benchmark t;
   for (int t=0; t < 5000000; t++)
      strSplit(str, split);
   cout << t.elapsed() << endl;
   //for (auto i = vec.begin(); i != vec.end(); ++i)
   //   cout << *i << '\n';
}
#endif

