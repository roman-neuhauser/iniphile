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

    iniphile::parse_result cfg(
        iniphile::parse(input, std::cerr)
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
            << q << ":\n"
            << "string: " << iniphile::get(afg, q, std::string("not found")) << "\n"
            << "long: " << iniphile::get(afg, q, long(-1)) << "\n"
            << "double: " << iniphile::get(afg, q, -1.1) << "\n"
            << "bool: " << iniphile::get(afg, q, false) << "\n"
            //<< "char*: " << iniphile::get(afg, q, "wtf") << "\n"
        ;
    }

    return 0;
}
