#include "dates.hxx"
#include <locale.h>
#include <stdio.h>

namespace itlib
{
   date::date()
   {
      setlocale(LC_ALL, "");
      Time = time((time_t *)NULL);
   };

   date::date(std::string Date)
   {
      tm tm_T;
      // memset(&tm_T, '\0', sizeof(tm));
      sscanf(Date.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &tm_T.tm_year, &tm_T.tm_mon,
             &tm_T.tm_mday, &tm_T.tm_hour, &tm_T.tm_min, &tm_T.tm_sec);
      tm_T.tm_year = tm_T.tm_year - 1900;
      tm_T.tm_mon = tm_T.tm_mon - 1;
      Time = mktime(&tm_T);
   };

   void date::SetDay(int Value)
   {
      tm *tm_T;
      tm_T = localtime(&Time);
      tm_T->tm_mday = Value;
      Time = mktime(tm_T);
   };

   void date::SetYear(int Value)
   {
      tm *tm_T;
      if (Value > 1900)
         Value -= 1900;
      tm_T = localtime(&Time);
      tm_T->tm_year = Value;
      Time = mktime(tm_T);
   };

   void date::SetMonth(int Value)
   {
      tm *tm_T;
      tm_T = localtime(&Time);
      tm_T->tm_mon = Value - 1;
      Time = mktime(tm_T);
   };

   void date::SetHour(int Value)
   {
      tm *tm_T;
      tm_T = localtime(&Time);
      tm_T->tm_hour = Value;
      Time = mktime(tm_T);
   };

   void date::SetMin(int Value)
   {
      tm *tm_T;
      tm_T = localtime(&Time);
      tm_T->tm_min = Value;
      Time = mktime(tm_T);
   };

   void date::SetSec(int Value)
   {
      tm *tm_T;
      tm_T = localtime(&Time);
      tm_T->tm_sec = Value;
      Time = mktime(tm_T);
   };

   std::string date::GetFDate()
   {
      char AuxTime[1024];
      tm *tm_Time = localtime(&Time);
      strftime(AuxTime, 1023, "%Y-%m-%d %H:%M:%S", tm_Time);
      return std::string(AuxTime);
   };

   std::string date::GetSQLDate()
   {
      char AuxTime[1024];
      tm *tm_Time = localtime(&Time);
      strftime(AuxTime, 1023, "%Y-%m-%d", tm_Time);
      return std::string(AuxTime);
   };

   std::string date::GetSQLDateTime()
   {
      char AuxTime[1024];
      tm *tm_Time = localtime(&Time);
      strftime(AuxTime, 1023, "%Y-%m-%d %H:%M:%S%z", tm_Time);
      return std::string(AuxTime);
   };

   std::ostream &operator<<(std::ostream &o, date &d)
   {
      return o << d.GetFDate();
   };
};