/***************
 *** VEA.CPP ***
 ***************/

#include <iostream>
#include "VEA.hpp"

using namespace std;

VEA::VEA() {}

Factor VEA::restrict( Factor factor, string variable, string values ) {
	// Check if variable is in the factor provided
	bool doesExist = exist( factor, variable );
	// If variable not found, return original factor
	if ( !doesExist ) return factor;
		
	Factor newFactor;
	newFactor.variables = factor.variables;
	newFactor.removeVariable( variable );
	newFactor.table = factor.table;

	unsigned int size = newFactor.table.size();

	for( unsigned int i = 0; i < size; i++ ) {
		list<FactorNode>::iterator it = newFactor.table.begin();
		size_t found = (*it).values.find( values );
		if ( found != string::npos ) { 	// Found the value
			FactorNode temp = (*it);
			newFactor.table.erase( it );
			temp.values = temp.values.erase( found, values.size() );
			newFactor.table.push_back( temp );
		} else {
			newFactor.table.erase( it );
		}
	}

	return newFactor;
}

Factor VEA::multiply( Factor factor1, Factor factor2 ) {
	Factor newFactor;
	FactorNode temp;
	double pr;
	bool basecase = false;

	// If the other factor doens't contain anything
	if ( factor1.variables.size() == 0 ) {
		if ( factor1.table.size() == 0 ) {
			return factor2;
		} else {
			basecase = true;
			newFactor = factor2;
			pr = factor1.table.front().pr;
		}
	}
	if ( factor2.variables.size() == 0 ) {
		if ( factor2.table.size() == 0 ) {
			return factor1;
		} else {
			basecase = true;
			newFactor = factor1;
			pr = factor2.table.front().pr;
		}
	}

	// Base case, multiply by the constant
	if ( basecase ) {
		for( list<FactorNode>::iterator it = newFactor.table.begin(); it != newFactor.table.end(); it++ ) {
			(*it).pr = (*it).pr * pr;
		}
		return newFactor;
	}

	// Append factor1 and factor2 variables 
	newFactor.variables = factor1.variables;
	for ( list<string>::iterator it = factor2.variables.begin(); it != factor2.variables.end(); it++ ) {
		newFactor.variables.push_back( (*it) );
	}
	newFactor.variables.sort();
	newFactor.variables.unique(); // Remove duplicates
	
	for( list<FactorNode>::iterator it1 = factor1.table.begin(); it1 != factor1.table.end(); it1++ ) {
		for ( list<FactorNode>::iterator it2 = factor2.table.begin(); it2 != factor2.table.end(); it2++ ) {
			string temp_val = checkValid( factor1.variables, (*it1), (*it2) );
			if ( temp_val.size() > 0 ) {
				temp.values = temp_val;
				temp.pr = (*it1).pr * (*it2).pr;
				newFactor.table.push_back( temp );
			}
		}
	}
	
	return newFactor;
}

Factor VEA::sumout( Factor factor, string variable ) {
	Factor newFactor;
	newFactor.variables = factor.variables;
	newFactor.removeVariable( variable );
	newFactor.table = factor.table;

	string positive = "+";
	string negative = "~";

	positive += variable;
	negative += variable;

	unsigned int size = newFactor.table.size();
	FactorNode temp;

	for( unsigned int i = 0; i < size; i += 1 ) {
		list<FactorNode>::iterator it = newFactor.table.begin();
		size_t found_pos = (*it).values.find( positive );
		size_t found_neg = (*it).values.find( negative );

		if ( found_pos != string::npos ) { 					// Found positive variables
			temp.pr = (*it).pr;						// Update the probability
			temp.values = (*it).values.erase( found_pos, positive.size() );	// Update the value
			newFactor.table.erase( it );
			string neg = temp.values.insert( found_pos, negative ); 	// Find the other one with negative
			it = find_pos( newFactor.table, neg );
			if ( it != newFactor.table.end() ) {
				temp.pr += (*it).pr;
				temp.values.erase( found_pos, negative.size() );
				newFactor.table.erase( it );
				newFactor.table.push_back( temp );
			}
		} else if ( found_neg != string::npos ) { 				// Found negative variables
			temp.pr = (*it).pr;						// Update the probability
			temp.values = (*it).values.erase( found_neg, negative.size() );	// Update the value
			newFactor.table.erase( it );
			string pos = temp.values.insert( found_neg, positive ); 	// Find the other one with negative
			//			cout << pos << endl;
			it = find_pos( newFactor.table, pos );
			if ( it != newFactor.table.end() ) {
				temp.pr += (*it).pr;
				temp.values.erase( found_neg, positive.size() );
				newFactor.table.erase( it );			
				newFactor.table.push_back( temp );
			}
		} else {
			temp = newFactor.table.front();
			newFactor.table.pop_front();
			newFactor.table.push_back( temp );
		}
		
	}
	return newFactor;
}

Factor VEA::normalize( Factor factor ) {
	Factor newFactor;
	FactorNode temp;
	newFactor.variables = factor.variables;

	double sum = 0;
	list<FactorNode>::iterator it;

	// Sum up all entries
	for( it = factor.table.begin(); it != factor.table.end(); it++ ) {
		sum += (*it).pr;
	}

	// Calculate new probability
	for( it = factor.table.begin(); it != factor.table.end(); it++ ) {
		temp.pr = (*it).pr / sum;
		temp.values = (*it).values;
		newFactor.table.push_back( temp );
	}

	return newFactor;

}

Factor VEA::inference( list<Factor> factorList, list<string> queryVariables, 
					   list<string> orderedLOHV, list<string> evidenceList, bool normalized ) {
	list<Factor> newFactors;
	Factor newFactor;
 
	// Go through each factor
	for( list<Factor>::iterator it2 = factorList.begin(); it2 != factorList.end(); it2++ ) {
		Factor temp = (*it2);
		for( list<string>::iterator it1 = evidenceList.begin(); it1 != evidenceList.end(); it1++ ) {
			// Find factor with the evidence values
			string value = (*it1);
			string variable = value.substr( 1, value.length() );
			temp = restrict( temp, variable, value );
		}
		// Add to restrict new factor list
		newFactors.push_back( temp );
	}

#ifdef DEBUG
	cout << "Restricted given factors:" << endl;
	for( list<Factor>::iterator it = newFactors.begin(); it != newFactors.end(); it++ ) {
		(*it).print();
	}
#endif	

	// This factor list is for the multiply when other factors have been sum out
	list<Factor> anotherNewFactor;
	for( list<Factor>::iterator it1 = newFactors.begin(); it1 != newFactors.end(); it1++ ) {
		bool add = true;
		Factor f = (*it1);
		for( list<string>::iterator it2 = orderedLOHV.begin(); it2 != orderedLOHV.end(); it2++ ) {
			string variable = (*it2);
			if ( exist( f, variable ) ) {
				add = false;
				break;
			}
		}
		if ( add ) anotherNewFactor.push_back( f );
	}


	// Eliminate hidden variables
	for( list<string>::iterator it1 = orderedLOHV.begin(); it1 != orderedLOHV.end(); it1++ ) {
		Factor temp;
		for ( list<Factor>::iterator it2 = newFactors.begin(); it2 != newFactors.end(); it2++ ) {
			string variable = (*it1);
			Factor f = (*it2);
			if ( exist ( f, variable ) ) temp = multiply( temp, f );
		}
		// Sumout the factor with all hidden variable
		for ( list<string>::iterator it3 = orderedLOHV.begin(); it3 != orderedLOHV.end(); it3++ ) {
			if ( exist( temp, (*it3) ) ) temp = sumout( temp, (*it3) );
		}
		cout << "New Factor: " << endl;
		temp.print();
		// Multiply to new factor
		newFactor = multiply( newFactor, temp );
	}

	// Multiply the rest of the factors
	cout << "New Factors multiply the rest of the factors: " << endl;
	for( list<Factor>::iterator it = anotherNewFactor.begin(); it != anotherNewFactor.end(); it++ ) {
		(*it).print();
		newFactor = multiply( newFactor, (*it) );
	}

	// Normalize?
	if ( normalized ) newFactor = normalize( newFactor );

	return newFactor;
}

list<FactorNode>::iterator VEA::find_pos( list<FactorNode> &table, string values ) {
	for( list<FactorNode>::iterator it = table.begin(); it != table.end(); it++ ) {
		if ( (*it).values.compare( values ) == 0 ) {
			return it;
		}
	}
	
	return table.end();
}

string VEA::checkValid( list<string> var, FactorNode n1, FactorNode n2 ) {
	string r = "";
	for( list<string>::iterator it = var.begin(); it != var.end(); it++ ) {
		size_t found1 = n1.values.find( (*it) );
		size_t found2 = n2.values.find( (*it) );
		if ( found1 != string::npos && found2 != string::npos &&	
			 ( n1.values[found1 - 1] == n2.values[found2 - 1] ) ) { 
			// Common variables and same sign
			n2.values.erase( found2 - 1, (*it).size() + 1 ); 	// Remove the common var from second value
		} else if ( found2 == string::npos ) {				// The variable DNE in the second value
			// Do nothing to second value
		} else {							// Contradicting variables, abort
			return r;
		}
	}

	r = n1.values.append( n2.values );
	return r;
}

bool VEA::exist( Factor factor, string variable ) {
	for( list<string>::iterator it = factor.variables.begin(); it != factor.variables.end(); it++ ) {
		if ( (*it).compare( variable ) == 0 ) return true;
	}	
	return false;
}

