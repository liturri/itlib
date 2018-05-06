#ifndef __DATES_H
#define __DATES_H

#include <time.h>
#include <string>
#include <iostream>

using std::string;

class date
{
public:
   date();
   date(string Date);
   string GetFDate();
   string GetSQLDate();
   string GetSQLDateTime();

   void   SetDay(int Value);
   void   SetYear(int Value);
   void   SetMonth(int Value);
   void   SetHour(int Value);
   void   SetMin(int Value);
   void   SetSec(int Value);

private:
   time_t Time;
};

std::ostream& operator<< (std::ostream &, date &);

#endif
