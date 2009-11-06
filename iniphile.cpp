// Copyright (c) 2009 Roman Neuhauser
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace iniphile
{

using boost::optional;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

using ascii::alnum;
using ascii::alpha;
using ascii::blank;
using ascii::char_;
using ascii::digit;
using ascii::no_case;
using ascii::space;
using ascii::string;
using qi::int_;
using qi::lit;
using qi::lexeme;
using qi::skip;
using qi::eoi;
using qi::eol;
using qi::omit;
using phx::val;
using phx::construct;
using qi::_1;
using qi::_2;
using qi::_3;
using qi::_4;

using qi::on_error;
using qi::fail;

namespace metagram
{
typedef std::string bareword;
typedef std::string optname;
typedef std::string qstring;
typedef std::vector<std::string> optval;
typedef std::string sectionname;
typedef std::pair<optname, optval> assignment;
typedef std::pair<sectionname, std::vector<assignment> > section;
typedef std::vector<section> config;
} // namespace metagram

// public type
typedef metagram::config config;

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

    grammar() : grammar::base_type(start)
    {
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
        sectionname %= lexeme[+~char_(']')];
        optname %= bareword;
        optval %= (qstring | bareword) % +blank;

        bareword %= lexeme[+(alnum | char_("-.,_$"))];
        qstring %= lexeme['"' > *~char_('"') > '"'];

        on_error<fail>
        (
            start
          , std::cerr
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
    }
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
optional<metagram::config>
parse(Iter & first, Iter last)
{
    grammar<Iter> g;
    metagram::config cfg;
    optional<metagram::config> rv;
    bool ok = qi::parse(
        first
      , last
      , g
      , cfg
    );
    if (ok && first == last)
        rv = cfg;
    return rv;
}

} // namespace iniphile

int
main(int argc, char **argv)
{
    std::string const s(argc > 1 ? argv[1] : "/dev/stdin");
    std::ifstream input(s, std::ios_base::binary);
    input.unsetf(std::ios::skipws);
    std::istreambuf_iterator<char> b(input), e;

    auto cfg = iniphile::parse(b, e);

    if (!cfg) {
        return 1;
    }

    auto & out(std::cout);
    using std::endl;
    BOOST_FOREACH(auto sec, *cfg) {
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
        out << "; END SECTION " << sec.first << endl;
    }

    return 0;
}

