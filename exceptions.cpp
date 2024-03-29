#include "exceptions.hpp"

using namespace std;

namespace PropertyParser {
    Exception::Exception(const std::string &w, const std::string &s) :
    		exception(), where(w), msg(s) {}

    string Exception::what() { return where + " " + msg; }
}
