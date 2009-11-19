#ifndef INIPHILE_INCLUDE_MANIP
#define INIPHILE_INCLUDE_MANIP

#include <vector>
#include <string>

#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_format.hpp>
#include <boost/foreach.hpp>

#include "metagram.hpp"

namespace iniphile
{

typedef std::vector<std::string> valpath;

std::string
get_string(metagram::config & cfg, valpath path)
{
    return "not yet";
}

valpath
to_valpath(std::string const & s)
{
    valpath rv;
    auto b(s.begin());
    qi::parse(
        b
      , s.end()
      , lexeme[+~char_('.')] % '.'
      , rv
    );
    return rv;
}

std::string
to_string(valpath path)
{
    namespace karma = boost::spirit::karma;
    namespace ascii = boost::spirit::ascii;
    using ascii::string;
    using karma::eol;

    std::string rv;
    auto sink(std::back_inserter(rv));
    karma::generate(
        sink
      , string % '.' << eol
      , path
    );
    return rv;
}

} // namespace iniphile

#endif

// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s
