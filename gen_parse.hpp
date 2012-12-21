#ifndef __GEN_PARSE_HPP__
#define __GEN_PARSE_HPP__

#include <istream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace PropertyParser {

    template<template <class, class> class Grammar, class DataType> 
    bool myparse(std::istream& input, const std::string filename, DataType &result)
    {
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        namespace classic = boost::spirit::classic;
   
        typedef std::istreambuf_iterator<char> base_iterator_type;
        base_iterator_type in_begin(input);
    
        // convert input iterator to forward iterator, usable by spirit parser
        typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
        forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
        forward_iterator_type fwd_end;

        // wrap forward iterator with position iterator, to record the position
    	typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    	pos_iterator_type position_begin(fwd_begin, fwd_end, filename);
    	pos_iterator_type position_end;

        qi::rule<pos_iterator_type> skipper = ascii::space | 
	    '#' >> *(ascii::char_ - qi::eol) >> qi::eol; 

        Grammar<pos_iterator_type, qi::rule<pos_iterator_type> > g;

        bool r = false;
  
        try {
            r = phrase_parse(position_begin, 
                             position_end, 
                             g, skipper, result);

            //std::cout << "parsed with no exceptions, r = " << r << std::endl;
        }
        catch(const qi::expectation_failure<pos_iterator_type>& e) {
            const classic::file_position_base<std::string>& pos = e.first.get_position();
            std::stringstream msg;
            msg <<
                "parse error at file " << pos.file <<
                " line " << pos.line << " column " << pos.column << std::endl <<
                "'" << e.first.get_currentline() << "'" << std::endl <<
                std::setw(pos.column) << " " << "^- here";
            throw std::runtime_error("puppa"); //msg.str());
        }
    
        return r;
    }

    template<template <class, class> class Grammar, class DataType> 
    bool simple_parse(std::istream& input, DataType &result)
    {
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        typedef std::istreambuf_iterator<char> base_iterator_type;
        base_iterator_type start(input);
        typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
        forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(start);
	forward_iterator_type fwd_end;

	typedef boost::spirit::classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    	pos_iterator_type position_begin(fwd_begin, fwd_end, "internal");
    	pos_iterator_type position_end;

	//typedef pos_iterator_type used_iterator_type;
	// used_iterator_type s = position_begin;
	// used_iterator_type e = position_end;
	typedef forward_iterator_type used_iterator_type;
	used_iterator_type s = fwd_begin;
	used_iterator_type e = fwd_end;

        qi::rule<used_iterator_type> skipper = ascii::space;
        Grammar<used_iterator_type, qi::rule<used_iterator_type> > g;

	bool r;
	
	//std::vector<double> v;
	try {
	    r =  phrase_parse(s, e, g, skipper, result);
	}  catch(const qi::expectation_failure<used_iterator_type>& e) {
	    std::cout << "Error!" << std::endl;
	    throw std::runtime_error("puppa");
	}
	return r;
    }


}
#endif
