
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of features from library proposal, P0828

#if (__cplusplus>201402L) && defined(CNL_EXCEPTIONS_ENABLED)

#include <cnl/scaled_integer.h>
#include <cnl/elastic_integer.h>

#include <gtest/gtest.h>

namespace {
    using namespace cnl;
    using _impl::identical;

    namespace motivation_and_scope {
        using cnl::elastic_integer;

        namespace section_2_1 {
            CNL_NODISCARD constexpr auto positive_unsigned_overflow = UINT_MAX+1;  // 0U
            static_assert(identical(0U, positive_unsigned_overflow), "");

//            auto positive_signed_overflow = INT_MAX + 1; // undefined behavior

            CNL_NODISCARD constexpr auto negative_unsigned_overflow = 0U>-1;   // false
            static_assert(identical(false, negative_unsigned_overflow), "");
        }

        namespace section_2_2 {
            CNL_NODISCARD constexpr auto positive_unsigned_ok = static_cast<unsigned long long>(UINT_MAX)+1;
            static_assert(identical(0x100000000ULL, positive_unsigned_ok), "");

            CNL_NODISCARD constexpr auto positive_signed_ok = static_cast<long long>(INT_MAX)+1;
            static_assert(identical(0x80000000LL, positive_signed_ok), "");

            CNL_NODISCARD constexpr auto negative_unsigned_ok = static_cast<long long>(0U)>-1;
            static_assert(identical(true, negative_unsigned_ok), "");
        }

        namespace section_2_3 {
            template<typename Operand>
            CNL_NODISCARD constexpr auto multiply(Operand a, Operand b)
            {
                // Get the number of digits in the input type.
                CNL_NODISCARD constexpr auto operand_digits = digits_v<Operand>;

                // Results of multiplication contain twice the number of digits.
                CNL_NODISCARD constexpr auto result_digits = operand_digits*2;

                // Use this to determine the result type.
                using result_type = set_digits_t<Operand, result_digits>;

                // Promoted operands before operation.
                return static_cast<result_type>(a)*static_cast<result_type>(b);
            }

            static_assert(identical(INT64_C(0x7fffffff0000000), multiply(INT32_C(0x7fffffff), INT32_C(0x10000000))),
                    "");
            static_assert(identical(UINT32_C(0xffff000), multiply(uint16_t(0xffff), uint16_t(0x1000))), "");
        }

        namespace section_2_4 {
            CNL_NODISCARD constexpr auto square_root(elastic_integer<31> n)
            {
                return (n==2048000000) ? 45254.83399593904156165403917471 : throw std::exception();
            }

            CNL_NODISCARD constexpr auto d(elastic_integer<15> x, elastic_integer<15> y)  // [-32767..32767]
            {
                // When two values are multiplied, result is the sum of the `Digits` parameters.
                auto xx = x*x, yy = y*y;  // elastic_integer<30>

                // When two values are summed, result is the `Digits` parameter plus one.
                auto dd = xx+yy;    // elastic_integer<31>

                // A square root operation almost halves the number of digits required
                auto d = square_root(dd); // elastic_integer<31> in range [0..46339]

                // so we know it's safe to cast to a narrower type.
                return elastic_integer<16, unsigned>(d);
            }

            static_assert(identical(elastic_integer<16, unsigned>{45254}, d(32000, 32000)), "");
        }

        namespace section_2_5_a {
            CNL_NODISCARD constexpr auto a = elastic_integer(123);  // commonly deduced as elastic_integer<31>
            static_assert(identical(elastic_integer<31>{123}, a), "");

            CNL_NODISCARD constexpr auto b = elastic_integer(UINT64_C(4096));  // commonly deduced as elastic_integer<64, unsigned>
            static_assert(identical(elastic_integer<64, unsigned>{4096}, b), "");
        }

        namespace section_2_5_b {
            CNL_NODISCARD constexpr auto a = elastic_integer(constant<123>());  // deduced as elastic_integer<7>
            static_assert(identical(elastic_integer<7>{123}, a), "");

            CNL_NODISCARD constexpr auto b = elastic_integer(constant<4096>());  // deduced as elastic_integer<13>
            static_assert(identical(elastic_integer<13>{4096}, b), "");
        }

        namespace section_2_5_c {
            using namespace cnl::literals;

            CNL_NODISCARD constexpr auto a = elastic_integer(123_c);  // deduced as elastic_integer<7>
            static_assert(identical(elastic_integer<7>{123}, a), "");

            CNL_NODISCARD constexpr auto b = elastic_integer(4096_c);  // deduced as elastic_integer<13>
            static_assert(identical(elastic_integer<13>{4096}, b), "");
        }

        TEST(p0828, section_4_1_1) {
            elastic_integer<10> a = 1023;   // within range; OK

            auto b = elastic_integer<9>(a);  // narrowing cast exceeds range; undefined behavior
            ASSERT_GT(b, numeric_limits<elastic_integer<9>>::max());

            a += 1;   // increment exceeds range; undefined behavior
            ASSERT_GT(a, numeric_limits<elastic_integer<9>>::max());

            a = -1024;  // narrowing assignment exceeds range; undefined behavior
            ASSERT_LT(a, numeric_limits<elastic_integer<9>>::lowest());
        }

        namespace section_4_1_2a {
            CNL_NODISCARD constexpr auto kibi = scaled_integer<int32_t, cnl::power<-16>>(1024); // 2^26
            static_assert(identical(1<<26, to_rep(kibi)), "");

//            CNL_NODISCARD constexpr auto mebi = kibi * kibi;   // scaled_integer<int, -32>; value: 2^52
//            static_assert(identical(1LL<<52, to_rep(mebi)), "");
        }

        namespace section_4_1_2b {
            template<int Digits, int Exponent>
            using elastic_scaled_integer = scaled_integer<elastic_integer<Digits>, Exponent>;

            CNL_NODISCARD constexpr auto kibi = elastic_scaled_integer<31, -16>(1024); // stores value 2^26
            static_assert(identical(1<<26, to_rep(to_rep(kibi))), "");

            CNL_NODISCARD constexpr auto mebi = kibi * kibi;   // elastic_scaled_integer<62, -32> stores value: 2^52
            static_assert(identical(elastic_scaled_integer<62, -32>{1<<20}, mebi), "");
            static_assert(identical(INT64_C(1)<<52, to_rep(to_rep(mebi))), "");

        }

        namespace section_4_1_3 {
            struct date {
                elastic_integer<15, int8_t> year;   // [-2047..2047]
                elastic_integer<4, uint8_t> month;  // [0..15]
                elastic_integer<5, uint8_t> day;    // [0..31]
            };

            static_assert(sizeof(date)==4);

            CNL_NODISCARD constexpr auto epochalypse = date{2038, 1, 19};
            CNL_NODISCARD constexpr auto end_of_ice_age = date{-10000, 7, 19};

            static_assert(epochalypse.year > end_of_ice_age.year);
        }
    };

    namespace design_decisions {
        using cnl::elastic_integer;
        using cnl::scaled_integer;

        template<int Digits, int Exponent, class Narrowest>
        using elastic_scaled_integer = scaled_integer<elastic_integer<Digits, Narrowest>, Exponent>;

        // a 15-bit number with 10 integer digits and 5 fraction digits
        CNL_NODISCARD constexpr auto n = elastic_scaled_integer<15, -5, int>{31.96875};

        // a 30-bit number with 20 integer digits and 10 fraction digits;
        CNL_NODISCARD constexpr auto nn = n*n;

        static_assert(identical(elastic_scaled_integer<30, -10, int>{1022.0009765625}, nn), "");
    }
}

#endif
