
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_integer.h>

#include <test.h>

// can only be compiled with constant evaluation limits raised
#if defined(CNL_IMPL_ONEROUS_EVALUATION)
#if defined(__SIZEOF_INT128__)
TEST(static_integer, shift_left)  // NOLINT
{
    using namespace cnl::literals;

    auto expected = cnl::make_static_integer(
            231584178474632390847141970017375815706539969331281128078915168015826259279872_wide);
    auto actual = cnl::static_integer<260>{1} << 257;
    ASSERT_EQ(expected, actual);
}
#endif
#endif
