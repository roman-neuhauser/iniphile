// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_INPUT_HPP
#define INIPHILE_INCLUDE_INPUT_HPP

#include <boost/optional.hpp>

#include "metagram.hpp"

namespace iniphile
{

// public type
typedef metagram::config config;

typedef boost::optional<metagram::config> parse_result;

parse_result
parse(std::string const & input, std::ostream & erros);

parse_result
parse(std::istream & input, std::ostream & erros);

} // namespace iniphile

#endif
