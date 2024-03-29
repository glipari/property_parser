#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include <exceptions.hpp>

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>

namespace PropertyParser {

    DECL_EXC(PropertyNotFound);
    DECL_EXC(PropertyWrongType);

    struct Property { 
        std::string name; 
        std::string value;

        Property() {}
        Property(const std::string &n, const std::string &v = "") :
            name(n), value(v) {}       
	
        std::string get_value() const { return value; }
        void set_value(const std::string &v) { value = v; }
        std::string get_name() const { return name; }
        int get_int() const { return std::atoi(get_value().c_str()); } 
        double get_double() const { return std::atof(get_value().c_str()); } 
    };
    
    struct PropertyList { 
        typedef boost::variant< Property, 
                                boost::recursive_wrapper<PropertyList> 
                                > Element;
        
        std::string name;
        std::string type;
        std::vector< Element > children;
    
        PropertyList() {}
        PropertyList(const std::string &n, const std::string &t) :
            name(n), type(t) {}
        
        size_t size() const { return children.size(); }
        
        Element get(unsigned i) const { return children.at(i); } 
        std::string get_type() const { return type; }
        std::string get_name() const { return name; }
	Property get_property(const std::string &path) const;
    };

    typedef std::vector<PropertyList> PropertySet;
}

#endif
