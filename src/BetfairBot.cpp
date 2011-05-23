//============================================================================
// Name        : BetfairBot.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : A Betfair Bot Attempt
//============================================================================


#include "SoapAPI/BetFair.nsmap"

#include <iostream>
#include <ctime>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

#include "Event.h"
#include "Market.h"
#include "BetFairProxy.h"
#include "DatabaseProxy.h"

using namespace std;
using namespace boost;
using namespace boost::assign;

int main()
{
	DatabaseProxy db( "/home/phil/dev/c++/workspace/BetfairBot/src/Database/BetFair.db" );
	BetFairProxy bf( db );


	// Get Events
	db.begin();
	db.prepare( Event::insertSql );
	apEvents events( bf.getActiveEvents() );
	BOOST_FOREACH( Event& event, *events )
	{
		//cout << "\n" << event.m_name << " " << event.m_id;
		db.execute( event.inserter() );
	}
	db.finalize();
	db.commit();


	// Request Markets for specific event
	MarketRequest mkReq;
	mkReq.events.push_back( Event( getSingleResult<int>( db.atomicExecute(Event::selectIdSql, "Soccer") ) ) );
	mkReq.countries.push_back("GBR");
	// From now
	time(&mkReq.fromDate);


	apMarkets markets( bf.getAllMarkets( mkReq ) );

	db.begin();
	db.prepare( Market::insertSql );
	BOOST_FOREACH( Market& market, *markets )
	{
		//cout << "\n Adding"; // << market.m_name;
		db.execute( market.inserter() );
	}
	db.finalize();
	db.commit();





	/*

	db.prepare( Event::selectIdSql );
	vector<ValueVariant> vals = list_of( ValueVariant("Cycling") );
	apResultVector result = db.execute( vals );
	BOOST_FOREACH( vector<ValueVariant> row, *result )
	{
		BOOST_FOREACH( ValueVariant value, row )
		{
			cout << "\nResult: " << value;
		}
	}
	db.finalize();

	db.prepare( Event::selectIdSql );
	result = db.execute( "Ice Hockey" );
	BOOST_FOREACH( vector<ValueVariant> row, *result )
	{
		BOOST_FOREACH( ValueVariant value, row )
		{
			cout << "\nResult: " << value;
		}
	}
	db.finalize();

	db.prepare( Event::selectIdSql );
	result = db.execute( Event( "Darts").selectId());
	BOOST_FOREACH( vector<ValueVariant> row, *result )
	{
		BOOST_FOREACH( ValueVariant value, row )
		{
			cout << "\nResult: " << value;
		}
	}
	db.finalize();

	db.prepare( Event::selectNameSql );
	result = db.execute( Event( 26686903 ).selectName());
	BOOST_FOREACH( vector<ValueVariant> row, *result )
	{
		BOOST_FOREACH( ValueVariant value, row )
		{
			cout << "\nResult: " << value;
		}
	}
	db.finalize();

	db.prepare( Event::selectNameSql );
	result = db.execute( 4339 );
	BOOST_FOREACH( vector<ValueVariant> row, *result )
	{
		BOOST_FOREACH( ValueVariant value, row )
		{
			cout << "\nResult: " << value;
		}
	}
	db.finalize();


	*/

	return 0;
}
