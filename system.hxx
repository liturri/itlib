#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "string"

class Popen
{
public:
   Popen(const std::string &cmd, const std::string &mode = "r");
   ~Popen();
   std::string readline();
   void write(const std::string &buff);

protected:
   FILE *cmdFD;
};

std::string getPathName(const std::string &s);
std::string getFileName(const std::string &s);
#endif
