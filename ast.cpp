// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <stack>
#include <string>

#include <boost/foreach.hpp>

#include <boost/spirit/include/qi.hpp>

#include "ast.hpp"

namespace iniphile
{
typedef std::stack<ast::branch *> astack;

void
push_path(valpath const & path, astack & stk) // {{{
{
    ast::branch *current = stk.top();
    BOOST_FOREACH (std::string const & label, path) {
        if (current->children.find(label) == current->children.end()
            || !boost::get<ast::branch>(&(current->children[label]))
        ) {
            current->children[label] = ast::branch();
        }
        current = boost::get<ast::branch>(&(current->children[label]));
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

valpath
to_valpath(std::string const & s) // {{{
{
    namespace ascii = boost::spirit::ascii;
    namespace qi = boost::spirit::qi;
    using qi::lexeme;
    using ascii::char_;

    valpath rv;
    std::string::const_iterator b(s.begin());
    qi::parse(
        b
      , s.end()
      , lexeme[+~char_('.')] % '.'
      , rv
    );
    return rv;
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

} // namespace iniphile

