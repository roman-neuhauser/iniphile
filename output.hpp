// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_OUTPUT_HPP
#define INIPHILE_INCLUDE_OUTPUT_HPP

#include <boost/foreach.hpp>

#include "metagram.hpp"

namespace iniphile
{

void
generate(std::ostream & out, config const & cfg) // {{{
{
    using std::endl;
    BOOST_FOREACH(auto sec, cfg) {
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
        out
            << "; END SECTION "
            << sec.first
            << endl
        ;
    }
} // }}}

} // namespace iniphile

#endif
