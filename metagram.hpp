// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_METAGRAM
#define INIPHILE_INCLUDE_METAGRAM

#include <vector>
#include <string>
#include <utility>

namespace iniphile { namespace metagram
{

typedef std::string bareword;
typedef std::string optname;
typedef std::string qstring;
typedef std::vector<std::string> optval;
typedef std::string sectionname;
typedef std::pair<optname, optval> assignment;
typedef std::pair<sectionname, std::vector<assignment> > section;
typedef std::vector<section> config;

} } // namespace iniphile::metagram

#endif
