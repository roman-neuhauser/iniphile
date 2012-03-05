// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <iostream>
#include <fstream>
#include <string>

#include <limits>

#include "input.hpp"
#include "output.hpp"
#include "ast.hpp"

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE iniphile tests

#include <boost/test/unit_test.hpp>

namespace ini = iniphile;
namespace ast = iniphile::ast;

BOOST_AUTO_TEST_CASE(parsing_empty_file_succeeds) // {{{
{
    std::ostringstream diag;
    std::istringstream input("");
    BOOST_CHECK_EQUAL(true, !!ini::parse(input, diag));
    BOOST_CHECK_EQUAL("", diag.str());
} // }}}

BOOST_AUTO_TEST_CASE(syntax_error) // {{{
{
    std::ostringstream diag;
    std::istringstream input("syntax error");
    BOOST_CHECK_EQUAL(false, !!ini::parse(input, diag));
    BOOST_CHECK_NE("", diag.str());
} // }}}

template<class T>
static
void
check_get_failure(ast::node const& afg, T exp, T dflt) // {{{
{
    BOOST_CHECK_EQUAL(exp, ini::get(afg, std::string("qux"), dflt));
} // }}}

BOOST_AUTO_TEST_CASE(key_not_found) // {{{
{
    std::ostringstream diag;
    std::istringstream input("");
    ini::parse_result cfg(ini::parse(input, diag));

    BOOST_CHECK_EQUAL(true, !!cfg);
    BOOST_CHECK_EQUAL("", diag.str());

    ast::node afg(ini::normalize(*cfg));

    check_get_failure(
        afg
      , std::string("it's not there")
      , std::string("it's not there")
    );
    check_get_failure(afg, true, true);
    check_get_failure(afg, false, false);
    check_get_failure(afg, 42L, 42L);
    check_get_failure(afg, 6.9, 6.9);

    std::vector<std::string> dflt, rv;
    dflt.push_back("not found");
    dflt.push_back("really missing");
    rv = ini::get(afg, std::string("qux"), dflt);
    BOOST_CHECK_EQUAL(unsigned(2), rv.size());
    BOOST_CHECK_EQUAL(dflt[0], rv[0]);
    BOOST_CHECK_EQUAL(dflt[1], rv[1]);
} // }}}

#define CHECK_BOOL(afg, p, exp) \
    BOOST_CHECK_EQUAL(exp, ini::get(afg, std::string(p), !exp))

#define CHECK_LONG(afg, p, exp) \
    BOOST_CHECK_EQUAL(exp, ini::get(afg, std::string(p), 1L))

#define CHECK_DOUBLE(afg, p, exp) \
    BOOST_CHECK_CLOSE(exp, ini::get(afg, std::string(p), 1.0), 0.00001)

#define CHECK_STRING(afg, p, exp, dflt) \
    BOOST_CHECK_EQUAL(exp, ini::get(afg, std::string(p), std::string(dflt)))

#define CHECK_STRINGS(afg, p, exp, dflt) \
    { \
        std::vector<std::string> rv = ini::get(afg, std::string(p), dflt); \
        BOOST_CHECK_EQUAL_COLLECTIONS(exp.begin(), exp.end(), rv.begin(), rv.end()); \
    }

BOOST_AUTO_TEST_CASE(get_bool) // {{{
{
    std::ostringstream diag;
    std::istringstream input(
        "[bools]\n"
        "t-digit = 1\n"
        "f-digit = 0\n"
        "t-yesno = YeS\n"
        "f-yesno = nO\n"
        "t-truefalse = tRuE\n"
        "f-truefalse = FAlsE\n"
        "t-onoff = oN\n"
        "f-onoff = ofF\n"
    );

    ast::node afg(ini::normalize(*ini::parse(input, diag)));

    CHECK_BOOL(afg, "bools.t-digit", true);
    CHECK_BOOL(afg, "bools.f-digit", false);

    CHECK_BOOL(afg, "bools.t-yesno", true);
    CHECK_BOOL(afg, "bools.f-yesno", false);

    CHECK_BOOL(afg, "bools.t-truefalse", true);
    CHECK_BOOL(afg, "bools.f-truefalse", false);

    CHECK_BOOL(afg, "bools.t-onoff", true);
    CHECK_BOOL(afg, "bools.f-onoff", false);
} // }}}

BOOST_AUTO_TEST_CASE(get_long) // {{{
{
    std::ostringstream diag;
    std::istringstream input(
        "[longs]\n"
        "dec-zero = 0\n"
        "dec-max = +2147483647\n"
        "dec-min = -2147483648\n"
    );

    long max = std::numeric_limits<long>::max();
    long min = std::numeric_limits<long>::min();

    ast::node afg(ini::normalize(*ini::parse(input, diag)));

    CHECK_LONG(afg, "longs.dec-max", max);
    CHECK_LONG(afg, "longs.dec-min", min);
    CHECK_LONG(afg, "longs.dec-zero", 0);
} // }}}

BOOST_AUTO_TEST_CASE(get_double) // {{{
{
    std::ostringstream diag;
    std::istringstream input(
        "[doubles]\n"
        "dec-zero = 0\n"
        "dec-max = 1.7976931348623157e+308\n"
        "dec-min = 2.2250738585072014e-308\n"
    );

    double max = std::numeric_limits<double>::max();
    double min = std::numeric_limits<double>::min();

    ast::node afg(ini::normalize(*ini::parse(input, diag)));

    CHECK_DOUBLE(afg, "doubles.dec-max", max);
    CHECK_DOUBLE(afg, "doubles.dec-min", min);
    CHECK_DOUBLE(afg, "doubles.dec-zero", 0);
} // }}}

BOOST_AUTO_TEST_CASE(get_string) // {{{
{
    std::ostringstream diag;
    std::istringstream input(
        "[strings]\n"
        "word = Hello\n"
        "words = hellO  World\n"
        "qstring = \"hello  WORLD\"\n"
        "mix = hello  \"  WORLD\"  \n  again\n"
    );

    ast::node afg(ini::normalize(*ini::parse(input, diag)));

    CHECK_STRING(afg, "strings.word", "Hello", "");
    CHECK_STRING(afg, "strings.words", "hellO World", "");
    CHECK_STRING(afg, "strings.qstring", "hello  WORLD", "");
    CHECK_STRING(afg, "strings.mix", "hello \"  WORLD\" again", "");
    CHECK_STRING(afg, "strings.not-there", "fallback", "fallback");
} // }}}

BOOST_AUTO_TEST_CASE(get_strings) // {{{
{
    std::ostringstream diag;
    std::istringstream input(
        "[strings]\n"
        "word = Hello\n"
        "words = hellO  World\n"
        "qstring = \"hello  WORLD\"\n"
        "mix = hello  \"  WORLD\"\n"
        "  again\n"
        "qstring-nl = \"  \n"
        "hello\n"
        " world\n"
        " \"\n"
    );

    ast::node afg(ini::normalize(*ini::parse(input, diag)));

    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("absent");
        exp = dflt;
        CHECK_STRINGS(afg, "strings.not-there", exp, dflt);
    }
    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("");
        exp.push_back("Hello");
        CHECK_STRINGS(afg, "strings.word", exp, dflt);
    }
    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("");
        exp.push_back("hellO");
        exp.push_back("World");
        CHECK_STRINGS(afg, "strings.words", exp, dflt);
    }
    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("");
        exp.push_back("hello  WORLD");
        CHECK_STRINGS(afg, "strings.qstring", exp, dflt);
    }
    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("");
        exp.push_back("  \nhello\n world\n ");
        CHECK_STRINGS(afg, "strings.qstring-nl", exp, dflt);
    }
    {
        std::vector<std::string> dflt, exp;
        dflt.push_back("");
        exp.push_back("hello");
        exp.push_back("  WORLD");
        exp.push_back("again");
        CHECK_STRINGS(afg, "strings.mix", exp, dflt);
    }
} // }}}

