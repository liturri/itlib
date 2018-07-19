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
      tr.commit();
   } catch (std::string &error) {
      std::cerr << error << "\n";
   };
   try {
      auto tr=PGConn.Begin();            
      auto cp=pgdb::make_copy(tr, "test_copy", "fecha", "val");
      cp.addRow("2018-01-01", 23);
      cp.addRow("2018-01-02", 21.3);
      cp.addRow("2018-01-03", 25L);
      cp.end();
      tr.commit();
   } catch(std::string &error)  {
      std::cerr << "Error: " << error << std::endl;
      return 1;
   }
   return 0;
};
