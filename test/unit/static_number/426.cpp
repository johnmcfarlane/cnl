
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>

#include <test.h>

#include <limits>

template<int IntegerDigits, int FractionalDigits, class Narrowest>
using saturated_elastic_scaled_integer = cnl::static_number<
        IntegerDigits + FractionalDigits, -FractionalDigits, cnl::native_rounding_tag,
        cnl::saturated_overflow_tag>;

using temp_wide_t = saturated_elastic_scaled_integer<23, 8, int32_t>;
using temp_t = saturated_elastic_scaled_integer<7, 8, int16_t>;

TEST(_426, half)  // NOLINT
{
    // non-clipping/non-overflowing conversion
    constexpr temp_wide_t tw = 0.5;
    constexpr temp_t t = tw;
    CNL_ASSERT_EQ(temp_t(0.5), t);
}

TEST(_426, minus_150)  // NOLINT
{
    // clipping conversion should be constrained to min/max
    constexpr temp_wide_t tw = -150;
    constexpr temp_t t = tw;
    constexpr temp_t lowest = std::numeric_limits<temp_t>::lowest();
    CNL_ASSERT_EQ(lowest, t);

    CNL_ASSERT_EQ(-32767, cnl::unwrap(t));
    CNL_ASSERT_EQ(-32767, cnl::unwrap(lowest));
}

TEST(_426, plus_150)  // NOLINT
{
    constexpr temp_wide_t tw = 150;  // NOLINT(cppcoreguidelines-init-variables)
    constexpr temp_t t = tw;  // NOLINT(cppcoreguidelines-init-variables)
    CNL_ASSERT_EQ(std::numeric_limits<temp_t>::max(), t);
}
