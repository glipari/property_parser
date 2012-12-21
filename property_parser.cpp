#include <string>
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <gen_parse.hpp>
#include <property_parser.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    PropertyParser::Property, 
    (std::string, name)
    (std::string, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    PropertyParser::PropertyList,
    (std::string, type)
    (std::string, name)
    (std::vector< PropertyParser::PropertyList::Element >, children)
    )

namespace PropertyParser {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;

    template <typename Iterator, typename Skipper>
    struct property_set_grammar : qi::grammar<Iterator, PropertyList(),
                                              Skipper>
    {
        qi::rule<Iterator, Property(), Skipper> prop;
        qi::rule<Iterator, std::string(), Skipper> name;
        qi::rule<Iterator, std::string(), Skipper> type;
        qi::rule<Iterator, std::string(), Skipper> value;
        qi::rule<Iterator, std::string(), Skipper> value_simple;
        qi::rule<Iterator, std::string(), Skipper> value_quoted;
        qi::rule<Iterator, PropertyList(), Skipper> plist;

        property_set_grammar() : 
            property_set_grammar::base_type(plist, "Set of Properties") {
            using qi::lit;
            using qi::alpha;
            using qi::alnum;
            using qi::lexeme;
            using qi::char_;

            name = lexeme[alpha >> *alnum];
            type = lexeme[alpha >> *alnum];
            value_simple = lexeme[*(alnum - lit('"'))];
            value_quoted = lit('"') > lexeme[*(char_ - lit('"'))] > lit('"');
            value = (value_quoted | value_simple);

            prop = name >> '=' > value > ';';
            plist = type >> '(' > name > ')' > '{' >> *(prop | plist) > '}' > ';';
        }
    };   

    bool parse_properties(std::istream &is, const std::string &filename,
        PropertyList &pset)
    {
        return PropertyParser::myparse<property_set_grammar, PropertyList>(is, filename, pset);
    }

    bool parse_properties_simple(std::istream& is, PropertyList &pset)
    {
        return PropertyParser::simple_parse<property_set_grammar, PropertyList>(is, pset);
    }

}
