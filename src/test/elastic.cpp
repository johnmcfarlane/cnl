
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests sg14::elastic type with a variety of instantiations

#include "elastic_common.h"

template
struct elastic_test_with_integer_digits<-43>;

template
struct elastic_test_with_integer_digits<-31>;

template
struct elastic_test_with_integer_digits<-19>;

template
struct elastic_test_with_integer_digits<-5>;

template
struct elastic_test_with_integer_digits<-1>;

template
struct elastic_test_with_integer_digits<0>;

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<8>;

template
struct elastic_test_with_integer_digits<13>;

template
struct elastic_test_with_integer_digits<16>;

template
struct elastic_test_with_integer_digits<19>;

template
struct elastic_test_with_integer_digits<31>;
