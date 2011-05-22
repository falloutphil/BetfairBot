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
#include <boost/shared_ptr.hpp>
#include "boost/variant.hpp"



using namespace std;
using namespace boost;

typedef variant< int, double, long, string, bool > ValueVariant;
typedef vector< vector<ValueVariant> > ResultVector;
typedef auto_ptr< vector< vector<ValueVariant> > > apResultVector;
typedef auto_ptr< vector<ValueVariant> > apValueVariants;






#endif /* TYPEDEFS_H_ */
