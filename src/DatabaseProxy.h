/*
 * DatabaseProxy.h
 *
 *  Created on: 18 Apr 2011
 *      Author: phil
 */

#ifndef DATABASEPROXY_H_
#define DATABASEPROXY_H_

#include "Database/sqlite3.h"

#include <string>
#include <vector>
#include <memory>

#include <boost/variant.hpp>
#include <boost/cstdint.hpp>

#include "TypeDefs.h"


using namespace std;
using namespace boost;



class DatabaseProxy
{
	public:
		DatabaseProxy( const string& dbLocation );
		~DatabaseProxy();
		bool login( const string& dbLocation );
		bool prepare( const string& sql );
		apResultVector execute( apValueVariants values );
		apResultVector execute( const vector<ValueVariant>& values );
		apResultVector execute( const ValueVariant& value );
		apResultVector execute();
		apResultVector atomicExecute( const string& sql, const ValueVariant& value );
		bool finalize();
		bool begin();
		bool commit();


	private:
		sqlite3* m_db;
		sqlite3_stmt* m_stmt;

		bool cleanStmt();
		apResultVector getResult();

		class ValueVisitor : public static_visitor<int32_t>
		{
			public:
				explicit ValueVisitor( int32_t index, sqlite3_stmt* stmt )
						 : m_index( ++index ), m_stmt( stmt ) {};

		    	int32_t operator()( int32_t value ) const
		    	{
		    		//cerr << "\nInt Binding: " << value << " to " << m_index;
		    		return sqlite3_bind_int( m_stmt, m_index, value );
		    	}

		    	int32_t operator()( double value ) const
		    	{
		    		//cerr << "\nDouble Binding: " << value << " to " << m_index;
		    		return sqlite3_bind_double( m_stmt, m_index, value );
		    	}

		    	int32_t operator()( int64_t value ) const
		    	{
		    		//cerr << "\nLong Binding: " << value << " to " << m_index;
		    		return sqlite3_bind_int64( m_stmt, m_index, value );
		    	}

		    	int32_t operator()( const string& value ) const
		    	{
		    		//cerr << "\nString Binding: " << value << " to " << m_index;
		    		return sqlite3_bind_text( m_stmt, m_index, value.c_str(), -1, NULL );
		    	}

		    	int32_t operator()( const char* value ) const
		    	{
		    		return sqlite3_bind_text( m_stmt, m_index, value, -1, NULL );
		    	}

		    	int32_t operator()( bool value ) const
		    	{
		    		//cerr << "\nBool Binding: " << value << " to " << m_index;
		    		return sqlite3_bind_int( m_stmt, m_index, value ? 1 : 0 );
		    	}



			private:
		    	const int32_t m_index;
		    	sqlite3_stmt* m_stmt;
		};

};

//Helper Functions

template <typename RESULT>
RESULT getSingleResult( apResultVector resultVector, int32_t col = 0, int32_t row = 0 )
{
	return get<RESULT>( (*resultVector)[row][col] );
}

#endif /* DATABASEPROXY_H_ */
