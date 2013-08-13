/******************
 *** FACTOR.CPP ***
 ******************/

#include <iostream>
#include "Factor.hpp"

using namespace std;

Factor::Factor() {
	variables.resize(0);
	table.resize(0);
}

void Factor::addVariable( string v ) {
	variables.push_back( v );
}

void Factor::addToTable( string v, double pr ) {
	FactorNode n;
	n.pr = pr;
	n.values = v;
	table.push_back( n );
}

void Factor::removeVariable( string v ) {
	variables.remove( v );
}

void Factor::removeFromTable( string v ) {
	for( list<FactorNode>::iterator it = table.begin(); it != table.end(); it++ ) {
		if ( (*it).values.compare( v ) == 0 ) {
			table.erase( it );
		}
	}
}

void Factor::print() {
	// Print variables
	cout << "\t" << "Variables: ";
	for( list<string>::iterator it = variables.begin(); it != variables.end(); it++ ) {
		cout << "\t" << (*it) << " ";
	}
	cout << endl;

	// Print Table
	for( list<FactorNode>::iterator it = table.begin(); it != table.end(); it++ ) {
		cout << "\t" << (*it).values << " :";
		cout << "\t" << (*it).pr << endl;
	}
	cout << endl;
}
