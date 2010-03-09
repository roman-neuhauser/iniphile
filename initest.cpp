// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <iostream>
#include <fstream>
#include <string>

#include <limits>

#include "input.hpp"
#include "output.hpp"
#include "ast.hpp"

#define BOOST_TEST_DYN_LINK
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
