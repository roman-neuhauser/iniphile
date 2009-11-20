// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_OUTPUT_HPP
#define INIPHILE_INCLUDE_OUTPUT_HPP

#include <boost/foreach.hpp>

#include "ast.hpp"

namespace iniphile
{

namespace karma = boost::spirit::karma;
namespace ascii = boost::spirit::ascii;

void
generate(std::ostream & out, config const & cfg) // {{{
{
    using std::endl;
    BOOST_FOREACH(auto sec, cfg) {
        out
            << "[" << sec.first << "]"
            << endl
        ;
        BOOST_FOREACH(auto ass, sec.second) {
            out << ass.first << " =";
            BOOST_FOREACH(auto w, ass.second) {
                out << " \"" << w << '"';
            }
            out << endl;
        }
        out
            << "; END SECTION "
            << sec.first
            << endl
        ;
    }
} // }}}

template <class Iter>
struct output_grammar 
: boost::spirit::karma::grammar<
    Iter
  , ast::branch()
  >
{
    template <class T>
    class my
    {
        typedef karma::rule<Iter, T> rule;
    };

    output_grammar()
      : output_grammar::base_type(file)
    {
        file = *section;
        section = header << *assignment;
        header = '[' << string << ']';
        assignment = propname << " = " << propvalue;
        propname = string;
        propvalue = string;
    }

    typename my<void()>::rule file;
    typename my<void()>::rule section;
    typename my<void()>::rule header;
    typename my<void()>::rule assignment;
    typename my<void()>::rule propname;
    typename my<void()>::rule propvalue;
};

void
generate(std::ostream & out, ast::node const & afg) // {{{
{
    using karma::stream;

    out << karma::format(
        stream
      , afg
    );

} // }}}

} // namespace iniphile

#endif
