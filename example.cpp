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

    bool result = false;
    try {
	result = PropertyParser::parse_properties(f1, string(argv[1]), plist1);
    } catch (const exception &err) {
	cout << err.what() << endl;
    }
    cout << "Result = " << result << endl;
    
    PropertyParser::PrintPropertyVisitor pvis(cout);
    pvis(plist1);
}
