/*
 * TypeDefs.h
 *
 *  Created on: 20 Apr 2011
 *      Author: phil
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <string>
#include <memory>
#include <boost/variant.hpp>
#include <boost/cstdint.hpp>



using namespace std;
using namespace boost;

// const char* needed as "text" gets matched to bool not string!!
typedef variant< int, double, int64_t, string, bool, const char* > ValueVariant;
typedef vector< vector<ValueVariant> > ResultVector;
typedef auto_ptr< vector< vector<ValueVariant> > > apResultVector;
typedef auto_ptr< vector<ValueVariant> > apValueVariants;






#endif /* TYPEDEFS_H_ */
