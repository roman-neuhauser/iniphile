// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_ASTFWD_HPP
#define INIPHILE_INCLUDE_ASTFWD_HPP

#include <vector>
#include <string>

#include <boost/variant.hpp>

namespace iniphile
{

typedef std::vector<std::string> valpath;

valpath
to_valpath(std::string const & s);

namespace ast
{
struct leaf;
struct branch;

typedef boost::variant<
    boost::recursive_wrapper<branch>
  , leaf
> node;

} // namespace ast
} // namespace iniphile

#endif // INIPHILE_INCLUDE_ASTFWD_HPP
