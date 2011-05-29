/*
 * Market.cpp
 *
 *  Created on: 15 May 2011
 *      Author: phil
 */

#include "Market.h"
#include "TypeDefs.h"

#include <boost/assign/std/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/cstdint.hpp>

using namespace boost::assign;
using namespace boost::posix_time;
using namespace boost;

const string Market::insertSql = "INSERT INTO Markets VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )";

Market::Market ( 	int32_t 		id,
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
					bool			isTurningInPlay ) :
m_id(id),
m_name(name),
m_type(type),
m_status(status),
m_eventDate(eventDate),
m_menuPath(menuPath),
m_eventHeirachy(eventHeirachy),
m_betDelay(betDelay),
m_exchangeId(exchangeId),
m_countryCode(countryCode),
m_lastRefresh(lastRefresh),
m_numOfRunners(numOfRunners),
m_numOfWinners(numOfWinners),
m_totalMatched(totalMatched),
m_isBsp(isBsp),
m_isTurningInPlay(isTurningInPlay)
{
/*
	cout << endl << "id: " 				<< m_id
		 <<	endl << "name: " 			<< m_name
		 << endl << "type: " 			<< m_type
		 << endl << "status: " 			<< m_status
		 << endl << "event d: " 		<< m_eventDate
		 << endl << "menu path: " 		<< m_menuPath
		 << endl << "event heir: " 		<< m_eventHeirachy
		 << endl << "bet delay: " 		<< m_betDelay
		 << endl << "exchange id: " 	<< m_exchangeId
		 << endl << "cntry code: " 		<< m_countryCode
		 << endl << "last rfrsh: " 		<< m_lastRefresh
		 << endl << "num runners: "		<< m_numOfRunners
		 << endl << "num winnders: "	<< m_numOfWinners
		 << endl << "tot matched: " 	<< m_totalMatched
		 << endl << "is bsp: " 			<< m_isBsp
		 << endl << "is turning: " 		<< m_isTurningInPlay;
*/
}


apValueVariants Market::inserter()
{
	apValueVariants myValues( new vector<ValueVariant> );

	*myValues += m_id, m_name, m_type, m_status, to_iso_string(m_eventDate), m_menuPath, m_eventHeirachy,
				 m_betDelay, m_exchangeId, m_countryCode, to_iso_string(m_lastRefresh), m_numOfRunners,
				 m_numOfWinners, m_totalMatched, m_isBsp, m_isTurningInPlay;

	return myValues;
}
