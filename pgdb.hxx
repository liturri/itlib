#ifndef __pgdb_hxx__
#define __pgdb_hxx__

#include <libpq-fe.h>
#include <memory>
#include <string>
#include <vector>

namespace pgdb
{
class trans;
class result;

void connDelete(PGconn *);
void resultDelete(PGresult *res);

class db
{

public:
   db();
   db(const std::string &connString);
   db(const db &dbConn);
   trans Begin();

   // ~db() =default;
   std::string getError();

protected:
   PGconn *getConn()
   {
      return conn.get();
   };
   std::string connString;
   std::shared_ptr<PGconn> conn;

   friend class trans;
};

class trans
{
public:
   trans(const db &conn);
   ~trans();
   result query(const std::string &q);

   void rollback();
   void commit();

protected:
   db conn;
   // result res;
};

class result
{

public:
   result(PGresult *res);
   ~result();
   ExecStatusType status()
   {
      return PQresultStatus(res.get());
   };
   const int getRowCount() const;
   const int getColCount() const;
   const std::string getValue(int row, int col) const;
   const std::vector<std::string> getRow(int row) const;

protected:
   std::shared_ptr<PGresult> res;
};

class colIter
{
public:
   colIter(const result &r, const int row, const int col)
      : row(row)
      , col(col)
      , res(r) {};
   std::string operator*() const
   {
      return res.getValue(row, col);
   }

   colIter &operator++()
   {
      col++;
      return *this;
   }

   bool operator!=(colIter const &rhs) const
   {
      return (this->col < rhs.col);
   }

   colIter begin() const
   {
      return colIter(res, row, 0);
   }

   colIter end() const
   {
      return colIter(res, row, res.getColCount());
   }

protected:
   int row, col;
   result res;
};

class rowIter
{
public:
   rowIter(const result &r, const int row= 0)
      : row(row)
      , res(r) {};
   colIter operator*() const
   {
      return colIter(res, row, 0);
   }

   rowIter &operator++()
   {
      row++;
      return *this;
   }

   bool operator!=(rowIter const &rhs) const
   {
      return (this->row < rhs.row);
   }

   rowIter begin() const
   {
      return rowIter(res, 0);
   }

   rowIter end() const
   {
      return rowIter(res, res.getRowCount());
   }

protected:
   int row;
   result res;
};
}
#endif
