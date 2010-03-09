// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_AST_HPP
#define INIPHILE_INCLUDE_AST_HPP

#include <map>
#include <string>

#include <boost/variant.hpp>

#include "astfwd.hpp"
#include "metagram.hpp"

namespace iniphile
{
namespace ast
{

struct leaf
{ // {{{
    metagram::optval value;
}; // }}}

struct branch;

typedef boost::variant<
    boost::recursive_wrapper<branch>
  , leaf
> node;

struct branch
{ // {{{
    typedef std::map<std::string, node> type;
    type children;
}; // }}}

} // namespace ast

ast::node
normalize(metagram::config const & cst);

} // namespace iniphile

#endif
