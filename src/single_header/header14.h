
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// mechanically retrieved, single-header version of CNL library
// https://github.com/johnmcfarlane/cnl

#if ! defined(CNL_COMPLETE_H)
#define CNL_COMPLETE_H

#if (__cplusplus == 199711L) && defined(_MSC_VER)
#error Required Visual C++ compiler flags: /std:c++14 /Zc:__cplusplus /EHsc
#endif

#if (__cplusplus < 201402L)
#error This build of CNL requires C++14 or above.
#endif
