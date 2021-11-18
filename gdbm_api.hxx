#ifndef GDBM_API_HXX
#define GDBM_API_HXX

#include <gdbm.h>
#include <string>

class Tgdbm_ro
{
public:
   Tgdbm_ro(const std::string &dbName);
   Tgdbm_ro();
   virtual ~Tgdbm_ro();

   virtual bool open(const std::string &dbName);
   bool exists(const std::string &key);

   const std::string operator[](const std::string &key);

protected:
   GDBM_FILE gdb_fd;
};

class Tgdbm_rw : public Tgdbm_ro
{
public:
   Tgdbm_rw(const std::string &dbName);
   Tgdbm_rw() : Tgdbm_ro(){};

   virtual bool open(const std::string &dbName);
   bool insert(const std::string &key, const std::string &value);
};

#endif
