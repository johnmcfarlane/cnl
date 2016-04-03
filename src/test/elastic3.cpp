
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "elastic_common.h"

template
struct elastic_test_with_integer_digits<-1>;

template
struct elastic_test_with_integer_digits<-8>;

template
struct elastic_test_with_integer_digits<-27>;
