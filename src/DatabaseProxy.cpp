/*
 * DatabaseProxy.cpp
 *
 *  Created on: 18 Apr 2011
 *      Author: phil
 */

#include "DatabaseProxy.h"

#include <iostream>
#include <boost/numeric/conversion/cast.hpp>

using namespace boost;

DatabaseProxy::DatabaseProxy( const string& dbLocation )
: m_stmt(NULL)
{
	if ( sqlite3_initialize() != SQLITE_OK )
	{
		cout << "\nDatabase init failure";
		return;
	}
	login( dbLocation );

}

DatabaseProxy::~DatabaseProxy()
{
	if ( sqlite3_close( m_db ) != SQLITE_OK )
	{
		cout << "\nDatebase close failure!";
	}

	if ( sqlite3_shutdown() != SQLITE_OK )
	{
		cout << "\nDatabase shutdown failure!";
	}
}

bool DatabaseProxy::login( const string& dbLocation )
{
	return sqlite3_open_v2( dbLocation.c_str(),
						    &m_db, SQLITE_OPEN_READWRITE, NULL ) == SQLITE_OK;
}


bool DatabaseProxy::prepare( const string& sql )
{
	return sqlite3_prepare_v2( m_db, sql.c_str(), -1, &m_stmt, NULL ) == SQLITE_OK;
}

apResultVector DatabaseProxy::getResult()
{
	apResultVector result( new ResultVector );
	//cout << "\nGet Rows";
	while ( sqlite3_step( m_stmt ) == SQLITE_ROW )
	{
		vector<ValueVariant> row;
		const int columns = sqlite3_column_count( m_stmt );
		row.reserve( columns );
		for( int col=0; col < columns; ++col )
		{
			//cout << "\ncol=" << col;
			switch ( sqlite3_column_type( m_stmt, col ) )
			{
				case SQLITE_INTEGER:
					// Very gay - as sqlite doesn't differ
					// between 32-bit and 64-bit ints
					// we get as 64-bit, try to downcast
					// if it fails we store the long.
					const long bigInt = sqlite3_column_int64( m_stmt, col );
					try
					{
						row.push_back( numeric_cast<int>(bigInt) );
						//cout << "\nINT";
					}
				    catch(numeric::negative_overflow)
				    {
				    	//cout << "\nLONG";
				    	row.push_back( bigInt );
				    }
				    catch(numeric::positive_overflow)
				    {
				    	//cout << "\nLONG";
				    	row.push_back( bigInt );
				    }
					//cout << "\nAdded";
					break;
				case SQLITE_FLOAT:
					//cout << "\nDOUBLE";
					row.push_back( sqlite3_column_double( m_stmt, col ) );
					break;
				case SQLITE_TEXT:
					//cout << "\nSTRING";
					// The devil's cast - will only work if ASCII is used!
					row.push_back(
							string( reinterpret_cast<const char *>(sqlite3_column_text(m_stmt, col)) )
									);
					break;
				default:
					cout << "\nUnsupport SQLITE Type!";
			}
		}
		//cout << "\nRow push";
		result->push_back( row );
	 }

	return result;

}

apResultVector DatabaseProxy::execute( apValueVariants values )
{
	//cout << "\nExecute";
	cleanStmt();
	int index = values->size();
	apResultVector result;
	while ( index > 0 &&
			apply_visitor( ValueVisitor( index, m_stmt ), (*values)[--index] ) == SQLITE_OK );
	if ( index == 0 )
	{
		result = getResult();
	}
	else
		cout << "\nBinding Failure at index " << index;

	//cout << "\nReturn result";
	return result;
}

apResultVector DatabaseProxy::execute( const vector<ValueVariant>& values )
{
	//cout << "\nExecute";
	cleanStmt();
	int index = values.size();
	apResultVector result;
	while ( index > 0 &&
			apply_visitor( ValueVisitor( index, m_stmt ), values[--index] ) == SQLITE_OK );
	if ( index == 0 )
	{
		result = getResult();
	}
	else
		cout << "\nBinding Failure at index " << index;

	return result;
}

apResultVector DatabaseProxy::execute( const ValueVariant& value )
{
	cleanStmt();
	apResultVector result( new ResultVector );
	if ( apply_visitor( ValueVisitor( 1, m_stmt ), value ) == SQLITE_OK )
	{
		result = getResult();
	}
	else
		cout << "\nBinding Failure at on value: " << value;

	return result;
}

apResultVector DatabaseProxy::execute()
{
	cleanStmt();
	return getResult();
}

apResultVector DatabaseProxy::atomicExecute( const string& sql, const ValueVariant& value )
{
	prepare( sql );
	apResultVector result = execute( value );
	finalize();
	return result;
}

bool DatabaseProxy::finalize()
{
	return sqlite3_finalize( m_stmt ) == SQLITE_OK;
}

bool DatabaseProxy::begin()
{
	return sqlite3_exec(m_db, "BEGIN", 0, 0, 0) == SQLITE_OK;
}

bool DatabaseProxy::commit()
{
	return sqlite3_exec(m_db, "COMMIT", 0, 0, 0) == SQLITE_OK;
}

bool DatabaseProxy::cleanStmt()
{
	return (sqlite3_clear_bindings( m_stmt ) || sqlite3_reset( m_stmt )) == SQLITE_OK;
}



