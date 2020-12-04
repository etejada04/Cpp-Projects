#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// enable only if your implementation supports Add ( ) with more than three parameters
#define MULTIPLE_STOPS
template <typename _T, typename _E>
class CAccess
{
public:
    // Add
    CAccess & Add (const _E & e, const _T & x, const _T & y);
    //Multiple adding
    template <typename... args>
    CAccess & Add (const _E & e, const _T & x, const _T & y, const args & ...nextElem);
    // Find (with optional max and filter)
    // Pointer to function
    map <_T, int> Find (const _T & node, unsigned int max = 0,
                        bool (*functionPtr) (const _E & arg) = nullptr) const;
    // Functor or Lambda function
    template <class _C>
    map <_T, int> Find (const _T & node, unsigned int max = 0,
                        _C functionOther = nullptr) const;

private:
    map <_T, vector < pair <_T, _E> > > mapOfCities;
};

/**
 *
 * @tparam _T Argument of type "Node"
 * @tparam _E Argument of type "Edge"
 * @param e Description of the connection: CTrain ("Company", "Speed")
 * @param x Starting node (From)
 * @param y Destination node (To)
 * @return Map of cities with name and their respective connections
 */

template <typename _T, typename _E>
CAccess <_T, _E> & CAccess <_T, _E>::Add (const _E & e, const _T & x, const _T & y)
{
    mapOfCities[x].emplace_back(y, e);
    mapOfCities[y].emplace_back(x, e);
    return *this;
}

/**
 *
 * @tparam _T Argument of type "Node"
 * @tparam _E Argument of type "Edge"
 * @tparam args Variadic template
 * @param e Description of the connection: CTrain ("Company", "Speed")
 * @param x Starting node (From)
 * @param y Destination node (To)
 * @param nextElem Next element of the list of input parameters
 * @return Map of cities with name and their respective connections
 */

template <typename _T, typename _E>
template <typename... args>
CAccess <_T, _E> & CAccess <_T, _E>::Add (const _E & e, const _T & x,  const _T & y,
                                          const args & ...nextElem)
{
    mapOfCities[x].emplace_back(y, e);
    mapOfCities[y].emplace_back(x, e);
    Add(e, x, nextElem...);
    Add(e, y, nextElem...);
    return *this;
}

/**
 *
 * @tparam _T Argument of type "Node"
 * @param arg Name of the city thrown by the exception
 * @return Exception for city not found
 */

template <typename _T>
string Exception (const _T & arg)
{
    stringstream oss;
    oss << "unknown " << arg;
    return oss.str();
}

/**
 *
 * @tparam _T Argument of type "Node"
 * @tparam _E Argument of type "Edge"
 * @param node Name of the city to search, in case is not found an exception is thrown
 * @param max Limit of the maximum number of connections (edges)
 * @param functionPtr Filter for the connections in the form of a pointer-to-function
 * @return Map of cities with the name of places accessible from input city and
 *         minimum number of connections needed to get to those cities from the input city
 */

template <typename _T, typename _E>
map <_T, int> CAccess <_T, _E>::Find (const _T & node, unsigned int max,
                                      bool (*functionPtr) (const _E & arg)) const
{
    if(mapOfCities.find(node) == mapOfCities.end())
    {
        throw invalid_argument(Exception(node));
    }
    map <_T, int> res;
    vector < pair <_T, _E> > adjacentCities = mapOfCities.at(node);
    res[node] = 0;
    for (int i = 1; adjacentCities.size() && (max == 0 || i <= max); ++i)
    {
        vector < pair <_T, _E> > next;
        for (unsigned int j = 0; j < adjacentCities.size(); ++j)
        {
            if (res.find(adjacentCities.at(j).first) != res.end())
            {
                continue;
            }
            if ((functionPtr == nullptr) || functionPtr(adjacentCities.at(j).second))
            {
                res[adjacentCities.at(j).first] = i;
                for (unsigned int k = 0; k < mapOfCities.at(adjacentCities.at(j).first).size(); ++k)
                {
                    next.push_back(mapOfCities.at(adjacentCities.at(j).first)[k]);
                }
            } else {
                continue;
            }
        }
        adjacentCities = next;
    }
    return res;
}

/**
 *
 * @tparam _T Argument of type "Node"
 * @tparam _E Argument of type "Edge"
 * @tparam _C Class template
 * @param node Name of the city to search, in case is not found an exception is thrown
 * @param max Limit of the maximum number of connections (edges)
 * @param functionOther Filter for the connections in the form of a functor or lambda-function
 *@return Map of cities with the name of places accessible from input city and
 *         minimum number of connections needed to get to those cities from the input city
 */

template <typename _T, typename _E>
template <class _C>
map <_T, int> CAccess <_T, _E>::Find (const _T & node, unsigned int max,
                                      _C functionOther) const
{
    if (mapOfCities.find(node) == mapOfCities.end())
    {
        throw invalid_argument(Exception(node));
    }
    map <_T, int> res;
    vector < pair <_T, _E> > adjacentCities = mapOfCities.at(node);
    res[node] = 0;
    for (int i = 1; adjacentCities.size() && (max == 0 || i <= max); ++i)
    {
        vector< pair <_T, _E> > next;
        for (unsigned int j = 0; j < adjacentCities.size(); ++j) {
            if (res.find(adjacentCities.at(j).first) != res.end())
            {
                continue;
            }
            if (functionOther(adjacentCities.at(j).second))
            {
                res[adjacentCities.at(j).first] = i;
                for (unsigned int k = 0; k < mapOfCities.at(adjacentCities.at(j).first).size(); ++k)
                {
                    next.push_back(mapOfCities.at(adjacentCities.at(j).first)[k]);
                }
            } else {
                continue;
            }
        }
        adjacentCities = next;
    }
    return res;
}

#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
public:
    CTrain                        ( const string    & company,
                                    int               speed )
            : m_Company ( company ),
              m_Speed ( speed )
    {
    }
    //---------------------------------------------------------------------------------------------
    string                   m_Company;
    int                      m_Speed;
};
//=================================================================================================
class TrainFilterCompany
{
public:
    TrainFilterCompany            ( const set<string> & companies )
            : m_Companies ( companies )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain & train ) const
    {
        return m_Companies . find ( train . m_Company ) != m_Companies . end ();
    }
    //---------------------------------------------------------------------------------------------
private:
    set <string>             m_Companies;
};
//=================================================================================================
class TrainFilterSpeed
{
public:
    TrainFilterSpeed              ( int               min,
                                    int               max )
            : m_Min ( min ),
              m_Max ( max )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain    & train ) const
    {
        return train . m_Speed >= m_Min && train . m_Speed <= m_Max;
    }
    //---------------------------------------------------------------------------------------------
private:
    int                      m_Min;
    int                      m_Max;
};
//=================================================================================================
bool                         NurSchnellzug                 ( const CTrain    & zug )
{
    return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
int                          main                          ( void )
{
    map<string,int> test;
    CAccess<string,CTrain> lines;
    lines . Add ( CTrain (   "DB", 120 ), "Berlin", "Prague"   )
            . Add ( CTrain (   "CD",  80 ), "Berlin", "Prague"   )
            . Add ( CTrain (   "DB", 160 ), "Berlin", "Dresden"  )
            . Add ( CTrain (   "DB", 160 ), "Dresden", "Munchen" )
            . Add ( CTrain (   "CD",  90 ), "Munchen", "Prague"  )
            . Add ( CTrain (   "DB", 200 ), "Munchen", "Linz"    )
            . Add ( CTrain (  "OBB",  90 ), "Munchen", "Linz"    )
            . Add ( CTrain (   "CD",  50 ), "Linz", "Prague"     )
            . Add ( CTrain (   "CD", 100 ), "Prague", "Wien"     )
            . Add ( CTrain (  "OBB", 160 ), "Linz", "Wien"       )
            . Add ( CTrain ( "SNCF", 300 ), "Paris", "Marseille" )
            . Add ( CTrain ( "SNCF", 250 ), "Paris", "Dresden"   )
            . Add ( CTrain ( "SNCF", 200 ), "London", "Calais"   );
    assert ( lines . Find ( "Berlin" ) == (map<string,int>
            {
                    make_pair ( "Berlin", 0 ),
                    make_pair ( "Dresden", 1 ),
                    make_pair ( "Linz", 2 ),
                    make_pair ( "Marseille", 3 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Paris", 2 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "London" ) == (map<string,int>
            {
                    make_pair ( "Calais", 1 ),
                    make_pair ( "London", 0 )
            }) );
    assert ( lines . Find ( "Wien" ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Marseille", 5 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Paris", 4 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 0 )
            }) );
    assert ( lines . Find ( "Wien", 3 ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 0 )
            }) );
    assert ( lines . Find ( "Wien", 5, NurSchnellzug ) == (map<string,int>
            {
                    make_pair ( "Berlin", 4 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Prague", 5 ),
                    make_pair ( "Wien", 0 )
            }) );
    assert ( lines . Find ( "Prague", 3,  TrainFilterCompany ( { "CD", "DB" } ) ) == (map<string,int>
            {
                    make_pair ( "Berlin", 1 ),
                    make_pair ( "Dresden", 2 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 1 ),
                    make_pair ( "Prague", 0 ),
                    make_pair ( "Wien", 1 )
            }) );
    assert ( lines . Find ( "Munchen", 4, TrainFilterSpeed ( 160, 250 ) ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 1 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 0 ),
                    make_pair ( "Paris", 2 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "Munchen", 4, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Linz", 2 ),
                    make_pair ( "Munchen", 0 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "London", 20, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
            {
                    make_pair ( "London", 0 )
            }) );
    try
    {
        auto res = lines . Find ( "Salzburg" );
        assert ( "No exception thrown" == NULL );
    }
    catch ( const invalid_argument & e )
    {
        assert ( string ( e . what () ) == "unknown Salzburg" );
    }
#ifdef MULTIPLE_STOPS
    lines . Add ( CTrain ( "RZD",  80 ), "Prague", "Kiev", "Moscow", "Omsk", "Irkutsk", "Vladivostok" );
  lines . Add ( CTrain ( "LAV", 160 ), "Malaga", "Cordoba", "Toledo", "Madrid", "Zaragoza", "Tarragona", "Barcelona", "Montpellier", "Marseille" );
  assert ( lines . Find ( "Madrid" ) == (map<string,int>
  {
    make_pair ( "Barcelona", 1 ),
    make_pair ( "Berlin", 4 ),
    make_pair ( "Cordoba", 1 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 6 ),
    make_pair ( "Kiev", 6 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Madrid", 0 ),
    make_pair ( "Malaga", 1 ),
    make_pair ( "Marseille", 1 ),
    make_pair ( "Montpellier", 1 ),
    make_pair ( "Moscow", 6 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 6 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Tarragona", 1 ),
    make_pair ( "Toledo", 1 ),
    make_pair ( "Vladivostok", 6 ),
    make_pair ( "Wien", 6 ),
    make_pair ( "Zaragoza", 1 )
  }) );
  assert ( lines . Find ( "Omsk", 4 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Moscow", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Omsk", 0 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Moscow", 10, TrainFilterCompany ( { "RZD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Moscow", 0 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 1 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 )
  }) );
#endif /* MULTIPLE_STOPS */
    return 0;
}
#endif  /* __PROGTEST__ */
