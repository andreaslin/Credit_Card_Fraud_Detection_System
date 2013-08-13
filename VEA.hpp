/***************
 *** VEA.HPP ***
 ***************/

#ifndef _VEA_HPP_
#define _VEA_HPP_

#include "Factor.hpp"
#include <string>

using namespace std;

/* Variable Elimination Algorithm */

class VEA {
 public:
	VEA();					/* Constructor */
	Factor restrict( Factor factor, string variable, string values );
	Factor multiply( Factor factor1, Factor factor2 );
	Factor sumout( Factor factor, string variable );
	Factor normalize( Factor factor );
	Factor inference(list<Factor> factorList, list<string> queryVariables, 
					 list<string> orderedLOHV, list<string> evidenceList, bool normalized );

	list<FactorNode>::iterator find_pos( list<FactorNode> &table, string values );
	string checkValid( list<string> var, FactorNode n1, FactorNode n2 );
	bool exist( Factor factor, string variable );
private:
	list<string>::iterator it;
	
};



#endif
