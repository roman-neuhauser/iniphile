// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_INPUT_HPP
#define INIPHILE_INCLUDE_INPUT_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include "metagram.hpp"

namespace iniphile
{

using boost::optional;

namespace ascii = boost::spirit::ascii;
namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

// public type
typedef metagram::config config;

typedef optional<metagram::config> parse_result;

template<class Iter>
struct
grammar
: qi::grammar<Iter, metagram::config()>
{
    template<class T>
    struct my
    {
        typedef qi::rule<Iter, T> rule;
    };

    grammar(std::ostream & erros)
    : grammar::base_type(start)
    { // {{{

        using ascii::alnum;
        using ascii::blank;
        using ascii::char_;
        using ascii::space;

        using qi::eoi;
        using qi::eol;

        using qi::lexeme;
        using qi::omit;

        using qi::_2;
        using qi::_3;
        using qi::_4;
        using qi::on_error;
        using qi::fail;

        using phx::val;
        using phx::construct;

        start
            %=  *section
            >>  eoi
        ;
        optionline
            %=  optname
            >   omit[*blank]
            >   '='
            >   omit[*space]
            >   optval
            >   eol
        ;
        section
            %=  headerline
            >   *optionline
        ;
        headerline
            %=  lexeme['[' > sectionname > ']']
            >   eol
        ;
        sectionname %= lexeme[+~char_("\n\r]")];
        optname %= bareword;
        optval %= (qstring | bareword) % +blank;

        bareword %= lexeme[+(alnum | char_("-.,_$"))];
        qstring %= lexeme['"' > *~char_('"') > '"'];

        on_error<fail>
        (
            start
          , erros
                << val("Error! Expecting ")
                << _4
                << val(" here: \"")
                << construct<std::string>(_3, _2)
                << val("\"")
                << std::endl
        );

        start.name("start");
        commentline.name("commentline");
        optionline.name("optionline");
        section.name("section");
        headerline.name("headerline");
        sectionname.name("sectionname");
        optname.name("optname");
        optval.name("optval");
        bareword.name("bareword");
        qstring.name("qstring");
        comment.name("comment");
    } // }}}
    typename my<metagram::config()>::rule start;
    typename my<void()>::rule commentline;
    typename my<metagram::assignment()>::rule optionline;
    typename my<metagram::section()>::rule section;
    typename my<metagram::sectionname()>::rule headerline;
    typename my<metagram::sectionname()>::rule sectionname;
    typename my<metagram::optname()>::rule optname;
    typename my<metagram::optval()>::rule optval;
    typename my<metagram::qstring()>::rule qstring;
    typename my<metagram::bareword()>::rule bareword;
    typename my<void()>::rule comment;
};

template<class Iter>
parse_result
parse(Iter & first, Iter last, std::ostream & erros) // {{{
{
    grammar<Iter> g(erros);
    metagram::config cfg;
    parse_result rv;
    bool ok = qi::parse(
        first
      , last
      , g
      , cfg
    );
    if (ok && first == last)
        rv = cfg;
    return rv;
} // }}}

} // namespace iniphile

#endif
