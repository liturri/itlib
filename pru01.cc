#include "pgdb.hxx"
#include <iostream>
#include <vector>

template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
   os << "[";
   for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
   {
      if (ii != v.end() -1 )
         os << '"' << *ii << "\", ";
      else
         os << '"' << *ii << '"';
   }
   os << "]";
   return os;
}


int main()
{
   pgdb::db PGConn;
   try {
      // PGConn=pgdb::db("dbname=sube_test user=liturri");
      PGConn=pgdb::db("dbname=liturri host=192.168.2.5 user=liturri");
   } catch (std::string &error) {
      std::cerr<<"Error: "<<error << std::endl;
      return 1;
   }
   try {
      auto tr=PGConn.Begin();
      tr.query(R"(create table test_copy (
    id serial,
    fecha date,
    val integer
))");
      auto cp=pgdb::make_copy(tr, "test_copy", "fecha", "val");
      cp.addRow("2018-01-01", "23");
      cp.addRow("2018-01-02", "21");
      cp.addRow("2018-01-02", "20");
      cp.end();
      tr.commit();
      //tr.query("drop table test_copy");
      // auto r=tr.query("select * from reporte order by reporte_id limit 100");
      // std::cerr << "Rows: " << r.getRowCount() << "\n";
      // // for (int rn=0 ; rn < r.getRowCount() ; rn++ )
      // //    std::cerr << r.getRow(rn) << "\n";
      // auto it=pgdb::rowIter(r);
      // for (auto rowI: it )
      // {
      //    for (auto colI: rowI)
      //       std::cerr << colI << " ";
      //    std::cerr << "\n";
      // }
   } catch(std::string &error)  {
      std::cerr<<"Error: "<<error << std::endl;
      return 1;
   }
   return 0;
};
