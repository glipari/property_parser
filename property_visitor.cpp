#include <string>
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <gen_parse.hpp>
#include <property_parser.hpp>
#include <property_visitor.hpp>

namespace PropertyParser {

    PrintPropertyVisitor::PrintPropertyVisitor() : indent_level(0) {}

    void PrintPropertyVisitor::operator()(const Property &p) 
    {
        for (unsigned i=0; i<indent_level; ++i) std::cout << "  ";
        std::cout << p.name << "=" << p.value << std::endl;
    }

    void PrintPropertyVisitor::operator()(const PropertyList &pl) 
    {
        for (int i=0; i<indent_level; ++i) std::cout << "  ";
        std::cout << pl.type << "(" << pl.name << ") {" << std::endl;
        indent_level++;
        for (auto x : pl.children) 
            boost::apply_visitor(std::ref(*this), x);
        indent_level--;
        for (int i=0; i<indent_level; ++i) std::cout << "  ";
        std::cout << "}" << std::endl;
    }

    MatchNameVisitor::MatchNameVisitor(const std::string &n) : name(n) {}
    void MatchNameVisitor::operator()(const Property &p) 
    {
	if (p.get_name() == name) {
	    result = true;
	    ptr1 = &p;
	    ptr2 = 0;
	}
	else result = false;
    }

    void MatchNameVisitor::operator()(const PropertyList &p) 
    {
	if (p.get_name() == name) {
	    result = true;
	    ptr1 = 0;
	    ptr2 = &p;
	}
	else result = false;
    }
    
    GenPropertyVisitor::GenPropertyVisitor() : name(""), type("") 
    {
    }

    template <class T>
    void check_and_store_values(std::map<std::string, T> &m, const std::string &n, T v)
    {
        if (m.find(n) != m.end()) 
            THROW_EXC(ValueAlreadySet, "value already set");
        else m[n] = v;
    }

    /** search an array of SVectors, to see if a certain keyword exists */
    template<class SV>
    std::string check_keyword(const std::vector<SV> &v, const std::string &name)
    {
        for (auto const &x : v) {
            std::string k = x.first[0];
            for (auto const &y : x.first)
                if (name ==  y) return k;
        } 
        return "";
    }

    void GenPropertyVisitor::operator()(const Property &p) 
    {
        std::string kd = check_keyword(keywords_double, p.name);
        std::string ki = check_keyword(keywords_int, p.name);
        std::string ks = check_keyword(keywords_string, p.name);
        if (kd != "") {
            if (d_values.find(kd) != d_values.end()) THROW_EXC(ValueAlreadySet, "value already set");
            else d_values[kd] = p.get_double();
        }
        if (ki != "") {
            if (i_values.find(ki) != i_values.end()) THROW_EXC(ValueAlreadySet, "value already set");
            else i_values[ki] = p.get_int();
        }
        if (ks != "") {
            if (s_values.find(ks) != s_values.end()) THROW_EXC(ValueAlreadySet, "value already set");
            else s_values[ks] = p.get_value();
        }
    }

    void GenPropertyVisitor::operator()(const PropertyList &pl) 
    {
        name = pl.name;
        type = pl.type;
        for (auto &x : pl.children) 
            boost::apply_visitor(std::ref(*this), x);            
    } 


    bool GenPropertyVisitor::check_mandatory() const
    {
        // for every mandatory field, there must be a field in d_values or i_values
        for (auto &x : mandatory_keys) {
            bool f = false;
            for (auto &y : d_values) 
                if (x == y.first) f = true;
            for (auto &y : i_values) 
                if (x == y.first) f = true;
            for (auto &y : s_values) 
                if (x == y.first) f = true;
            if (!f) {
                std::cerr << "Mandatory property " << x << " not set in " 
                          << type << "(" << name << ")" << std::endl;
                return false;
            }
        }
        return true;
    }
   
    void GenPropertyVisitor::add_double_parameter(const SVector &v, double x, bool mandatory) 
    {
        keywords_double.push_back(make_pair(v, x));
        if (mandatory) mandatory_keys.push_back(v[0]);
    }

    void GenPropertyVisitor::add_int_parameter(const SVector &v, int x, bool mandatory) 
    {
        keywords_int.push_back(make_pair(v, x));
        if (mandatory) mandatory_keys.push_back(v[0]);
    }

    void GenPropertyVisitor::add_string_parameter(const SVector &v, std::string x, bool mandatory) 
    {
        keywords_string.push_back(make_pair(v, x));
        if (mandatory) mandatory_keys.push_back(v[0]);
    }
}
