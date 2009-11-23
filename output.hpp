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

template <class Iter>
struct output_grammar 
: boost::spirit::karma::grammar<
    Iter
  , metagram::config()
  >
// {{{
{
    template <class T>
    struct my
    {
        typedef karma::rule<Iter, T> rule;
    };

    output_grammar()
    : output_grammar::base_type(start)
    {
        using ascii::string;
        using karma::eol;

        start %= *section;
        section
            %=  headerline
            <<  *optionline
        ;
        headerline %= '[' << string << ']' << eol;
        optionline
            %=  optname
            <<  " = "
            <<  optval
            <<  eol
        ;
        optname = string;
        optval = ('"' << string << '"') % ' ';

        start.name("start");
        section.name("section");
        headerline.name("headerline");
        optionline.name("optionline");
        optname.name("optname");
        optval.name("optval");
    }

    typename my<metagram::config()>::rule start;
    typename my<metagram::section()>::rule section;
    typename my<metagram::sectionname()>::rule headerline;
    typename my<metagram::assignment()>::rule optionline;
    typename my<metagram::optname()>::rule optname;
    typename my<metagram::optval()>::rule optval;
}; // }}}

template<class Iter>
void
generate(Iter && sink, config const & cfg) // {{{
{
    output_grammar<Iter> g;
    karma::generate(
        sink
      , g
      , cfg
    );
} // }}}

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
