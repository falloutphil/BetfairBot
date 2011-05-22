/*
 * Event.h
 *
 *  Created on: 19 Apr 2011
 *      Author: phil
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <memory>
#include <string>
#include <vector>

#include "TypeDefs.h"

struct Event
{
	Event ( int id, const string& name ) : m_id(id), m_name(name) {};
	Event ( int id ) : m_id(id) {};
	Event ( const string& name ) : m_name(name) {};

	static const string insertSql;
	static const string selectIdSql;
	static const string selectNameSql;

	apValueVariants inserter();
	apValueVariants selectId();
	apValueVariants selectName();

	int m_id;
	string m_name;
};

typedef auto_ptr< vector<Event> > apEvents;

#endif /* EVENT_H_ */
