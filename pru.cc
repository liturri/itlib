#include "dates.hxx"
#include <stdio.h>
#include <locale.h>

int main()
{
   setlocale(LC_ALL, "");
   date Fecha2;
   date Fecha(Fecha2.GetFDate());
   Fecha2.SetDay(01);
   Fecha2.SetYear(99);
   std::cout << Fecha.GetSQLDateTime() << '\n' << Fecha2.GetSQLDateTime() << '\n';
   return 0;
};
