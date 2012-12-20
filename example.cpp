#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include "property_parser.hpp"
#include "property_visitor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream f1;
    f1.open(argv[1]);
    
    PropertyParser::PropertyList plist1;

    string alltext;
    while (!f1.eof()) {
	string line;
	getline(f1, line);
	alltext += line;
    }

    bool result = false;
    try {
	result = PropertyParser::parse_properties_simple(alltext, plist1);
	//result = PropertyParser::parse_properties(f1, argv[1],  plist1);
    } catch (const exception &err) {
	cout << err.what() << endl;
    }

    PropertyParser::PrintPropertyVisitor pvis;
    pvis(plist1);
}
