// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_OUTPUT_HPP
#define INIPHILE_INCLUDE_OUTPUT_HPP

#include <boost/foreach.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include "ast.hpp"

namespace iniphile
{

namespace karma = boost::spirit::karma;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

template <class Iter>
struct possibly_quoted_output
: boost::spirit::karma::grammar<
    Iter
  , metagram::string()
  > // {{{
{
    template <class T>
    struct my
    {
        typedef karma::rule<Iter, T> rule;
    };
    possibly_quoted_output() // {{{
    : possibly_quoted_output::base_type(start)
    {
        using ascii::string;
        using ascii::alnum;
        using ascii::char_;
        using karma::repeat;
        using karma::_val;

        start
            %= bareword
             | qstring
        ;
        bareword %= repeat(phx::size(_val))[
            alnum | char_("-.,_$")
        ];
        qstring %= '"' << string << '"';
    } // }}}
    typename my<metagram::string()>::rule start;
    typename my<metagram::qstring()>::rule qstring;
    typename my<metagram::bareword()>::rule bareword;
}; // }}}

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
        using ascii::alnum;
        using ascii::char_;
        using ascii::blank;
        using karma::eol;
        using karma::repeat;
        using karma::_val;

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
        optname %= string;

        optval %= possibly_quoted % ' ';

        start.name("start");
        section.name("section");
        headerline.name("headerline");
        optionline.name("optionline");
        optname.name("optname");
        optval.name("optval");
        possibly_quoted.name("possibly_quoted");
    }

    typename my<metagram::config()>::rule start;
    typename my<metagram::section()>::rule section;
    typename my<metagram::sectionname()>::rule headerline;
    typename my<metagram::assignment()>::rule optionline;
    typename my<metagram::optname()>::rule optname;
    typename my<metagram::optval()>::rule optval;
    possibly_quoted_output<Iter> possibly_quoted;
}; // }}}

template<class Iter>
void
generate(Iter & sink, config const & cfg) // {{{
{
    output_grammar<Iter> g;
    karma::generate(
        sink
      , g
      , cfg
    );
} // }}}

std::string
get_string(ast::node const & cfg, valpath const & path) // {{{
{
    find_node<valpath::const_iterator> f(
        path.begin()
      , path.end()
    );
    ast::node nd = boost::apply_visitor(f, cfg);
    ast::leaf *l = boost::get<ast::leaf>(&nd);
    if (!l || l->value.empty()) {
        return "";
    }

    typedef std::back_insert_iterator<std::string> Sink;
    std::string rv;
    Sink s(std::back_inserter(rv));
    karma::generate(
        s
      , (possibly_quoted_output<Sink>() % ' ')
      , l->value
    );
    return rv;
} // }}}

std::string
get_string(ast::node const & cfg, std::string const & path) // {{{
{
    return get_string(cfg, to_valpath(path));
} // }}}

} // namespace iniphile

#endif
