// #define EXEC_GDBM_API
#include "gdbm_api.hxx"
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

using namespace std;

Tgdbm_ro::Tgdbm_ro()
{
   gdb_fd = nullptr;
}

Tgdbm_ro::Tgdbm_ro(const std::string &fName)
{
   open(fName);
}

Tgdbm_rw::Tgdbm_rw(const std::string &fName)
{
   open(fName);
}

Tgdbm_ro::~Tgdbm_ro()
{
   if (gdb_fd != NULL)
      gdbm_close(gdb_fd);
}

bool Tgdbm_ro::open(const std::string &fName)
{
   // cerr << "Open_RO" << endl;
   gdb_fd = gdbm_open(fName.c_str(), 512, GDBM_READER, 0770, NULL);
   if (!gdb_fd)
   {
      if (gdbm_errno == GDBM_FILE_OPEN_ERROR || gdbm_errno == GDBM_FILE_WRITE_ERROR || gdbm_errno == GDBM_FILE_SEEK_ERROR || gdbm_errno == GDBM_FILE_READ_ERROR || gdbm_errno == GDBM_FILE_STAT_ERROR)
         throw fName + "(ro): " + gdbm_strerror(gdbm_errno) + " " + strerror(errno);
      else
         throw fName + "(ro): " + gdbm_strerror(gdbm_errno);
   }
   size_t cacheSize = 500;
   int syncMode = false;

   gdbm_setopt(gdb_fd, GDBM_SETCACHESIZE, &cacheSize, sizeof(cacheSize));
   gdbm_setopt(gdb_fd, GDBM_SETSYNCMODE, &syncMode, sizeof(syncMode));
   return true;
}

bool Tgdbm_rw::open(const std::string &fName)
{
   // cerr << "Open_WR" << endl;
   gdb_fd = gdbm_open(fName.c_str(), 512, GDBM_WRCREAT, 0770, NULL);
   if (!gdb_fd)
   {
      if (gdbm_errno == GDBM_FILE_OPEN_ERROR || gdbm_errno == GDBM_FILE_WRITE_ERROR || gdbm_errno == GDBM_FILE_SEEK_ERROR || gdbm_errno == GDBM_FILE_READ_ERROR || gdbm_errno == GDBM_FILE_STAT_ERROR)
         throw fName + "(rw): " + gdbm_strerror(gdbm_errno) + " " + strerror(errno);
      else
         throw fName + "(rw): " + gdbm_strerror(gdbm_errno);
   }
   return true;
}

bool Tgdbm_rw::insert(const std::string &key, const std::string &value)
{
   datum k, v;
   k.dptr = const_cast<char *>(key.c_str());
   k.dsize = static_cast<int>(key.size());
   v.dptr = const_cast<char *>(value.c_str());
   v.dsize = static_cast<int>(value.size());

   return gdbm_store(gdb_fd, k, v, GDBM_INSERT) == 0;
}

const std::string Tgdbm_ro::operator[](const std::string &key)
{
   datum v, k;

   k.dptr = const_cast<char *>(key.c_str());
   k.dsize = static_cast<int>(key.size());
   v = gdbm_fetch(gdb_fd, k);
   if (v.dptr != NULL)
      return std::string(v.dptr);
   throw std::out_of_range("Key not found");
}

bool Tgdbm_ro::exists(const std::string &key)
{
   datum k;

   k.dptr = const_cast<char *>(key.c_str());
   k.dsize = static_cast<int>(key.size());
   return gdbm_exists(gdb_fd, k);
}

#ifdef EXEC_GDBM_API

int main(int argc, char **argv)
{
   Tgdbm_rw dbm;

   dbm.open("/tmp/test.db");

   std::cerr << dbm.exists("Hola1");
   // std::cerr << "Insert: " <<  dbm.insert("Hola", "Value1") << std::endl;

   // std::cerr << "Hola: " << dbm["Hola"] << std::endl;

   // dbm["Key1"] = "Key 2";
};

#endif
