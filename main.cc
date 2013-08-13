/***************
 *** MAIN.CC ***
 ***************/

#include <iostream>
#include <fstream>
#include <cstdlib>				// exit
#include <cmath>				// pow
#include <cctype>				// isspace, isdigit
#include <map>					// map
#include "VEA.hpp"

#define BUFFERSIZE 512

using namespace std;

void printStringList( list<string> l ) {
	for( list<string>::iterator it = l.begin(); it != l.end(); it++ ) {
		cout << "\t" << *it << endl;
	}
	cout << endl;
}

void printAll( list<string> l1, list<string> l2, list<string> l3 ) {
	cout << "Query Variables:" << endl;
	printStringList( l1 );
	cout << "Hidden Variables:" << endl;
	printStringList( l2 );
	cout << "Evidence List:" << endl;
	printStringList( l3 );	
}

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] << " input-file" << endl;
    exit( EXIT_FAILURE );		// TERMINATE
} // usage

int main( int argc, char *argv[] ) {
    istream *infile;					// default value
	list<Factor> factors;				// List of factor
	VEA vea;					// Variable elimination algorithm

	switch ( argc ) {
	case 2:
        infile = new ifstream( argv[1] );
        if ( infile->fail() ) {		// open failed ?
			cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
			usage( argv );
		} // if
		break;
	default:					// wrong number of options
		usage( argv );
	} // switch*/

	FactorNode temp;
	Factor f_temp;
	char input[BUFFERSIZE];				// Buffer for input 
	char c;						// Single character
	int a;
	string str_temp = "";				// Temporary for string
	list<string> var;
	int id = 0;					// Factor id 

	while( true ) {
		// Parse the first factor line
		*infile >> a;
		if ( infile->fail() ) break;

		var.clear();

		// Flush the first whitespace
		*infile >> noskipws;			// turn off white space skipping during input
		*infile >> c;

		for( unsigned int j = 0; j < a; j += 1 ) { 
			str_temp.clear();
			while( true ) {
				*infile >> c;
				if ( !isalpha(c) ) break;
				str_temp.append( 1, c );
			}
			// One variable parsed, add to variable list
			var.push_back( str_temp );
		}

		// Finish parsing variables, add to factor
		f_temp.variables = var;
		f_temp.table.clear();
		
		*infile >> skipws;			// Start skipping whitespace again
		// Start parsing values
		int size = pow( 2, a );
		for( unsigned int j = 0; j < size; j += 1 ) {
			str_temp.clear();
			// Parse value
			while( true ) {
				*infile >> c;
				if ( isdigit(c) ) break;
				str_temp.append( 1, c );
			}
			temp.values = str_temp;

			// Parse probability
			*infile >> input;
			temp.pr = atof( input );
			
			// Push into factor
			f_temp.table.push_back( temp );
		}
		// Push into factor list
		//		factors.insert( pair<unsigned int, Factor>( id, f_temp) );
		factors.push_back( f_temp );
		id += 1;
	}

	// Print factors
	/*	for( unsigned int i = 0; i < factors.size(); i += 1 ) {
		factors[i].print();
		cout << endl;
		}*/
#ifdef DEBUG1
	for( list<Factor>::iterator it = factors.begin(); it != factors.end(); it++ ) {
		(*it).print();
	}
#endif

	list<string> qv;
	qv.push_back("+FRAUD");
	list<string> hv;
	hv.push_back("TRAV");
	hv.push_back("OC");
	list<string> el;
	el.push_back("+FP");
	el.push_back("~IP");
	el.push_back("+CRP");

	cout << "=================For Question 2b and 2c================" << endl;

	// Pr( FRAUD | +FP, ~IP, +CRP )
	cout << "Pr( FRAUD | +FP, ~IP, +CRP )" << endl;
	printAll( qv, hv, el );
	Factor factor4 = vea.inference( factors, qv, hv, el, true );
	cout << "Result:" << endl;
	factor4.print();

	cout << "----------------------------------" << endl;

	// Pr( FRAUD | +FP, ~IP, +CRP, +TRAV )	
	hv.clear();
	hv.push_back("OC");
	el.push_back("+TRAV");
	cout << "Pr( FRAUD | +FP, ~IP, +CRP, +TRAV )" << endl;
	printAll( qv, hv, el );
	Factor factor5 = vea.inference( factors, qv, hv, el, true );
	cout << "Result:" << endl;
	factor5.print();
		
	cout << endl;
	cout << "=================For Question 3b and 3c=================" << endl;
	
	// Pr( TRAV | +FP, ~IP, +CRP )
	qv.clear();
	qv.push_back( "+TRAV" );
	hv.clear();
	hv.push_back( "OC" );
	hv.push_back( "FRAUD" );
	el.clear();
	el.push_back("+FP");
	el.push_back("~IP");
	el.push_back("+CRP");	
	cout << "Pr( TRAV | +FP, ~IP, +CRP )" << endl;
	printAll( qv, hv, el );
	Factor factor6 = vea.inference( factors, qv, hv, el, true );
	cout << "Result:" << endl;
	factor6.print();

	cout << "----------------------------------" << endl;
	
	// Pr( FRAUD | +FP, ~IP, +CRP, ~TRAV )
	qv.clear();
	qv.push_back( "+FRAUD" );
	hv.clear();
	hv.push_back( "OC" );
	el.push_back( "~TRAV" );
	cout << "Pr( FRAUD | +FP, ~IP, +CRP, ~TRAV )" << endl;
	printAll( qv, hv, el );
	Factor factor7 = vea.inference( factors, qv, hv, el, true );
	cout << "Result:" << endl;
	factor7.print();


    if ( infile != &cin ) delete infile;		// close file, do not delete cin!	
} // main
