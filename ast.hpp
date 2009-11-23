// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_AST_HPP
#define INIPHILE_INCLUDE_AST_HPP

#include <vector>
#include <stack>
#include <map>
#include <string>

#include <boost/foreach.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/map.hpp>

#include "metagram.hpp"
#include "manip.hpp"

namespace iniphile
{
namespace ast
{

struct leaf
{ // {{{
    metagram::optval value;
}; // }}}

std::ostream &
operator<<(std::ostream & os, leaf const & lf) // {{{
{
    os << "(leaf)";
    BOOST_FOREACH(auto w, lf.value) {
        os << ":" << w;
    }
    os << "(/leaf)";
    return os;
} // }}}

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

std::ostream &
operator<<(std::ostream & os, branch const & br) // {{{
{
    os << "(branch)";
    BOOST_FOREACH(auto child, br.children) {
        os << child.first << " " << child.second << std::endl;
    }
    os << "(/branch)\n";
    return os;
} // }}}

} // namespace ast

typedef std::stack<ast::branch *> astack;

void
push_path(valpath const & path, astack & stk) // {{{
{
    auto current = stk.top();
    BOOST_FOREACH (auto label, path) {
        current->children[label] = ast::branch();
        current = &boost::get<ast::branch>(current->children[label]);
        stk.push(current);
    }
} // }}}
void
pop_path(valpath const & path, astack & stk) // {{{
{
    BOOST_FOREACH (auto label, path) {
        stk.pop();
    }
} // }}}
valpath
dirname(valpath const & path) // {{{
{
    auto rv(path);
    rv.pop_back();
    return rv;
} // }}}
std::string
basename(valpath const & path) // {{{
{
    return path.back();
} // }}}
void
add_sym(metagram::optname name, metagram::optval val, ast::branch & scope) // {{{
{
    ast::leaf v;
    v.value = val;
    scope.children[name] = v;
} // }}}

ast::node
normalize(metagram::config const & cst) // {{{
{
    astack stk;
    ast::branch root;
    stk.push(&root);
    BOOST_FOREACH (auto sec, cst) {
        auto secpath(to_valpath(sec.first));
        push_path(secpath, stk);

        BOOST_FOREACH (auto prop, sec.second) {
            auto proppath(to_valpath(prop.first));
            auto dir(dirname(proppath));
            auto name(basename(proppath));
            push_path(dir, stk);
            add_sym(name, prop.second, *stk.top());
            pop_path(dir, stk);
        }
        pop_path(secpath, stk);
    }
    return root;
} // }}}

} // namespace iniphile::ast

#endif
