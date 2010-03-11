// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_OUTPUT_HPP
#define INIPHILE_INCLUDE_OUTPUT_HPP

#include "astfwd.hpp"
#include "metagram.hpp"

namespace iniphile
{

template<class T>
void
generate(T& sink, config const& cfg);

template<class T>
T
get(ast::node const& cfg, valpath const& path, T dflt);

template<class T>
T
get(ast::node const& cfg, std::string const& path, T dflt);

} // namespace iniphile

#endif
