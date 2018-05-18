#ifndef __pgdb_hxx__
#define __pgdb_hxx__

#include <libpq-fe.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>


namespace pgdb
{
   class trans;
   class result;

   void connDelete(PGconn *conn);
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
      PGconn *getConn() {
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
      trans(trans &&) = default;
      // trans(const trans &&) = default;
      trans(const trans &) = delete;
      void operator=(trans const&) = delete;
      ~trans();
      result query(const std::string &q);
      void rollback();
      void commit();


   protected:
      db conn;
      bool transOpen;
      // result res;
      void putCopyData(const std::string &Buffer);
      void endCopy();

      template<typename... ARGS>
      friend class copy;

   };

   class result
   {

   public:
      result(PGresult *res);
      ~result();
      ExecStatusType status() {
         return PQresultStatus(res.get());
      };
      const int getRowCount() const;
      const int getColCount() const;
      const std::string getValue(int row, int col) const;
      const std::vector<std::string> getRow(int row) const;

   protected:
      std::shared_ptr<PGresult> res;
   };

   template<typename... ARGS>
   class copy
   {
   protected:
      trans *tr;
      std::string table;
      std::string columns;

   public:
      copy(trans &trans, const std::string &tbl, ARGS... args) : tr{&trans}, table{tbl} {
         addCol(args...);
         std::string cmd="copy ";
         cmd+=tbl;
         cmd+=" (";
         cmd+=columns;
         cmd+=") FROM STDIN;";
         // std::cerr << cmd << '\n';
         tr->query(cmd);
      };
      // ~copy() {
      //    tr.endCopy();
      // };
      std::string getColumns() const { return columns; }
      std::string getCopyBuffer() const { return copyBuffer; }
      void addRow(ARGS... args) {
         copyBuffer.clear();
         addRowTmpl(args...);
         tr->putCopyData(copyBuffer);
         std::cerr << copyBuffer;
      };

      void end() {
         //std::cerr << "\\.\n";
         tr->endCopy();
      };

   private:
      void addRowTmpl(const std::string &row) {
         copyBuffer.append(row);
         copyBuffer.append("\n");
      };
      template<typename... ROWS>
      void addRowTmpl(const std::string &row, ROWS... rows) {
         copyBuffer.append(row);
         copyBuffer.append("\t");
         addRowTmpl(rows...);
      }

      void addCol(const std::string &col) {
         columns.append(col);
      };
      template<typename... COLS>
      void addCol(const std::string &col, COLS... cols) {
         columns.append(col);
         columns.append(", ");
         addCol(cols...);
      }

      std::string copyBuffer;
   };


   template<typename... ARGS>
   copy<ARGS...>make_copy(trans &trans, const std::string &tbl, ARGS... args)
   {
      return copy<ARGS...>(trans, tbl, args...);
   }

   class colIter
   {
   public:
      colIter(const result &r, const int row, const int col)
         : row(row)
         , col(col)
         , res(r) {};
      std::string operator*() const {
         return res.getValue(row, col);
      }

      colIter &operator++() {
         col++;
         return *this;
      }

      bool operator!=(colIter const &rhs) const {
         return (this->col < rhs.col);
      }

      colIter begin() const {
         return colIter(res, row, 0);
      }

      colIter end() const {
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
      colIter operator*() const {
         return colIter(res, row, 0);
      }

      rowIter &operator++() {
         row++;
         return *this;
      }

      bool operator!=(rowIter const &rhs) const {
         return (this->row < rhs.row);
      }

      rowIter begin() const {
         return rowIter(res, 0);
      }

      rowIter end() const {
         return rowIter(res, res.getRowCount());
      }

   protected:
      int row;
      result res;
   };
}
#endif
