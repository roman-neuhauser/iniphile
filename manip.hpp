// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_MANIP_HPP
#define INIPHILE_INCLUDE_MANIP_HPP

#include <vector>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_format.hpp>

#include "metagram.hpp"

namespace iniphile
{

valpath
to_valpath(std::string const & s) // {{{
{
    namespace ascii = boost::spirit::ascii;
    namespace qi = boost::spirit::qi;
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

template<class Seq, class Xform>
std::string
to_string(Seq const & pieces, Xform const & xf, std::string const & sep) // {{{
{
    namespace karma = boost::spirit::karma;
    typedef std::back_insert_iterator<std::string> Sink;

    std::string rv;
    Sink s(std::back_inserter(rv));
    karma::generate(
        s
      , xf % sep
      , pieces
    );
    return rv;
} // }}}

template<class Seq>
std::string
to_string(Seq const & pieces) // {{{
{
    namespace ascii = boost::spirit::ascii;

    return to_string(pieces, ascii::string, ".");
} // }}}

} // namespace iniphile

#endif
