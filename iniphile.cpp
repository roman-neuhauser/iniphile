// Copyright (c) 2011 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=2 sts=2 et fdm=marker cms=\ //\ %s

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "input.hpp"
#include "output.hpp"
#include "ast.hpp"

#define foreach BOOST_FOREACH

namespace iniphile_cmdline // {{{
{

namespace ini = iniphile;
namespace ast = iniphile::ast;

using boost::format;

using std::cin;
using std::cout;
using std::cerr;

using std::string;
using std::vector;

#define USAGE_FMT \
  "usage: %1% file format query [default]\n" \
  " format:\n" \
  "  a - array, items terminated with '\\0'\n" \
  "  b - bool\n" \
  "  d - double\n" \
  "  l - long\n" \
  "  s - string\n"

const string FORMATS("abdls");

int const EX_OK = 0;
int const EX_USAGE = 64;
int const EX_DATAERR = 65;
int const EX_NOINPUT = 66;

static
int
usage(int argc, char **argv, int exit = EX_USAGE) // {{{
{
  if (argc == 0) return EX_USAGE;

  string self(*argv);
  string::size_type slash(self.find_last_of("\\/"));

  cerr << format(USAGE_FMT)
    % self.substr(slash == string::npos ? 0 : slash + 1)
  ;
  return exit;
} // }}}

static
std::streambuf*
open(std::ifstream &in, string const &file) // {{{
{
  if (file == "-") {
    return cin.rdbuf();
  } else {
    in.open(file.c_str());
    if (in.fail()) return 0;
    return in.rdbuf();
  }
} // }}}

static
bool
query(ast::node &afg, char fmt, string qry, string dflt) // {{{
{
  switch (fmt) {
  case 'a': {
    typedef vector<string> array;
    array defaults;
    array rv = ini::get(afg, qry, defaults);
    foreach (string &s, rv)
      cout << s << '\0';
    return true;
  }
  case 'b':
    cout << ini::get(afg, qry, ini::parse_(dflt, false));
    return true;

  case 'd':
    cout << ini::get(afg, qry, ini::parse_(dflt, 0.0));
    return true;

  case 'l':
    cout << ini::get(afg, qry, ini::parse_(dflt, 0L));
    return true;

  case 's':
    cout << ini::get(afg, qry, dflt);
    return true;
  }
  return false;
} // }}}

} // namespace iniphile_cmdline }}}

int
main(int argc, char **argv)
{
  using namespace iniphile_cmdline;

  if (argc < 4 || argc > 5) return usage(argc, argv);

  string file(argv[1]);
  string fmt(argv[2]);
  string qry(argv[3]);
  string dflt(argc == 5 ? argv[4] : "");

  if (fmt.length() != 1 || string::npos == FORMATS.find(fmt))
    return usage(argc, argv);

  std::ifstream in;
  std::streambuf* buf = open(in, file);
  if (!buf) return EX_NOINPUT;
  std::istream input(buf);

  std::ostringstream diag;
  ini::parse_result cfg(ini::parse(input, diag));

  if (!cfg) return EX_DATAERR;

  ast::node afg(ini::normalize(*cfg));

  return query(afg, fmt[0], qry, dflt)
    ? EX_OK
    : usage(argc, argv)
  ;
}
