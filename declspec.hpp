// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_INCLUDE_DECLSPEC_HPP
#define INIPHILE_INCLUDE_DECLSPEC_HPP

#ifdef INIPHILE_DLL
# ifdef LIBINIPHILE_SOURCE
#  define DLLEXPORT __declspec(dllexport)
# else
#  define DLLEXPORT __declspec(dllimport)
# endif
#else
# define DLLEXPORT
#endif

#endif
