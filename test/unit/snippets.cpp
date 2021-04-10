
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_scaled_integer.h>

#include <cnl/_impl/config.h>

#include <gtest/gtest.h>

#include <sstream>

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
    // How many candles can I buy?

    // Euros have subunits of 10^-2
    using euros = scaled_integer<int, power<-2, 10>>;

    // Using the _cnl suffix helps avoid decimal rounding errors
    using namespace cnl::literals;

    // I have €5
    constexpr auto funds{euros{5}};

    // Candles cost €1.10¢
    auto candle_price{euros{1.10_cnl}};

    // I can buy 5 / 1.10 = 4 candles; result is a whole number
    auto num_candles{funds / candle_price};

    // I get 5.00 % 1.10 = 0.60¢ change; result has resolution of 0.01
    auto change{funds % candle_price};

    // "I get 4 candles and €.6 change.\n"
    cout << "I get " << num_candles << " candles and €" << change << " change.\n";
    //! [scaled_integer division example]

    static_assert(std::is_same_v<decltype(num_candles), scaled_integer<int, power<0, 10>>>);
    ASSERT_EQ(4, num_candles);

    static_assert(std::is_same_v<decltype(change), euros>);
    ASSERT_EQ(0.60, change);

    ASSERT_EQ("I get 4 candles and €.6 change.\n", cout.str());
}
#endif
