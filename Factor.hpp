/******************
 *** FACTOR.HPP ***
 ******************/

#ifndef _FACTOR_HPP_
#define _FACTOR_HPP_

#include <string>
#include <list>
#include <map>

using namespace std;

/* Factor Class */

class Factor {	
 public:
	// NOTE: True value will have a + sign and false will have a ~ sign
	struct FactorNode {
		double pr;
		string values;
	};
	
	list<string> variables;
	list<FactorNode> table;

	/* Public constructor */
	Factor();

	void addVariable( string v );
	void addToTable( string v, double pr );
	void removeVariable( string v );
	void removeFromTable( string v );
	void print();
	
};

typedef struct Factor::FactorNode FactorNode;

#endif
