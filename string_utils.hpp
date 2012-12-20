#include <string>
#include <vector>

namespace StrUtils {
    /** removes spaces from the beginning and from the end of the string */
    std::string remove_spaces(const std::string &tk);
    
    /** split the string into a vector of words, using sep as separator */
    std::vector<std::string> split(const std::string &code, const std::string &sep); 
    
    /** merge the strings, using the specified separator (inverse of split) */
    template<class Iter>
    std::string merge(Iter b, Iter e, const std::string &sep)
    {
	std::string r = *b;
	for (Iter i = b+1; i!= e; ++i) 
	    r += sep + *i;
	return r;
    }
}
