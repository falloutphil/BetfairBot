/*
 * GlobalServiceProxy.cpp
 *
 *  Created on: 17 Apr 2011
 *      Author: phil
 */

#include "BetFairProxy.h"
#include "Event.h"
#include "Market.h"
#include "DatabaseProxy.h"

#include <boost/foreach.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/cstdint.hpp>

#include <memory>

#include <gnome-keyring-1/gnome-keyring.h>

using namespace boost;
using namespace boost::xpressive;
using namespace boost::posix_time;


const sregex BetFairProxy::tilde = ~after('\\') >> '~';
const sregex BetFairProxy::colon = ~after('\\') >> ':';

// Database and Keyring authentication
const string BetFairProxy::selectKeyringValueSql = "SELECT Value FROM Keyring WHERE Attribute = ?";
const string BetFairProxy::user   = "user";
const string BetFairProxy::server = "server";

BetFairProxy::BetFairProxy( DatabaseProxy& db )
{
	const string userValue   = getSingleResult<string>( db.atomicExecute(selectKeyringValueSql, user) );
	const string serverValue = getSingleResult<string>( db.atomicExecute(selectKeyringValueSql, server) );
	char* password;

	if ( gnome_keyring_find_password_sync (GNOME_KEYRING_NETWORK_PASSWORD,  /* The password type */
								  	  	   &password,
								  	  	   /* These are the attributes - we search on these not the Key name*/
								  	  	   user.c_str(),   userValue.c_str(),
								  	  	   server.c_str(), serverValue.c_str(),
								  	  	   NULL) /* Always end with NULL */ != GNOME_KEYRING_RESULT_OK )
	{
		cerr << "ERROR: Betfair password not in Gnome keychain!";
		exit(1);
	}

	ns1__LoginReq loginReq;
	loginReq.username = userValue;
	loginReq.password = password;
	loginReq.productId = 82;
	loginReq.ipAddress = "0";
	loginReq.locationId = 0;
	loginReq.vendorSoftwareId = 0;
	_ns2__login login;
	_ns2__loginResponse response;
	login.request = &loginReq;
	cout << "\nProxy call..";
	if ( globalProxy.login( &login, &response ) == SOAP_OK )
	{
		storeNewToken( response );
		cout << "\n" << sessionToken << endl;
	}
	else
	{
		globalProxy.soap_stream_fault(cerr);
	}


}

BetFairProxy::~BetFairProxy()
{
	// Proxy logout required
}


apEvents BetFairProxy::getActiveEvents()
{
	cout << "\ngetActiveEvents" << endl;
	_ns2__getActiveEventTypes req;
	ns1__APIRequestHeader hdr;
	ns1__GetEventTypesReq internalReq;
	_ns2__getActiveEventTypesResponse resp;

	createRequest( req, internalReq, hdr );

	apEvents pEvents( new vector<Event> );
	if ( globalProxy.getActiveEventTypes( &req, &resp ) == SOAP_OK )
	{
		cout << "\nOK" << endl;
		storeNewToken( resp );
		std::vector<ns1__EventType*>& events = resp.Result->eventTypeItems->ns1__EventType_;
		cout << events.size();

		BOOST_FOREACH( ns1__EventType* event, events )
		{
			pEvents->push_back( Event( event->id, *event->name ) );
		}

	}
	else cout << "\nEND" << endl;
	return pEvents;
}

apMarkets BetFairProxy::getAllMarkets( MarketRequest& marketRequest )
{
	cout << "\ngetAllMarkets" << endl;

	_ns4__getAllMarkets req;
	ns3__APIRequestHeader hdr;
	ns3__GetAllMarketsReq internalReq;
	_ns4__getAllMarketsResponse resp;

	createRequest( req, internalReq, hdr );

	//cerr << "\nStart";
	internalReq.locale = marketRequest.locale.empty() ? NULL :  &marketRequest.locale;
	//cerr << "\nStart1";
	ns3__ArrayOfInt eventIdArray;
	BOOST_FOREACH( Event& event, marketRequest.events )
	{
		eventIdArray.ns3__int.push_back( event.m_id );
	}
	internalReq.eventTypeIds = &eventIdArray;
	//cerr << "\nStart2";
	ns3__ArrayOfCountryCode countryArray;
	if (!marketRequest.countries.empty())
	{
		countryArray.ns3__Country.assign( marketRequest.countries.begin(), marketRequest.countries.end() );
		internalReq.countries = &countryArray;
	}
	//cerr << "\nStart3";
	if (marketRequest.fromDate != -1)
	{
		//cerr << "\nAdding time: " << marketRequest.fromDate;
		internalReq.fromDate = &marketRequest.fromDate;
	}
	//cerr << "\nStart4";
	if (marketRequest.toDate != -1)
		internalReq.toDate = &marketRequest.toDate;
	req.request = &internalReq;
	//cerr << "\nEnd";
	apMarkets pMarkets( new vector<Market> );

	if ( exchangeProxy.getAllMarkets( &req, &resp ) == SOAP_OK )
	{
		storeNewToken( resp );
		cout << "\nOK" << endl;
		const string& respString = *(resp.Result->marketData);

	    // the -1 below directs the token iterator to display the parts of
	    // the string that did NOT match the regular expression.
	    sregex_token_iterator marketCur( respString.begin(), respString.end(), colon, -1 );
	    sregex_token_iterator end;

	    for( ; marketCur != end; ++marketCur )
	    {
	        const string& market = *marketCur;
	    	if (!market.empty())
	    	{
	    		sregex_token_iterator cur( market.begin(), market.end(), tilde, -1 );
	    		pMarkets->push_back( Market(
	    				lexical_cast<int>(*cur),
	    				*(++cur),
	    				*(++cur),
	    				*(++cur),
	    				from_time_t( lexical_cast<int64_t>(*(++cur)) / 1000 ),
	    				*(++cur),
	    				*(++cur),
	    				*(++cur),
	    				lexical_cast<int>(*(++cur)),
	    				*(++cur),
	    				from_time_t( lexical_cast<int64_t>(*(++cur)) / 1000 ),
	    				lexical_cast<int>(*(++cur)),
	    				lexical_cast<int>(*(++cur)),
	    				lexical_cast<double>(*(++cur)),
	    				(*(++cur)) == "Y",
	    				(*(++cur)) == "Y" ) );
	    	}
	    }


		//cout << "\n\n\nResponse: " << respString;


	}
	else cerr << "\nNOT OK" << endl;

	return pMarkets;


}





// Utility function if you need to re-add password to Gnome Keychain.
void save_my_password()
{
    cerr << "Trying ";
	cerr << gnome_keyring_store_password_sync (GNOME_KEYRING_NETWORK_PASSWORD, /* The password type */
                                      	       GNOME_KEYRING_DEFAULT,          /* Where to save it */
                                      	       "Betfair",       /* Password description, displayed to user */
                                      	       "*******",                 /* The password itself */
                                      	       /* These are the attributes */
                                      	       "user", "betfair username",
                                      	       "server", "betfair.com",
                                      	       NULL); /* Always end with NULL */
}
