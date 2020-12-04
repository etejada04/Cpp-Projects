#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
                             : m_X ( x ),
                               m_Y ( y ),
                               m_W ( w ),
                               m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */
/****** CMaster ******/
class CMaster{
public:
    CMaster(const int & id, const CRect & pos): m_id(id), m_relPos(pos), m_absPos(pos){};

    virtual ~CMaster() {};

    virtual CMaster * clone () const{
        return new CMaster(*this);
    }

    virtual string Type(void) const{
        return "";
    }

    friend ostream &operator << (ostream & os, const CMaster & cmtr){
        cmtr.print(os);
        return os;
    }

    virtual void transform (const CRect & a) {
        m_absPos.m_X = (m_relPos.m_X*a.m_W)+a.m_X;
        m_absPos.m_Y = (m_relPos.m_Y*a.m_H)+a.m_Y;
        m_absPos.m_W = m_relPos.m_W*a.m_W;
        m_absPos.m_H = m_relPos.m_H*a.m_H;
    }
    int getId()const {
        return m_id;
    }

    virtual void print(ostream & os) const {
        os << "[" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
    }

    virtual void printWindow(ostream & os) const {
        os << "+- [" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
    }

    virtual void printWindowBranch(ostream & os) const {
        os << "+- [" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
    }

protected:
    int m_id;
    CRect m_relPos;
    CRect m_absPos;
};
/****** CMaster ******/

/****** CMasterStr ******/
class CMasterStr: public CMaster{
public:
    CMasterStr(const int & id, const CRect & pos, const string & str): CMaster(id, pos), m_title(str){};
    virtual ~CMasterStr() {};
protected:
    virtual void print(ostream & os) const {
        os << "[" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos;
    }

    virtual void printWindow(ostream & os) const {
        os << "+- [" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos;
    }

    string m_title;
};
/****** CMasterStr ******/

/****** CWindow ******/
class CWindow
{
  public:
                             CWindow                       ( const string    & title,
                                                             const CRect     & absPos ):  m_title(title), m_pos(absPos){}

    CWindow(const CWindow & cwin): m_title(cwin.m_title), m_pos(cwin.m_pos){
        for (unsigned int i = 0; i < cwin.m_buttons.size(); ++i) {
            m_buttons.push_back(cwin.m_buttons[i]->clone());
        }
    }

    ~CWindow(){
        for( vector<CMaster*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it) {
            delete *it;
        }
        //m_buttons.clear();
    }

    // Add
    virtual CWindow & Add (const CMaster & src){
        CMaster * cmstr = src.clone();
        cmstr->transform(m_pos);
        m_buttons.push_back(cmstr);
        return *this;
    }
    // Search
    virtual CMaster * Search (int index){
        CMaster * res = NULL;
        for (CMaster * cmstr : m_buttons) {
            if(cmstr->getId() == index){
                res = cmstr;
                break;
            }
        }
        return res;
    }

    /*virtual CMaster *& Search (int index){
        static CMaster * res = nullptr;
        for (CMaster * it : m_buttons) {
            if(it->getId() == index){
                res = it;
                break;
            } else{
                res = NULL;
            }
        }
        return res;
    }*/

    // SetPosition
    virtual void SetPosition (const CRect & pos) {
        m_pos = pos;
        for (CMaster * cmstr : m_buttons){
            cmstr->transform(pos);
        }
    }

    CWindow& operator = (const CWindow & a) {
        for( vector<CMaster*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it) {
            delete *it;
        }
        m_buttons.clear();
        m_pos = a.m_pos;
        m_title = a.m_title;
        for (unsigned int i = 0; i < a.m_buttons.size(); ++i) {
            m_buttons.push_back(a.m_buttons[i]->clone());
        }
        return *this;
    }

    /*CWindow & operator = (const CWindow & a){
        counter = a.counter;
        m_title = a.m_title;
        m_pos = a.m_pos;
        m_buttons = a.m_buttons;
        return *this;

    }*/

    friend ostream &operator << (ostream & os, const CWindow & cwin){
        os << "Window \"" << cwin.m_title << "\" " << cwin.m_pos << '\n';
        /*for (CMaster * it : cwin.m_buttons) {
            //os << "+- ";
            if( cnt >=2 && it->Type()=="ComboBox"){
                it->printWindowBranch(os);
                cnt--;
            } else {
                it->printWindow(os);
            }

        }*/
        for (unsigned int i = 0; i < cwin.m_buttons.size(); ++i) {
            if(cwin.m_buttons[i]->Type()=="ComboBox" && i == cwin.m_buttons.size()-1){
                cwin.m_buttons[i]->printWindow(os);
            } else if( cwin.m_buttons[i]->Type()=="ComboBox"){
                cwin.m_buttons[i]->printWindowBranch(os);
            } else {
                cwin.m_buttons[i]->printWindow(os);
            }
        }
        return os;
    }

private:
    string m_title;
    CRect m_pos;
    vector<CMaster*> m_buttons;

};
/****** CWindow ******/

/****** Button ******/
class CButton : public CMasterStr
{
  public:

    CButton (int id,const CRect & relPos, const string & name): CMasterStr(id, relPos, name ){};
    virtual string Type(void) const override {
        return "Button";
    }

    virtual CMaster * clone () const{
        return new CButton (*this);
    }

private:
    virtual void print(ostream & os) const override{
        os << "[" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

    virtual void printWindow(ostream & os) const override{
        os << "+- [" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

};
/****** Button ******/

/****** Input ******/
class CInput : public CMasterStr
{
  public:
    CInput ( int id, const CRect & relPos, const string & value ): CMasterStr(id, relPos, value){};
    virtual string Type(void) const override{
        return "Input";
    }

    virtual CMaster * clone () const{
        return new CInput (*this);
    }

    // SetValue
    void SetValue(const string & src){
        m_title = src;
    }
    // GetValue
    string GetValue() const {
        return m_title;
    }

private:
    virtual void print(ostream & os) const override{
        os << "[" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

    virtual void printWindow(ostream & os) const override{
        os << "+- [" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

};
/****** Input ******/

/****** Label ******/
class CLabel : public  CMasterStr
{
  public:
    CLabel (int id, const CRect & relPos, const string & label ): CMasterStr(id, relPos, label){};
    virtual string Type(void) const override{
        return "Label";
    }

    virtual CMaster * clone () const{
        return new CLabel (*this);
    }

private:
    virtual void print(ostream & os) const override{
        os << "[" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

    virtual void printWindow(ostream & os) const override{
        os << "+- [" << m_id << "] " << Type() << " \"" << m_title << "\" " << m_absPos << endl;
    }

};
/****** Label ******/

/****** ComboBox ******/
class CComboBox : public CMaster
{
  public:
    CComboBox (int id, const CRect & relPos ): CMaster(id, relPos), m_selected(0){};
    virtual string Type(void) const override{
        return "ComboBox";
    }

    virtual CMaster * clone () const{
        return new CComboBox (*this);
    }

    // Add
    virtual CComboBox & Add (const string & src) {
        m_names.push_back(src);
        return *this;
    }
    // SetSelected
    void SetSelected (unsigned int index){
        m_selected = index;
    }
    // GetSelected
    unsigned int GetSelected(void) const{
        return m_selected;
    }

private:
    virtual void print(ostream & os) const override{
        os << "[" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
        for (unsigned int  i = 0; i < m_names.size(); ++i) {
            if(i == GetSelected()){
                os << "+->" << m_names[i] << '<' << endl;
            } else if (i == GetSelected() && i == m_names.size()-1){
                os << "+->" << m_names[i] << '<' << endl;
            } else {
                os << "+- " << m_names[i] << endl;
            }
        }
    }

    virtual void printWindow(ostream & os) const override{
        os << "+- [" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
        for (unsigned int  i = 0; i < m_names.size(); ++i) {
            if(i == GetSelected()){
                os << "   +->" << m_names[i] << '<' << endl;
            } else if (i == GetSelected() && i == m_names.size()-1){
                os << "   +->" << m_names[i] << '<' << endl;
            } else {
                os << "   +- " << m_names[i] << endl;
            }
        }
        //os << endl;
    }

    virtual void printWindowBranch(ostream & os) const override{
        os << "+- [" << m_id << "]" << ' ' << Type() << ' ' << m_absPos << endl;
        for (unsigned int  i = 0; i < m_names.size(); ++i) {
            if (i == GetSelected() && i == m_names.size()-1){
                os << "|  +->" << m_names[i] << '<' << endl;
            } else if(i == GetSelected()){
                os << "|  +->" << m_names[i] << '<' << endl;
            } else {
                os << "|  +- " << m_names[i] << endl;
            }
        }
        //os << endl;
    }
    unsigned int m_selected ;
    vector <string> m_names;
};
/****** ComboBox ******/
// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );

  CWindow a ( "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . Add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . Add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . Add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . Add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . Add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . Add ( "Karate" ) . Add ( "Judo" ) . Add ( "Box" ) . Add ( "Progtest" ) );
    assert ( toString ( a ) ==
      "Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "   +->Karate<\n"
      "   +- Judo\n"
      "   +- Box\n"
      "   +- Progtest\n" );

    CWindow b = a;
    assert ( toString ( *b . Search ( 20 ) ) ==
      "[20] ComboBox (70,154,480,48)\n"
      "+->Karate<\n"
      "+- Judo\n"
      "+- Box\n"
      "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . GetSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . SetSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . GetValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . SetValue ( "chucknorris@fit.cvut.cz" );
    b . Add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . Add ( "PA2" ) . Add ( "OSY" ) . Add ( "Both" ) );
    assert ( toString ( b ) ==
      "Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "|  +- Karate\n"
      "|  +- Judo\n"
      "|  +- Box\n"
      "|  +->Progtest<\n"
      "+- [21] ComboBox (70,250,480,48)\n"
      "   +->PA2<\n"
      "   +- OSY\n"
      "   +- Both\n" );
    assert ( toString ( a ) ==
      "Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "   +->Karate<\n"
      "   +- Judo\n"
      "   +- Box\n"
      "   +- Progtest\n" );
    b . SetPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
      "Window \"Sample window\" (20,30,640,520)\n"
      "+- [1] Button \"Ok\" (84,446,192,52)\n"
      "+- [2] Button \"Cancel\" (404,446,192,52)\n"
      "+- [10] Label \"Username:\" (84,82,128,52)\n"
      "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
      "+- [20] ComboBox (84,186,512,52)\n"
      "|  +- Karate\n"
      "|  +- Judo\n"
      "|  +- Box\n"
      "|  +->Progtest<\n"
      "+- [21] ComboBox (84,290,512,52)\n"
      "   +->PA2<\n"
      "   +- OSY\n"
      "   +- Both\n" );
    return 0;
}
#endif /* __PROGTEST__ */
