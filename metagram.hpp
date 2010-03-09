// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_METAGRAM_HPP
#define INIPHILE_INCLUDE_METAGRAM_HPP

#include <vector>
#include <string>
#include <utility>

namespace iniphile
{

namespace metagram
{

typedef std::string string;
typedef string bareword;
typedef string optname;
typedef string qstring;
typedef std::vector<string> optval;
typedef string sectionname;
typedef std::pair<optname, optval> assignment;
typedef std::vector<assignment> sectionbody;
typedef std::pair<sectionname, sectionbody> section;
typedef std::vector<section> config;

} // namespace metagram

// public types
typedef metagram::config config;

} // namespace iniphile

#endif
