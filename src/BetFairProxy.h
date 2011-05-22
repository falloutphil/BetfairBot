/*
 * GlobalServiceProxy.h
 *
 *  Created on: 17 Apr 2011
 *      Author: phil
 */

#ifndef BETFAIRPROXY_H_
#define BETFAIRPROXY_H_

#include "SoapAPI/soapBFGlobalServiceProxy.h"
#include "SoapAPI/soapBFExchangeServiceProxy.h"

#include "Event.h"
#include "Market.h"

#include <memory>

#include <boost/xpressive/xpressive.hpp>

using namespace std;
using namespace boost::xpressive;

class BetFairProxy
{
	public:
		BetFairProxy( const string& username, const string& password );
		~BetFairProxy();
		apEvents getActiveEvents();
		apMarkets getAllMarkets( MarketRequest& marketRequest );
	private:
		BFGlobalServiceProxy globalProxy;
		BFExchangeServiceProxy exchangeProxy;
		string sessionToken;

		static const sregex tilde;
		static const sregex colon;

		template <typename REQ, typename INTERNAL, typename HDR>
		void createRequest( REQ& req, INTERNAL& internalReq, HDR& hdr )
		{
			hdr.sessionToken = &sessionToken;
			internalReq.header = &hdr;
			req.request = &internalReq;
		}

		template <typename RESP>
		void storeNewToken( const RESP& resp )
		{
			sessionToken = *resp.Result->header->sessionToken;
		}
};

#endif /* BETFAIRPROXY_H_ */
