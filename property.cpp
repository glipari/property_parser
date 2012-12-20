#include <algorithm>
#include <vector>

#include <property.hpp>
#include <property_visitor.hpp>
#include <string_utils.hpp>

using namespace std;

namespace PropertyParser {
    Property PropertyList::get_property(const std::string &path) const
    {
	vector<string> ss = StrUtils::split(path, ".");
	MatchNameVisitor vis(ss[0]);

	for (auto x : children) {
	    boost::apply_visitor(vis, x);
	    if (vis.result && vis.ptr1 && ss.size() == 1) 
		return *vis.ptr1;
	    else if (vis.result && vis.ptr2) {
		string rem = StrUtils::merge(ss.begin()+1, ss.end(), ".");
		return vis.ptr2->get_property(rem);
	    }    
	}
	throw PropertyNotFound(ss[0]);
    }

}
