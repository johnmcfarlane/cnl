//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_TYPES)
#define CNL_DSP_TYPES

#include <cnl.h>

// Elastic fixed point types equivalent to Cirrus ADSP2
typedef cnl::elastic_number<24, -20> q4_20;
typedef cnl::elastic_number<48, -40> q8_40;

#endif //CNL_DSP_TYPES
