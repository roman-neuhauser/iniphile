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
    std::string const p(argc > 2 ? argv[2] : ".");
    auto q(iniphile::to_valpath(p));
    std::ifstream input(s, std::ios_base::binary);
    input.unsetf(std::ios::skipws);
    std::istreambuf_iterator<char> b(input), e;

    auto cfg = iniphile::parse(b, e, std::cerr);

    if (!cfg) {
        return 1;
    }

    auto afg = iniphile::normalize(*cfg);

    if (q.empty()) {
        iniphile::generate(std::cout, *cfg);
        std::cout << "=================" << std::endl;
        namespace ast = iniphile::ast;
        iniphile::generate(std::cout, afg);
    } else {
        std::cout << iniphile::get_string(*cfg, q);
    }

    return 0;
}
