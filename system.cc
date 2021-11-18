#include "system.hxx"

Popen::Popen(const std::string &cmd, const std::string &mode)
{
   cmdFD = popen(cmd.c_str(), mode.c_str());
}

std::string Popen::readline()
{
   char buff[1024];
   char *read = fgets(buff, sizeof(buff), cmdFD);

   if (read == NULL)
      return "";
   return buff;
}

void Popen::write(const std::string &buff)
{
   fputs(buff.c_str(), cmdFD);
}

Popen::~Popen()
{
   pclose(cmdFD);
}

std::string getPathName(const std::string &s)
{

   char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != std::string::npos)
   {
      return (s.substr(0, i));
   }

   return ("");
}

std::string getFileName(const std::string &s)
{

   char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != std::string::npos)
   {
      return (s.substr(i + 1));
   }

   return (s);
}
