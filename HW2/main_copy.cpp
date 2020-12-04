#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
#define INIT_SIZE 100

struct EmployeesByEmail {
    string name = " ", surname = " ", email = " ";
    unsigned int salary = 0;

};

struct EmployeesByName {
    string name = " ", surname = " ", email = " ";
    unsigned int salary = 0;

};

class CPersonalAgenda
{
public:
    CPersonalAgenda  ( void );
    ~CPersonalAgenda ( void );
    bool          Add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary );
    bool          Del              ( const string    & name,
                                     const string    & surname );
    bool          Del              ( const string    & email );
    bool          ChangeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname );
    bool          ChangeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail );
    bool          SetSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary );
    bool          SetSalary        ( const string    & email,
                                     unsigned int      salary );
    unsigned int  GetSalary        ( const string    & name,
                                     const string    & surname ) const;
    unsigned int  GetSalary        ( const string    & email ) const;
    bool          GetRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetFirst         ( string          & outName,
                                     string          & outSurname ) const;
    bool          GetNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const;


private:
    int size = 0, counter = 0;
    EmployeesByName * struct1 {};
    EmployeesByEmail * struct2 {};

    void IncreaseSize();
    int BinSearch(const string &name, const string &surname, int L, int R) const;
    int BinSearch(const string &email, int L, int R) const;
    int GetIndexBS(const string &name, const string &surname, int L, int R) const;
    int GetIndexBS(const string &email, int L, int R) const;

};

CPersonalAgenda::CPersonalAgenda() {
    counter = 0;
    size = INIT_SIZE;
    struct1 = new EmployeesByName[size];
    struct2 = new EmployeesByEmail[size];
}

CPersonalAgenda::~CPersonalAgenda() {
    delete[] struct1;
    delete[] struct2;

}

bool CPersonalAgenda::Add(const string &name, const string &surname, const string &email, unsigned int salary) {
    if(BinSearch(name, surname, 0, counter-1)!=-1 || BinSearch(email, 0, counter-1) !=-1){ //Already added or not found
        return false; //item is already there
    } else {
        int tmp1, tmp2;
        tmp1 = GetIndexBS(name, surname, 0, counter-1);
        tmp2 = GetIndexBS(email, 0, counter-1);

        if( counter+1 == size){
            IncreaseSize();
        }

        for (int i = counter-1; i >= tmp1; --i) {
            struct1[i+1].name = struct1[i].name;
            struct1[i+1].surname = struct1[i].surname;
            struct1[i+1].email = struct1[i].email;
            struct1[i+1].salary = struct1[i].salary;

        }

        for (int i = counter-1; i >= tmp2; --i) {
            struct2[i+1].name = struct2[i].name;
            struct2[i+1].surname = struct2[i].surname;
            struct2[i+1].email = struct2[i].email;
            struct2[i+1].salary = struct2[i].salary;

        }

        //Struct sorted by name + surname
        struct1[tmp1].name = name;
        struct1[tmp1].surname = surname;
        struct1[tmp1].email = email;
        struct1[tmp1].salary = salary;

        //Struct sorted by email
        struct2[tmp2].name = name;
        struct2[tmp2].surname = surname;
        struct2[tmp2].email = email;
        struct2[tmp2].salary = salary;

        counter++;

        return true;
    }
}

bool CPersonalAgenda::Del(const string &name, const string &surname) {

    if(BinSearch(name, surname, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index1 = BinSearch(name, surname, 0, counter-1);
    int index2 = BinSearch(struct1[index1].email, 0, counter-1);

    for (int i = index1; i < counter - 1; ++i) {
        struct1[i] = struct1[i+1];
    }

    for (int i = index2; i < counter - 1; ++i) {
        struct2[i] = struct2[i+1];
    }
     counter--;


    return true;
}

bool CPersonalAgenda::Del(const string &email) {
    if(BinSearch( email, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index1 = BinSearch(email, 0, counter-1);
    int index2 = BinSearch(struct2[index1].name, struct2[index1].surname, 0, counter-1);

    for (int i = index1; i < counter - 1; ++i) {
        struct1[i] = struct1[i+1];
    }

    for (int i = index2; i < counter - 1; ++i) {
        struct2[i] = struct2[i+1];
    }
    counter--;


    return true;
}

bool CPersonalAgenda::ChangeName(const string &email, const string &newName, const string &newSurname) {

    if(BinSearch(email, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index1 = BinSearch(email, 0, counter-1);
    int index2  = BinSearch(struct2[index1].name, struct2[index1].surname, 0, counter-1);
    string email1, name, surname;
    unsigned int salary = 0;
    salary = struct1[index2].salary;
    name = struct1[index2].name;
    surname = struct1[index2].surname;

    Del(struct1[index2].email);

    if(!Add(newName, newSurname, email, salary ) || (name == newName && surname == newSurname) ){
        Add(name, surname, email, salary);
        return false;
    }

    return true;

}

bool CPersonalAgenda::ChangeEmail(const string &name, const string &surname, const string &newEmail) {

    if(BinSearch(name, surname, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index = BinSearch(name, surname, 0, counter-1);
    string email;
    unsigned int salary = 0;
    salary = struct1[index].salary;
    email = struct1[index].email;

    Del(struct2[index].email);

    if(!Add(name, surname, newEmail, salary ) || email == newEmail ){
        Add(name, surname, email, salary);
        return false;
    }

    return true;
}

bool CPersonalAgenda::SetSalary(const string &name, const string &surname, unsigned int salary) {

    if(BinSearch(name, surname, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index1 = BinSearch(name, surname, 0 , counter-1);
    int index2 = BinSearch(struct1[index1].email, 0, counter-1);

    struct1[index1].salary = salary;
    struct2[index2].salary = salary;

    return true;
}

bool CPersonalAgenda::SetSalary(const string &email, unsigned int salary) {
    if(BinSearch( email, 0, counter-1)==-1){ //Not found
        return false;
    }

    int index1 = BinSearch(email, 0, counter-1);
    int index2  = BinSearch(struct2[index1].name, struct2[index1].surname, 0, counter-1);

    struct1[index2].salary = salary;
    struct2[index1].salary = salary;

    return true;
}

unsigned int CPersonalAgenda::GetSalary(const string &name, const string &surname) const {
    if(BinSearch(name, surname, 0, counter-1)==-1){ //Not found
        return 0;
    }

    int index = BinSearch(name, surname, 0, counter-1);

    return struct1[index].salary;
}

unsigned int CPersonalAgenda::GetSalary(const string &email) const {
    if(BinSearch(email, 0, counter-1)==-1){ //Not found
        return 0;
    }

    int index = BinSearch(email, 0, counter-1);

    return struct2[index].salary;
}

bool CPersonalAgenda::GetRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {

    if(BinSearch(name, surname, 0, counter-1)==-1){ //Not found
        return 0;
    }

    int index = BinSearch(name, surname, 0, counter-1);
    unsigned int salary = struct1[index].salary;
    int less = 0, equal = 0;
    for (int i = 0; i < counter; ++i) {
        if(struct1[i].salary==salary)
            equal++;
        else if(struct1[i].salary < salary)
            less++;
    }
    rankMin = less;
    rankMax = (rankMin + equal)-1;
    return true;
}

bool CPersonalAgenda::GetRank(const string &email, int &rankMin, int &rankMax) const {
    if(BinSearch(email, 0, counter-1)==-1){//Not found
        return 0;
    }

    int index1 = BinSearch(email, 0, counter-1);
    int index2  = BinSearch(struct2[index1].name, struct2[index1].surname, 0, counter-1);
    unsigned int salary = struct1[index2].salary;
    int less = 0, equal = 0;
    for (int i = 0; i < counter; ++i) {
        if(struct1[i].salary==salary)
            equal++;
        else if(struct1[i].salary < salary)
            less++;
    }
    rankMin = less;
    rankMax = (rankMin + equal)-1;
    return true;
}

bool CPersonalAgenda::GetFirst(string &outName, string &outSurname) const {
    if(counter==0 ){ //DBS is empty
        return false;
    }
    outName=struct1[0].name;
    outSurname = struct1[0].surname;
    return struct1[0].name==outName && struct1[0].surname==outSurname;
}

bool CPersonalAgenda::GetNext(const string &name, const string &surname, string &outName, string &outSurname) const {

    if(BinSearch(name, surname, 0, counter-1)==-1){//Not found
        return 0;
    }


    int index = BinSearch(name, surname, 0 , counter-1);
    if(counter==0 || index+1==counter){ //DBS is empty or index is last element in DBS
        return false;
    }

    outName = struct1[index+1].name;
    outSurname = struct1[index+1].surname;

    return true;
}

 void CPersonalAgenda::IncreaseSize() {
     int resize = size+1000;
     EmployeesByName * tmp1 = new EmployeesByName[resize];
     EmployeesByEmail * tmp2 = new EmployeesByEmail[resize];
     size = resize;
     for (int i = 0; i < counter; ++i) {
         tmp1[i].name = struct1[i].name;
         tmp1[i].surname = struct1[i].surname;
         tmp1[i].email = struct1[i].email;
         tmp1[i].salary = struct1[i].salary;

         tmp2[i].name = struct2[i].name;
         tmp2[i].surname = struct2[i].surname;
         tmp2[i].email = struct2[i].email;
         tmp2[i].salary = struct2[i].salary;
     }

     delete[]struct1;
     delete[]struct2;

     struct1 =  tmp1;
     struct2 =  tmp2;

 }

int CPersonalAgenda::BinSearch(const string &name, const string &surname, int L, int R) const {

    while(L <= R){
        int middle =L+(R-L)/2;

        if(struct1[middle].surname < surname){

            L = middle+1;
        } else if(struct1[middle].surname > surname) {

            R = middle-1;
        } else if(struct1[middle].name < name) {

            L = middle+1;
        } else if(struct1[middle].name > name) {

            R = middle-1;
        } else {
            return middle;
        }
    }

    return -1;
}

int CPersonalAgenda::BinSearch(const string &email, int L, int R) const {


    while (L<=R){
        int middle =L+(R-L)/2;

        if(struct2[middle].email < email) {

            L = middle+1;
        } else if (struct2[middle].email > email) {

            R = middle-1;
        } else {
            return middle;
        }
    }

    return -1;
}

int CPersonalAgenda::GetIndexBS(const string &name, const string &surname, int L, int R) const {
    if(L >= R || R < 0){
        int res;
        if( counter > 0 && struct1[L].surname < surname)
            res = L+1;
        else if (counter > 0 && struct1[L].name < name)
            res = L+1;
        else
            res = L;
        return res;
    }

    int middle =(R+L)/2;

    if(struct1[middle].surname < surname){
        return GetIndexBS(name, surname, middle+1, R);
    } else if(struct1[middle].surname > surname) {
        return GetIndexBS(name, surname, L , middle-1);
    } else if(struct1[middle].name < name) {
        return GetIndexBS(name, surname, middle+1, R);
    } else if(struct1[middle].name > name) {
        return GetIndexBS(name, surname, L , middle-1);
    } else {
        return middle;
    }

    return -1;
}

int CPersonalAgenda::GetIndexBS(const string &email, int L, int R) const {
    if(L >= R || R < 0){
        int res;
        if( counter > 0 && struct2[L].email < email)
            res = L+1;
        else
            res = L;
        return res;
    }

    int middle = (R+L)/2;

    if(struct2[middle].email < email) {
        return GetIndexBS(email, middle+1, R);
    } else if (struct2[middle].email > email) {
        return GetIndexBS(email, L , middle-1);
    } else {
        return middle;
    }
   
    return -1;
}


#ifndef __PROGTEST__
int main ( void )
{
    string outName, outSurname;
    int lo, hi;
    /*Invalid input test*/
    CPersonalAgenda b3;

    assert ( b3 . Add ( "John", "Smith", "john", 30000 ) );
    assert ( b3 . Add ( "Peter", "Smith", "peter", 30000 ) );
    assert ( !b3 . Add ( "John", "Miller", "john", 35000 ) );
    assert ( !b3 . Add ( "John", "Smith", "john", 35000 ) );
    assert ( !b3 . Del ( "Peter", "James") );
    assert ( !b3 . Del ("james") );
    assert ( !b3 . ChangeName ("peter", "John", "Smith") );
    assert ( !b3 . ChangeName ("james", "John", "Cena") );
    assert ( !b3 . ChangeEmail ("John", "Cena", "cena") );
    assert ( !b3 . ChangeEmail ("John", "Smith", "peter") );
    assert ( !b3 . SetSalary ("cena", 4000) );
    assert ( !b3 . SetSalary ("John", "Cena", 4000) );
    assert ( !b3 . GetSalary ("cena") );
    assert ( !b3 . GetSalary ("John", "Cena") );
    assert ( !b3 . GetRank ("cena", lo, hi) );
    assert ( !b3 . GetRank ("John", "Cena", lo, hi) );
    assert(b3.Del("john"));
    assert(b3.Del("peter"));
    assert ( !b3 . GetFirst (outName, outSurname) );
    assert(b3.Add("John", "Cena", "jcena" , 100000));
    assert ( !b3 . GetNext ("John", "Cena", outName, outSurname) );
    assert ( !b3 . GetNext ("John", "Smith", outName, outSurname) );
    assert(b3.Add ("XXX", "YYY", "ZZZ", 30));
    assert(b3.Add ("XX", "XYYY", "ZZZ2", 30));
    /*Invalid input test*/


    CPersonalAgenda b1;

    assert ( b1 . Add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . Add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . Add ( "Peter", "Smith", "peter", 23000 ) );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . GetNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "Peter", "Smith", outName, outSurname ) );
    assert ( b1 . SetSalary ( "john", 32000 ) );
    assert ( b1 . GetSalary ( "john" ) ==  32000 );
    assert ( b1 . GetSalary ( "John", "Smith" ) ==  32000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );
    assert ( b1 . SetSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . GetSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . GetSalary ( "john" ) ==  35000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . ChangeName ( "peter", "James", "Bond" ) );
    assert ( b1 . GetSalary ( "peter" ) ==  23000 );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . GetNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . ChangeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "james" ) ==  23000 );
    assert ( b1 . GetSalary ( "peter" ) ==  0 );
    assert ( b1 . Del ( "james" ) );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );
    assert ( b1 . Del ( "John", "Miller" ) );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . Del ( "john" ) );
    assert ( ! b1 . GetFirst ( outName, outSurname ) );
    assert ( b1 . Add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . Add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . Add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . GetFirst ( outName, outSurname ) );
    assert ( b2 . Add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . Add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . Add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . Add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . SetSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . SetSalary ( "joe", 90000 ) );
    assert ( b2 . GetSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . GetSalary ( "joe" ) ==  0 );
    assert ( ! b2 . GetRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . GetRank ( "joe", lo, hi ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . Del ( "Joe", "Black" ) );
    assert ( ! b2 . Del ( "joe" ) );
    assert ( ! b2 . ChangeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . SetSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . GetRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . SetSalary ( "peter", 37000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  0 );
    assert ( ! b2 . GetRank ( "peter", lo, hi ) );
    assert ( ! b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . Del ( "peter" ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  40000 );

    return 0;
}
#endif /* __PROGTEST__ */
