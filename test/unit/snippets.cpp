
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_scaled_integer.h>

#include <cnl/_impl/config.h>

#include <gtest/gtest.h>

#include <sstream>
#include <type_traits>

using namespace cnl;

// contains snippets of code displayed in Doxygen documentation as examples

namespace define_a_scaled_integer_value {
    //! [define a scaled_integer value]
    constexpr auto n = scaled_integer<std::int8_t, power<-3>>{-2.75};
    static_assert(n == -2.75, "fixed-point type was unable to store the value");
    //! [define a scaled_integer value]
}

namespace define_a_fast_object_using_make_elastic {
    //! [define an int-sized object using make_elastic_scaled_integer and constant]
    // std::uint8_t specifies the type of const_integer - not elastic
    constexpr auto n = make_elastic_scaled_integer(constant<0xAA>{});

    static_assert(n == 0xAA, "n now has the value, 1024");
    static_assert(
            std::is_same<decltype(n), elastic_scaled_integer<7, power<1>, int> const>::value,
            "by default make_elastic_scaled_integer uses the most efficient type it can");
    //! [define an int-sized object using make_elastic_scaled_integer and constant]
}

namespace define_a_small_object_using_make_elastic {
    //! [define a byte-sized object using \ref make_elastic_scaled_integer and \ref _c]
    constexpr auto n = make_elastic_scaled_integer<char>(constant<1536>{});

    static_assert(n == 1536, "n now has the value, 1536");
    static_assert(
            std::is_same<decltype(n), elastic_scaled_integer<2, power<9>, char> const>::value,
            "by default make_elastic_scaled_integer uses the most efficient type it can");
    //! [define a byte-sized object using \ref make_elastic_scaled_integer and \ref _c]
}

namespace define_a_fast_object_using_elastic_literal {
    //! [define an object using elastic literal]
    using namespace cnl::literals;
    constexpr auto n = 3.141_cnl;

    static_assert(n == 3.141);
    static_assert(
            std::is_same<
                    elastic_scaled_integer<12, power<-3, 10>> const,
                    decltype(n)>::value);
    //! [define an object using elastic literal]
}

#if defined(CNL_IOSTREAMS_ENABLED)
TEST(snippets, scaled_integer_division)  // NOLINT
{
    std::stringstream cout;

    //! [scaled_integer division example]
    // The _cnl suffix prevents decimal rounding errors.
    using namespace cnl::literals;

    // Euros have subunits of 100^-1
    using euros = scaled_integer<int, power<-1, 100>>;

    auto funds{euros{5.00_cnl}};
    auto candle_price{euros{1.10_cnl}};

    cout << "Q: If I have €" << funds
         << " and candles cost €" << candle_price
         << ", how many candles can I buy?\n";

    // 5.00 / 1.10 = 4
    auto num_candles{funds / candle_price};

    // 5.00 % 1.10 = 0.60
    auto change{funds % candle_price};

    cout << "A: I get "
         << num_candles << " candles and €"
         << change << " change.\n";
    //! [scaled_integer division example]

    static_assert(std::is_same_v<decltype(num_candles), scaled_integer<int, power<0, 100>>>);
    ASSERT_EQ(4, num_candles);

    static_assert(std::is_same_v<decltype(change), euros>);
    ASSERT_EQ(0.60, change);

    auto constexpr expected_cout{"Q: If I have €5 and candles cost €1.1, how many candles can I buy?\n"
                                 "A: I get 4 candles and €.6 change.\n"};
    ASSERT_EQ(expected_cout, cout.str());
}
#endif
