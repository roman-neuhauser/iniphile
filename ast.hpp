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

typedef std::stack<ast::branch *> astack;

void
push_path(valpath const & path, astack & stk) // {{{
{
    ast::branch *current = stk.top();
    BOOST_FOREACH (std::string const & label, path) {
        current->children[label] = ast::branch();
        current = &boost::get<ast::branch>(current->children[label]);
        stk.push(current);
    }
} // }}}
void
pop_path(valpath const & path, astack & stk) // {{{
{
    BOOST_FOREACH (std::string label, path) {
        stk.pop();
    }
} // }}}
valpath
dirname(valpath path) // {{{
{
    path.pop_back();
    return path;
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
    BOOST_FOREACH (metagram::section const & sec, cst) {
        valpath secpath(to_valpath(sec.first));
        push_path(secpath, stk);

        BOOST_FOREACH (metagram::assignment prop, sec.second) {
            valpath proppath(to_valpath(prop.first));
            valpath dir(dirname(proppath));
            std::string name(basename(proppath));
            push_path(dir, stk);
            add_sym(name, prop.second, *stk.top());
            pop_path(dir, stk);
        }
        pop_path(secpath, stk);
    }
    return root;
} // }}}

template <class Iter>
class find_node
: public boost::static_visitor<ast::node>
// {{{
{
public:
    find_node(Iter pos, Iter end)
    : pos(pos)
    , end(end)
    {}
    ast::node operator()(ast::branch const & b) const
    {
        if (pos == end) {
            return ast::node();
        }
        Iter p(pos);
        ast::branch::type::const_iterator next(b.children.find(*p));
        if (next == b.children.end()) {
            return ast::node();
        }
        ++p;
        Iter e(end);
        if (p == e) {
            return (*next).second;
        }
        return boost::apply_visitor(
            find_node<Iter>(p, e)
          , (*next).second
        );
    }
    ast::node operator()(ast::leaf const & l) const
    {
        return l;
    }
private:
    Iter pos;
    Iter end;
}; // }}}

std::string
get_string(ast::node const & cfg, valpath const & path) // {{{
{
    find_node<valpath::const_iterator> f(
        path.begin()
      , path.end()
    );
    ast::node rv = boost::apply_visitor(f, cfg);
    ast::leaf *l = boost::get<ast::leaf>(&rv);
    if (!l || l->value.empty()) {
        return "";
    }
    return l->value[0];
} // }}}

std::string
get_string(ast::node const & cfg, std::string const & path) // {{{
{
    return get_string(cfg, to_valpath(path));
} // }}}

} // namespace iniphile::ast

#endif
