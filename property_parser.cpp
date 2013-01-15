#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <vector>


#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
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

    bool parse_properties(std::istream& is, const std::string &filename, PropertyList &pset)
    {
	namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        namespace classic = boost::spirit::classic;

	is >> std::noskipws;

        typedef std::istreambuf_iterator<char> base_iterator_type;
        base_iterator_type in_begin(is);
    
        // convert input iterator to forward iterator, usable by spirit parser
        typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
        forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
        forward_iterator_type fwd_end;

        // wrap forward iterator with position iterator, to record the position
    	typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    	pos_iterator_type position_begin(fwd_begin, fwd_end, filename);
    	pos_iterator_type position_end;

	// typedef forward_iterator_type pos_iterator_type;
	// pos_iterator_type position_begin(fwd_begin);
    	// pos_iterator_type position_end;

        qi::rule<pos_iterator_type> skipper = ascii::space | 
	    '#' >> *(ascii::char_ - qi::eol) >> qi::eol; 

	property_set_grammar<pos_iterator_type, qi::rule<pos_iterator_type> > g;
	bool r = false;
	try {
            r = phrase_parse(position_begin, 
                             position_end, 
			     g, skipper, pset);
	}
	catch(const qi::expectation_failure<pos_iterator_type>& e) {
            const classic::file_position_base<std::string>& pos = e.first.get_position();
            std::stringstream msg;
            msg <<
                "parse error at file " << pos.file <<
                " line " << pos.line << " column " << pos.column << std::endl <<
                "'" << e.first.get_currentline() << "'" << std::endl <<
                std::setw(pos.column) << " " << "^- here";
            throw std::runtime_error(msg.str());
            // throw std::runtime_error("error");
        }

	return r;
    }

}
