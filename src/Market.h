/*
 * Market.h
 *
 *  Created on: 15 May 2011
 *      Author: phil
 */

#ifndef MARKET_H_
#define MARKET_H_

#include <memory>
#include <string>
#include <vector>
#include <ctime>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/cstdint.hpp>

#include "TypeDefs.h"
#include "Event.h"

using namespace boost::posix_time;
using namespace boost;

struct Market
{
	public:

		Market ( 	int32_t  		id,
					const string& 	name,
					const string& 	type,
					const string& 	status,
					ptime 			eventDate,
					const string& 	menuPath,
					const string&	eventHeirachy,
					const string&	betDelay,
					int32_t			exchangeId,
					const string&	countryCode,
					ptime			lastRefresh,
					int32_t			numOfRunners,
					int32_t			numOfWinners,
					double			totalMatched,
					bool			isBsp,
					bool			isTurningInPlay );

		apValueVariants inserter();
		static const string insertSql;

	private:
		int32_t m_id;
		string 	m_name;
		string 	m_type;
		string 	m_status;
		ptime 	m_eventDate;
		string 	m_menuPath;
		string	m_eventHeirachy;
		string	m_betDelay;
		int32_t	m_exchangeId;
		string	m_countryCode;
		ptime	m_lastRefresh;
		int32_t	m_numOfRunners;
		int32_t	m_numOfWinners;
		double	m_totalMatched;
		bool	m_isBsp;
		bool	m_isTurningInPlay;


};

typedef auto_ptr< vector<Market> > apMarkets;

struct MarketRequest
{
	MarketRequest() : fromDate(-1), toDate(-1) {};
	string locale;
	vector<Event> events;
	vector<string> countries;
	time_t fromDate;
	time_t toDate;
};

#endif /* MARKET_H_ */
