// Copyright (c) 2013 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=2 sts=2 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_ERROR_HANDLER_HPP
#define INIPHILE_INCLUDE_ERROR_HANDLER_HPP

#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>

namespace iniphile
{

template <class Iter>
struct error_handler
{
  template <class, class, class>
  struct result { typedef void type; };

  error_handler(std::ostream& erros)
  : erros(erros)
  {}

  template <class Args, class Context, class Rule>
  void operator()(Args const& args, Context const&, Rule const&) const
  {
    Iter const& rulebeg = boost::fusion::at_c<0>(args);
    Iter const& ruleend = boost::fusion::at_c<1>(args);
    erros
      << "Error! Expecting "
      << boost::fusion::at_c<3>(args)
      << " here: "
      << std::string(rulebeg, ruleend)
      << std::endl
    ;
  }

  std::ostream& erros;
};

} // namespace iniphile

#endif // INIPHILE_INCLUDE_ERROR_HANDLER_HPP
