/*
 * Event.cpp
 *
 *  Created on: 19 Apr 2011
 *      Author: phil
 */

#include "Event.h"

const string Event::insertSql     = "INSERT INTO Events VALUES ( ?, ? )";
const string Event::selectIdSql   = "SELECT id FROM Events WHERE name = ?";
const string Event::selectNameSql = "SELECT name FROM Events WHERE id = ?";

apValueVariants Event::inserter()
{
	apValueVariants myValues( new vector<ValueVariant> );
	myValues->push_back( m_id );
	myValues->push_back( m_name );
	return myValues;
}

apValueVariants Event::selectId()
{
	apValueVariants myValues( new vector<ValueVariant> );
	myValues->push_back( m_name );
	return myValues;
}

apValueVariants Event::selectName()
{
	apValueVariants myValues( new vector<ValueVariant> );
	myValues->push_back( m_id );
	return myValues;
}


