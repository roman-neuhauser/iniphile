// Copyright (c) 2011 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=2 sts=2 et fdm=marker cms=\ //\ %s

#include <iostream>
#include <fstream>
#include <string>

#include <boost/format.hpp>

using std::cin;
using std::ofstream;
using std::cerr;

using std::string;
using std::vector;

using boost::format;

int
main(int argc, char **argv)
{
  if (argc < 3) return EXIT_FAILURE;

  ofstream output(argv[1]);

  for (int i = 2; i < argc; ++i) {
    string def(argv[i]);
    string defname(def.substr(0, def.find('=')));
    string defval(def.substr(def.find('=') + 1));
    output << format("#define %1% %2%\n") % defname % defval;
    output << format("#define %1%_QUOTED \"%2%\"\n") % defname % defval;
  }

  return EXIT_SUCCESS;
}
