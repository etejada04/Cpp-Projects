#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidDateException
{
};
#endif /* __PROGTEST__ */

// uncomment if your code implements the overloaded suffix operators
#define TEST_LITERALS
enum TypeDate: uint8_t {
    y, //Type year
    m, //Type month
    d //Type day
};

struct Inter{
    TypeDate type;
    int val;
};

class CPeriod{
public:
    vector<Inter> inter;
    CPeriod( const TypeDate t, const int v): type(t), value(v){
        Inter myinter = {t, v};
        inter.push_back(myinter);
    };
    ~CPeriod(){
        inter.clear();
    };

    TypeDate getType() const {
        return type;
    }

    int getValue() const {
        return value;
    }

    friend const CPeriod operator+(CPeriod a, const CPeriod &b);
    friend const CPeriod operator-(CPeriod a, const CPeriod &b);
    friend const CPeriod operator-(CPeriod a);
    /*Literals operators*/
    friend CPeriod operator""_year(long long unsigned int yearVal);
    friend CPeriod operator""_years(long long unsigned int yearVal);
    friend CPeriod operator""_month(long long unsigned int monthVal);
    friend CPeriod operator""_months(long long unsigned int monthVal);
    friend CPeriod operator""_day(long long unsigned int dayVal);
    friend CPeriod operator""_days(long long unsigned int dayVal);

    /*Literals operators*/

private:
    TypeDate type;
    int value;

};

class CDate
{
public:
    // constructor
    CDate(const int y = 0, const int m = 0,const int d = 0): year(y), month(m), day(d){
        if(!DateValidator(y,m,d))
            throw InvalidDateException();
    };
    ~CDate(){};
    int getYear() const{
        return year;
    }

    int getMonth()const{
        return month;
    }

    int getDay()const{
        return day;
    }

    // operator(s) +
    friend const CDate operator+(const CDate &a, const CPeriod &b);
    friend long operator+(const CDate &a, const CDate &b);
    CDate add(const CPeriod &a) const;
    long add(const CDate &a) const;
    // operator(s) -
    friend const CDate operator-(const CDate &a, const CPeriod &b);
    friend long operator-(const CDate &a, const CDate &b);
    CDate diff(const CPeriod &a)const;
    long diff(const CDate &a)const;
    // operator ==
    friend bool operator==(const CDate &a, const CDate &b);
    friend bool operator==(const CDate &a, const long &numDays);
    bool equal(const CDate &a) const;
    bool equal(const long &numDays) const;
    // operator !=
    friend bool operator!=(const CDate &a, const CDate &b);
    bool notequal(const CDate &a) const;
    // operator <
    friend bool operator<(const CDate &a, const CDate &b);
    bool lessthan(const CDate &a) const;
    // operator(s) +=
    friend const CDate operator+=( CDate& a, const CPeriod &b);
    CDate increment(const CPeriod &a) const;
    // operator <<
    friend ostream& operator<<(ostream&, const CDate &x);
private:
    int year, month, day;
    bool IsLeap(int y) const;
    bool DateValidator(int y, int m, int d) const;
    long NumberOfDays(int y, int m, int d) const;
    int DaysMonth(int m, int y) const;
    void DaysToDate(long dayNum, int & y, int & m, int & d)const;

};
/*****************************CDate*****************************************/
bool CDate::IsLeap(int year) const{
    if ( year == 1){
        return true;
    } else if (year % 4 == 0 && year % 100 != 0 && year % 4000 != 0)
    {
        return true;
    } else if (year % 400 == 0 && year % 100 == 0 && year % 4000 != 0)
    {
        return true;
    } else if (year % 4 == 0 && year % 100 == 0 && year % 4000 == 0)
    {
        return false;
    }

    return 0;
}

bool CDate::DateValidator(int y, int m, int d) const {
    switch (m)
    {
        case 1://January
        case 3://March
        case 5://May
        case 7://July
        case 8://August
        case 10://October
        case 12://December
            if (d > 31 || d <= 0)
            {
                return false;
            }
            break;
        case 4://April
        case 6://June
        case 9://September
        case 11://November
            if (d > 30 || d <=0)
            {
                return false;
            }
            break;
        case 2://February
            if ( (IsLeap(y) && d > 29) || d <= 0)
            {
                return false;
            }
            else if ( (!(IsLeap(y)) && d > 28) || d <= 0)
            {
                return false;
            }
            break;
        default:
            return false;
    }

    return y > 1599;
}

int CDate::DaysMonth(int m, int y) const {
    int month[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int res = 0;
    for (int i = 0; i < m-1; ++i) {
        if(i == 1){
            if(IsLeap(y)){
                res += 29;
            } else {
                res += 28;
            }
        } else {
            res += month[i];
        }
    }
    return res;
}

long CDate::NumberOfDays(int y, int m, int d) const {
    long count = 0;
    for (int i = 1; i < y; i++) {
        if( IsLeap(i)){
            count += 366;
        } else {
            count += 365;
        }
    }
    count+= DaysMonth(m, y);
    count+=d;
    return count;
}

void CDate::DaysToDate(long dayNum, int & newY, int & newM, int & newD) const {
    long rem1 = 0, rem2 = 0, rem3 = 0, rem4 = 0, rem5 = 0;
    int year = 0, month = 0, day = 0;
    int y = 365;
    int year_4 = y * 4 +1;// 1461
    int year_100 = year_4 * 25 -1;// 36524
    int year_400 = year_100 * 4 + 1;// 146097
    int year_4000 = year_400 * 10 -1;// 1460969

    rem1 = dayNum % year_4000;
    year += ((dayNum - rem1)/year_4000) * 4000;

    dayNum = rem1;

    if( rem1 > year_4000 - 365){
        year+=3999;
        dayNum -= (year_4000-365);
    } else {
        rem2 = dayNum % year_400;
        year += ((dayNum - rem2)/year_400) * 400;

        dayNum = rem2;

        if( rem2 > year_400 - 366){
            year+=399;
            dayNum -= (year_400-366);
        } else {
            rem3 = dayNum % year_100;
            year += ((dayNum - rem3)/year_100) * 100;

            dayNum = rem3;

            if( rem3 > year_100 - 365){
                year+=99;
                dayNum -= (year_100-365);
            } else {
                rem4 = dayNum % year_4;
                year += ((dayNum - rem4)/year_4) * 4;

                dayNum = rem4;

                if( rem4 > year_4 - 366){
                    year+=3;
                    dayNum -= (year_4-366);
                } else {
                    rem5 = (dayNum % y);
                    year += ((dayNum - rem5)/y);
                    dayNum = rem5;
                }
            }
        }
    }
    year += 1;
    if(dayNum == 0){
        year -= 1;
        month = 12;
        day = 31;

        newY = year;
        newM = month;
        newD = day;
        return;
    }
    day =(int) dayNum;
    if (day <= 31) {
        month=1;
    } else if (day <= 59 + IsLeap(year)) {
        month = 2;
        day-= 31;
    } else if (day <= 90 + IsLeap(year)) {
        month = 3;
        day-= (59 + IsLeap(year));
    } else if (day <= 120 + IsLeap(year)) {
        month = 4;
        day-= (90 + IsLeap(year));
    } else if (day <= 151 + IsLeap(year)) {
        month = 5;
        day-= (120 + IsLeap(year));
    } else if (day <= 181 + IsLeap(year)) {
        month = 6;
        day-= (151 + IsLeap(year));
    } else if (day <= 212 + IsLeap(year)) {
        month = 7;
        day-= (181 + IsLeap(year));
    } else if (day <= 243 + IsLeap(year)) {
        month = 8;
        day-= (212 + IsLeap(year));
    } else if (day <= 273 + IsLeap(year)) {
        month = 9;
        day-= (243 + IsLeap(year));
    } else if (day <= 304 + IsLeap(year)) {
        month = 10;
        day-= (273 + IsLeap(year));
    } else if (day <= 334 + IsLeap(year)) {
        month = 11;
        day-= (304 + IsLeap(year));
    } else if (day <= 365 + IsLeap(year)) {
        month = 12;
        day-= (334 + IsLeap(year));
    }
    newY = year;
    newM = month;
    newD = day;
}

CDate CDate::add(const CPeriod &a) const{
    int newY = 0, newM = 0, newD = 0;
    int inVal = a.getValue();
    long dayNum = 0;

    if(inVal < 0 ){
        inVal*=-1; //Change sign because it's already done in the operator
    }

    if(a.getType()==y){
        newY = year + inVal;
        newM = month;
        newD = day;
    } else if (a.getType()==m){
        newY = year + (month-1 + inVal)/12;
        newM = 1 + (month-1 + inVal)%12;
        newD = day;
    } else {
        dayNum = NumberOfDays(year, month, day) + inVal;
        DaysToDate(dayNum-1, newY, newM, newD);
    }

    return !DateValidator(newY, newM, newD) ? throw InvalidDateException():CDate(newY, newM, newD);
}

long CDate::add(const CDate &a) const {
    long numDays1 = NumberOfDays(year, month, day)-1;
    long numDays2 = NumberOfDays(a.getYear(), a.getMonth(), a.getDay())-1;
    return numDays1+numDays2;
}

CDate CDate::diff(const CPeriod &a) const{
    int newY = 0, newM = 0, newD = 0;
    int inVal = a.getValue();
    long dayNum = 0;

    if(inVal < 0){
        inVal*=-1; //Change sign because it's already done in the operator
    }

    if(a.getType()==y){
        newY = year - inVal;
        newM = month;
        newD = day;
    } else if (a.getType()==m){
        newY = year + (month-1 - inVal)/12;
        newM = 1 + (month-1 - inVal)%12;
        if(newM <= 0){
            newM+=12;
            newY-=1;
        }
        newD = day;
    } else{
        dayNum = NumberOfDays(year, month, day) - inVal;
        DaysToDate(dayNum-1, newY, newM, newD);
    }

    return !DateValidator(newY, newM, newD) ? throw InvalidDateException():CDate(newY, newM, newD);
}

long CDate::diff(const CDate &a) const {
    long numDays1 = NumberOfDays(year, month, day)-1;
    long numDays2 = NumberOfDays(a.getYear(), a.getMonth(), a.getDay())-1;
    return numDays1-numDays2;
}

CDate CDate::increment(const CPeriod &a) const {
    CDate res (year, month, day);
    for (int i = 0; i < (int) a.inter.size(); ++i) {
        res = res + CPeriod(a.inter[i].type, a.inter[i].val);
    }
    return res;
}

bool CDate::equal(const CDate &a) const {
    bool flag = false;
    if(year==a.getYear() && month==a.getMonth() && day==a.getDay())
        flag = true;

    return flag;
}

bool CDate::equal(const long &numDays) const {
    long days = 0;
    days = NumberOfDays(year, month, day)-1;
    return (days+366)==numDays;
}

bool CDate::notequal(const CDate &a) const {
    bool flag = false;
    if(year!=a.getYear() || month!=a.getMonth() || day!=a.getDay())
        flag = true;
    return flag;
}

bool CDate::lessthan(const CDate &a) const {
    long numDays1 = NumberOfDays(year, month, day)-1;
    long numDays2 = NumberOfDays(a.getYear(), a.getMonth(), a.getDay())-1;
    return numDays1 < numDays2;
}

const CDate operator+(const CDate &a, const CPeriod &b){
    if(b.getValue() < 0)
        return a.diff(b);
    else
        return a.add(b);
}

long operator+(const CDate &a, const CDate &b){
    /*if(b.getYear() < 0 || b.getMonth() < 0 || b.getDay() < 0)
        return a.diff(b);
    else*/
    return a.add(b);
}

const CDate operator-(const CDate &a, const CPeriod &b){
    if(b.getValue() < 0 )
        return a.add(b);
    else
        return a.diff(b);
}

long operator-(const CDate &a, const CDate &b){
    /*if(b.getYear() < 0 || b.getMonth() < 0 || b.getDay() < 0)
        return a.diff(b);
    else*/
    return a.diff(b);
}

bool operator==(const CDate &a, const CDate &b) {
    return a.equal(b);
}

bool operator==(const CDate &a, const long &numDays){
    return a.equal(numDays);
}

bool operator!=(const CDate &a, const CDate &b) {
    return a.notequal(b);

}

bool operator<(const CDate &a, const CDate &b) {
    return a.lessthan(b);

}

const CDate operator+=(CDate& a, const CPeriod &b) {
    //CDate c = a.increment(b);
    a = a.increment(b);
    return a;
}

ostream & operator <<(ostream& oss, const CDate &x){
    ostringstream os;
    os << x.getYear() << "-" << setfill('0') << setw(2) << x.getMonth() << "-" << setfill('0') << setw(2) << x.getDay();
    oss << os.str();
    return oss;
}
/*****************************CDate*****************************************/

/*****************************CPeriod*****************************************/
CPeriod Year(const int & yearVal) {
    return CPeriod(y, yearVal);
}

CPeriod Month(const int & monthVal) {
    return CPeriod(m, monthVal);
}

CPeriod Day(const int & dayVal) {
    return CPeriod(d, dayVal);
}

const CPeriod operator+(CPeriod a, const CPeriod &b){
    Inter myinter1 = {b.getType(), b.getValue()};
    a.inter.push_back(myinter1);
    return a;
}

const CPeriod operator-(CPeriod a, const CPeriod &b){
    Inter myinter1 = {b.getType(), -b.getValue()};
    a.inter.push_back(myinter1);
    return a;
}

const CPeriod operator-(CPeriod a){
    return CPeriod(a.getType(), -a.getValue());
}

/*****************************CPeriod*****************************************/
/*****************************Literals*****************************************/
CPeriod operator""_year(long long unsigned int yearVal){
    return CPeriod(y, (const int) yearVal);
}

CPeriod operator""_years(long long unsigned int yearVal){
    return CPeriod(y, (const int) yearVal);
}

CPeriod operator""_month(long long unsigned int monthVal){
    return CPeriod(m, (const int) monthVal);
}

CPeriod operator""_months(long long unsigned int monthVal){
    return CPeriod(m, (const int) monthVal);
}

CPeriod operator""_day(long long unsigned int dayVal){
    return CPeriod(d, (const int) dayVal);
}

CPeriod operator""_days(long long unsigned int dayVal){
    return CPeriod(d, (const int) dayVal);
}
/*****************************Literals*****************************************/

#ifndef __PROGTEST__
string             toString                                ( const CDate     & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int                main                                    ( void )
{
    CDate tmp ( 2000, 1, 1 );

    assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
    assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
    assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
    try
    {
        tmp = CDate ( 1900, 2, 29 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
    try
    {
        tmp = CDate ( 4000, 2, 29 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 1 ) )  == "2019-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 7 ) )  == "2025-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 73 ) )  == "2091-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 3 ) )  == "2018-06-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 1 ) )  == "2018-04-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 285 ) )  == "2041-12-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1 ) )  == "2018-03-16" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 12 ) )  == "2018-03-27" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1234567 ) )  == "5398-05-02" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 3 ) + Year ( 2 ) + Month ( 3 ) + Day ( 5 ) + Month ( 11 ) )  == "2021-05-23" );
    try
    {
        tmp = CDate ( 2000, 2, 29 ) + Year ( 300 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    try
    {
        tmp = CDate ( 2000, 3, 30 ) + Month ( 11 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    assert ( toString ( CDate ( 2001, 2, 20 ) + Day ( 9 ) )  == "2001-03-01" );
    assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 1999, 1, 1 ) + Year ( 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 2001, 1, 1 ) + Day ( 1095 ) + Day ( 28 ) + Month ( 1 ) )  == "2004-02-29" );
    try
    {
        tmp = CDate ( 2001, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Day ( 1095 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    try
    {
        tmp = CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Year ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2000, 1, 1 );
    assert ( toString ( tmp - Year ( 5 ) + Month ( 2 ) )  == "1995-03-01" );
    assert ( toString ( tmp ) == "2000-01-01" );
    assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
    assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 2 ) - Month ( -3 ) + Day ( -10 ) )  == "1998-03-22" );
    assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 59 ) - Month ( 1 ) - Year ( 2 ) )  == "1998-01-29" );
    try
    {
        tmp = CDate ( 2000, 1, 1 ) + Day ( 59 ) - Year ( 2 ) - Month ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2000, 1, 1 );
    tmp +=  - Year ( 2 ) - Month ( -3 ) + Day ( -10 );
    assert ( toString ( tmp ) == "1998-03-22" );
    tmp = CDate ( 2000, 1, 1 );
    tmp += Day ( 59 ) - Month ( 1 ) - Year ( 2 );
    assert ( toString ( tmp ) == "1998-01-29" );
    try
    {
        tmp = CDate ( 2000, 1, 1 );
        tmp += Day ( 59 ) - Year ( 2 ) - Month ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2018, 3, 15 ) + Day ( -3 );
    assert ( toString ( tmp ) == "2018-03-12" );
    assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
    assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
    assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
    assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
    assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) == CDate ( 2018, 3, 15 ) ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( 1 ) != CDate ( 2018, 3, 15 ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) < CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( -1 ) == CDate ( 2018, 3, 15 ) ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) != CDate ( 2018, 3, 15 ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) < CDate ( 2018, 3, 15 ) );
    assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
    assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
    assert ( CDate ( 2018, 3, 15 ) + Year ( 3 ) + Month ( -18 ) - CDate ( 2000, 1, 1 ) == 7197 );
    assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
#ifdef TEST_LITERALS
    assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 7_years )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 73_years )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_months )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_month )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 285_months )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_day )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 12_days )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1234567_days )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_days + 2_years + 3_months + 5_days + 11_months )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + 300_years;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + 11_months;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + 9_days )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + 1_year + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + 1095_days + 28_days + 1_month )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + 28_days + 1_month + 1095_days;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 28_days + 1_month + 1_year;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - 5_years + 2_months )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 1_year - 3_months - 10_days )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 2_years - ( -3_months ) + ( -10_days ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 59_days - 1_month - 2_years )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - 2_years - ( -3_months ) + ( -10_days );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += 59_days - 1_month - 2_years;
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + ( -3_days );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + 1_day != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + ( -1_day ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) < CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2018, 3, 15 ) + 3_years + ( -18_months ) - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
  ostringstream oss;
  oss << setfill ( 'x' ) << left << hex << CDate ( 2000, 1, 1 ) << ' ' << setw ( 10 ) << 65536;
  assert ( oss . str () == "2000-01-01 10000xxxxx" );
#endif /* TEST_LITERALS */
    return 0;
}
#endif /* __PROGTEST__ */
