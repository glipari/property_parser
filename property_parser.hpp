#ifndef __PROPERTY_PARSER_HPP__
#define __PROPERTY_PARSER_HPP__

#include <map>
#include <vector>
#include <string>
#include <property.hpp>

namespace PropertyParser {
    bool parse_properties(std::istream &is, const std::string &filename,
                          PropertyList &pset);

    bool parse_properties_simple(std::istream& input, PropertyList &pset);
}

#endif
