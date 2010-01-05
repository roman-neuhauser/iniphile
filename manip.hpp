// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_MANIP_HPP
#define INIPHILE_INCLUDE_MANIP_HPP

#include <vector>
#include <string>

#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_format.hpp>

#include "metagram.hpp"

namespace iniphile
{

valpath
to_valpath(std::string const & s) // {{{
{
    using qi::lexeme;
    using ascii::char_;

    valpath rv;
    std::string::const_iterator b(s.begin());
    qi::parse(
        b
      , s.end()
      , lexeme[+~char_('.')] % '.'
      , rv
    );
    return rv;
} // }}}

template<class Seq>
std::string
to_string(Seq pieces) // {{{
{
    namespace karma = boost::spirit::karma;
    namespace ascii = boost::spirit::ascii;
    using ascii::string;

    typedef std::back_insert_iterator<std::string> Sink;
    std::string rv;
    Sink s(std::back_inserter(rv));
    karma::generate(
        s
      , string % '.'
      , pieces
    );
    return rv;
} // }}}

} // namespace iniphile

#endif
