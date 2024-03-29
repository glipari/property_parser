#ifndef __PROPERTY_VISITOR_HPP__
#define __PROPERTY_VISITOR_HPP__

#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <property.hpp>
#include <property_parser.hpp>

namespace PropertyParser {

    DECL_EXC(ValueAlreadySet);
    DECL_EXC(IncorrectType);

    struct PrintPropertyVisitor : public boost::static_visitor<> {
        int indent_level;
	std::ostream &out;
        PrintPropertyVisitor(std::ostream &o);
        void operator()(const Property &p);
        void operator()(const PropertyList &pl);
    };

    struct MatchNameVisitor : public boost::static_visitor<> {
	std::string name;
	bool result;
	const Property *ptr1;
	const PropertyList *ptr2;
	MatchNameVisitor(const std::string &n);
        void operator()(const Property &p);
	void operator()(const PropertyList &p);
    };


    class GenPropertyVisitor : public boost::static_visitor<> {
    protected:
        std::map<std::string, double> d_values;
        std::map<std::string, int> i_values;
        std::map<std::string, std::string> s_values;

        typedef std::vector< std::string > SVector;
        typedef std::pair< SVector, double > SVDouble; 
        typedef std::pair< SVector, int > SVInt; 
        typedef std::pair< SVector, std::string > SVString; 
        
        std::vector< std::string > mandatory_keys;
        std::vector< std::pair< SVector, double> > keywords_double;
        std::vector< std::pair< SVector, int> > keywords_int;
        std::vector< std::pair< SVector, std::string> > keywords_string;

        std::string name;
        std::string type;
        
    public:
        GenPropertyVisitor();

        void add_double_parameter(const SVector &v, double x, bool mandatory = false);
        void add_int_parameter(const SVector &v, int x, bool mandatory = false);

        void add_string_parameter(const SVector &v, std::string x, bool mandatory = false);

        // parses a property
        void operator()(const Property &p);
        // the user has to derive and write an operator to read a PropertyList
        void operator()(const PropertyList &pl); 
        
        bool check_mandatory() const;
    };
}

#endif
