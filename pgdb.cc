#include "pgdb.hxx"
#include <iostream>

namespace pgdb
{
   void connDelete(PGconn *ptr)
   {
      // std::cerr << "Connection destroy:" << ptr << "\n" ;
      PQfinish(ptr);
   }

   db::db()
   {
      // std::cerr << "Empty connection init\n" ;
   };

   db::db(const std::string &connString)
      : connString(connString)
      , conn(std::shared_ptr<PGconn>(PQconnectdb(connString.c_str()), connDelete))
   {
      // std::cerr << "Connection init:" << this->conn.get() << "\n" ;
      if(PQstatus(this->conn.get()) != CONNECTION_OK) {
         // throw std::exception (std::string("Connection to database failed: ") + PQerrorMessage(this->conn));
         throw std::string("Connection to database failed: ") + PQerrorMessage(this->conn.get());
      }
   }

   db::db(const db &dbConn)
      : conn(dbConn.conn)
   {
   }

   std::string db::getError()
   {
      return PQerrorMessage(conn.get());
   }

   std::string db::escapeString(const std::string str)
   {
      std::string strBuff;
      char *buff;
      buff=PQescapeLiteral(conn.get(), str.c_str(), 100000);
      strBuff += buff;
      PQfreemem(buff);
      return strBuff;
   }

   trans db::Begin()
   {
      return trans(*this);
   }

   trans::trans(const db &conn)
      : conn(conn), transOpen(false)
   {
      // std::cerr << "Init trans: " << conn.conn.get() << "\n";
      auto res= query("BEGIN");
      transOpen=true;
   }

   void trans::rollback()
   {
      query("ROLLBACK");
      transOpen=false;
   };

   void trans::commit()
   {
      query("commit");
      transOpen=false;
   };

   result trans::query(const std::string &q)
   {
      // std::cerr << "Q: " << q << "\n";
      auto res= result(PQexec(this->conn.getConn(), q.c_str()));
      auto status= res.status();
      if(status == PGRES_BAD_RESPONSE || status == PGRES_FATAL_ERROR) {
         std::string err= "Query command failed: (" + std::string(PQresStatus(status)) + ") " + conn.getError();
         throw err;
      }
      return res;
   }


   void trans::endCopy()
   {
      // std::cerr << "endCopy\n";

      if (PQputCopyEnd(conn.getConn(), nullptr) != 1)
      {
         std::string err= "Copy failed: " + conn.getError();
         throw err;
      }
   }


   void trans::putCopyData(const std::string &buffer)
   {
      if (PQputCopyData(conn.getConn(), buffer.c_str(), buffer.length()) != 1)
      {
         std::string err= "Copy failed: " + conn.getError();
         throw err;
      }
   }


   trans::~trans()
   {
      // std::cerr << "Done trans: " << conn.conn.get() << "\n";
      if (transOpen)
         rollback();
   }

   void resultDelete(PGresult *res)
   {
      PQclear(res);
   }

   result::result(PGresult *r)
      : res(std::shared_ptr<PGresult>(r, resultDelete))
   {
      // std::cerr << "Init result: " << res << "\n";
      // this->res = res;
   };

   result::~result()
   {
      // std::cerr << "Done result: " << res << "\n";
   }

   int result::getRowCount() const
   {
      return PQntuples(res.get());
   }

   int result::getColCount() const
   {
      return PQnfields(res.get());
   }

   const std::string result::getValue(int row, int col) const
   {
      if(row < 0 || row > getRowCount())
         throw std::string("Row number:") + std::to_string(row) + " out of range 0 - " + std::to_string(getRowCount());
      if(col < 0 || col > getColCount())
         throw std::string("Col number:") + std::to_string(col) + " out of range 0 - " + std::to_string(getColCount());
      return PQgetvalue(res.get(), row, col);
   }

   const std::vector<std::string> result::getRow(int row) const
   {
      int nFields= getColCount();
      std::vector<std::string> ret;
      ret.reserve(nFields);
      for(int j= 0; j < nFields; j++)
         ret.push_back(PQgetvalue(res.get(), row, j));
      return ret;
   }
}
