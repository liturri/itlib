#ifndef __DATES_H
#define __DATES_H

#include <ctime>
#include <iostream>
#include <string>

// using std::string;
namespace itlib
{
   class date
   {
   public:
      date();
      date(std::string Date);
      date(const time_t &Time) : Time(Time){};

      std::string GetFDate();
      std::string GetSQLDate();
      std::string GetSQLDateTime();

      void SetDay(int Value);
      void SetYear(int Value);
      void SetMonth(int Value);
      void SetHour(int Value);
      void SetMin(int Value);
      void SetSec(int Value);

      bool operator<(const date &d) { return this->Time < d.Time; };
      bool operator>(const date &d) { return this->Time > d.Time; };
      bool operator<=(const date &d) { return this->Time <= d.Time; };
      bool operator>=(const date &d) { return this->Time >= d.Time; };
      bool operator==(const date &d) { return this->Time == d.Time; };

   private:
      time_t Time;
   };

   std::ostream &operator<<(std::ostream &, date &);
};
#endif
