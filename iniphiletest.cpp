// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <iostream>
#include <fstream>
#include <string>

#include "metagram.hpp"
#include "input.hpp"
#include "output.hpp"
#include "manip.hpp"
#include "ast.hpp"

int
main(int argc, char **argv)
{
    std::string const s(argc > 1 ? argv[1] : "/dev/stdin");
    std::string const q(argc > 2 ? argv[2] : "");
    std::ifstream input(s.c_str(), std::ios_base::binary);
    input.unsetf(std::ios::skipws);
    std::istreambuf_iterator<char> bb(input), ee;
    std::string tmp(bb, ee);
    std::string::iterator b(tmp.begin()), e(tmp.end());

    iniphile::parse_result cfg(
        iniphile::parse(b, e, std::cerr)
    );

    if (!cfg) {
        return 1;
    }

    iniphile::ast::node afg(iniphile::normalize(*cfg));

    typedef std::ostream_iterator<char> Sink;

    if (q.empty()) {
        Sink s(std::cout);
        iniphile::generate(s, *cfg);
    } else {
        std::cout
            << q
            << ": "
            << iniphile::get_string(afg, q)
        ;
    }

    return 0;
}
