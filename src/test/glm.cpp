
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#pragma warning(pop) 
#else
#include <glm/glm.hpp>
#endif

#include <cnl/scaled_integer.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    TEST(glm, char_multiply) {
        auto lhs = glm::tvec4<char>{7};
        auto rhs = glm::tvec4<char>{5};

        auto expected = glm::tvec4<int>{35};
        auto product = lhs*rhs;

        ASSERT_TRUE(cnl::_impl::identical(expected, product));
    }

    TEST(glm, fp_char_multiply) {
        using op_fp = cnl::scaled_integer<char, cnl::power<-4>>;
        auto lhs = glm::tvec4<op_fp>{op_fp{7.5}};
        auto rhs = glm::tvec4<op_fp>{op_fp{5.25}};

        using result_fp = cnl::scaled_integer<int, cnl::power<-8>>;
        auto expected = glm::tvec4<result_fp>{result_fp{39.375}};
        auto product = lhs*rhs;

        ASSERT_TRUE(cnl::_impl::identical(expected, product));
    }
}
