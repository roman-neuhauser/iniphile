// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#define LIBINIPHILE_SOURCE

#include "error-handler.hpp"
#include "input.hpp"

namespace iniphile
{

namespace ascii = boost::spirit::ascii;
namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

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

    grammar(std::string const fname, Iter const& srcbeg, Iter const& srcend, std::ostream& erros)
    : grammar::base_type(start)
    { // {{{

        using ascii::alnum;
        using ascii::blank;
        using ascii::char_;
        using ascii::print;
        using ascii::space;

        using qi::eoi;
        using qi::eol;

        using qi::lexeme;
        using qi::omit;

        using qi::_val;
        using qi::_1;
        using qi::_2;
        using qi::_3;
        using qi::_4;
        using qi::on_error;
        using qi::fail;

        using phx::insert;
        using phx::push_back;
        using phx::begin;
        using phx::end;
        using phx::val;
        using phx::construct;

        start
            %=  *section
            >   eoi
        ;
        optionline
            %=  optname
            >>  omit[*blank]
            >>  '='
            >>  omit[*blank]
            >>  optval
        ;
        section
            %=  *commentline
            >>  headerline
            >>  sectionbody
        ;
        headerline
            %=  lexeme['[' > sectionname > ']']
            >>  -comment
            >>  eol
        ;
        sectionbody
            =   *(
                    commentline
                  | optionline  [push_back(_val, _1)]
                 )
        ;
        commentline
            %=  -comment
            >>  eol
        ;
        sectionname %= lexeme[+~char_("\n\r]")];
        optname %= lexeme[+(!space >> !char_('=') >> print)];
        optval
             =  -ovline[_val = _1]
            >>  -comment
            >>  eol
            >>  *(
                    omit[+blank]
                 >> ovline[insert(_val, end(_val), begin(_1), end(_1))]
                 >> -comment
                 >> eol
                 )
        ;
        ovline
            %=  ((qstring | bareword) % +blank)
        ;
        comment %= omit[*blank >> -(';' >> *~char_("\n\r"))];

        bareword %= lexeme[+(!space >> !char_(';') >> print)];
        qstring %= lexeme['"' > *~char_('"') > '"'];

        on_error<fail>
        (
            start
          , error_handler<Iter>(fname, srcbeg, srcend, erros)
        );

        start.name("start");
        commentline.name("commentline");
        optionline.name("optionline");
        section.name("section");
        headerline.name("headerline");
        sectionname.name("sectionname");
        sectionbody.name("sectionbody");
        optname.name("optname");
        optval.name("optval");
        ovline.name("optvalline");
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
    typename my<metagram::sectionbody()>::rule sectionbody;
    typename my<metagram::optname()>::rule optname;
    typename my<metagram::optval()>::rule optval;
    typename my<metagram::optval()>::rule ovline;
    typename my<metagram::qstring()>::rule qstring;
    typename my<metagram::bareword()>::rule bareword;
    typename my<void()>::rule comment;
};

template<class Iter>
parse_result
parse(std::string const& fname, Iter& first, Iter last, std::ostream& erros) // {{{
{
    grammar<Iter> g(fname, first, last, erros);
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

parse_result
parse(std::string const& fname, std::string const& input, std::ostream& erros)
{
    std::string::const_iterator b(input.begin()), e(input.end());
    return parse(fname, b, e, erros);
}

parse_result
parse(std::string const& fname, std::istream& input, std::ostream& erros)
{
    std::istreambuf_iterator<char> b(input), e;
    return parse(fname, std::string(b, e), erros);
}

} // namespace iniphile
