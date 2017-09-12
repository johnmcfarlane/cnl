
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief functions that might belong in the <numeric> header

#if !defined(CNL_NUMERIC_H)
#define CNL_NUMERIC_H

#include <cnl/bits/common.h>
#include <cnl/num_traits.h>

#include <limits.h>

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // math constants

#if defined(__cpp_inline_variables)

    // partial implementation of P0631 (http://wg21.link/p0631)
    // values taken from c++ (GCC) 7.2.0 implementation of math.h
    // disclaimer: may be a bit or two off

    template<typename T> inline constexpr T e{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T log2e{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T log10e{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T ln2{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T ln10{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T pi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T pi_2{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T pi_4{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T one_pi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T two_pi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T two_sqrtpi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T sqrt2{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T one_sqrt2{_impl::deleted_fn<T>()};

    template<> inline constexpr long double e<long double>{2.718281828459045235360287471352662498L};
    template<> inline constexpr long double log2e<long double>{1.442695040888963407359924681001892137L};
    template<> inline constexpr long double log10e<long double>{0.434294481903251827651128918916605082L};
    template<> inline constexpr long double ln2<long double>{0.693147180559945309417232121458176568L};
    template<> inline constexpr long double ln10<long double>{2.302585092994045684017991454684364208L};
    template<> inline constexpr long double pi<long double>{3.141592653589793238462643383279502884L};
    template<> inline constexpr long double pi_2<long double>{1.570796326794896619231321691639751442L};
    template<> inline constexpr long double pi_4<long double>{0.785398163397448309615660845819875721L};
    template<> inline constexpr long double one_pi<long double>{0.318309886183790671537767526745028724L};
    template<> inline constexpr long double two_pi<long double>{0.636619772367581343075535053490057448L};
    template<> inline constexpr long double two_sqrtpi<long double>{1.128379167095512573896158903121545172L};
    template<> inline constexpr long double sqrt2<long double>{1.414213562373095048801688724209698079L};
    template<> inline constexpr long double one_sqrt2<long double>{0.707106781186547524400844362104849039L};

    template<> inline constexpr double e<double>{2.7182818284590452354};
    template<> inline constexpr double log2e<double>{1.4426950408889634074};
    template<> inline constexpr double log10e<double>{0.43429448190325182765};
    template<> inline constexpr double ln2<double>{0.69314718055994530942};
    template<> inline constexpr double ln10<double>{2.30258509299404568402};
    template<> inline constexpr double pi<double>{3.14159265358979323846};
    template<> inline constexpr double pi_2<double>{1.57079632679489661923};
    template<> inline constexpr double pi_4<double>{0.78539816339744830962};
    template<> inline constexpr double one_pi<double>{0.31830988618379067154};
    template<> inline constexpr double two_pi<double>{0.63661977236758134308};
    template<> inline constexpr double two_sqrtpi<double>{1.12837916709551257390};
    template<> inline constexpr double sqrt2<double>{1.41421356237309504880};
    template<> inline constexpr double one_sqrt2<double>{0.70710678118654752440};

    template<> inline constexpr float e<float>{2.7182818284590452354};
    template<> inline constexpr float log2e<float>{1.4426950408889634074};
    template<> inline constexpr float log10e<float>{0.43429448190325182765};
    template<> inline constexpr float ln2<float>{0.69314718055994530942};
    template<> inline constexpr float ln10<float>{2.30258509299404568402};
    template<> inline constexpr float pi<float>{3.14159265358979323846};
    template<> inline constexpr float pi_2<float>{1.57079632679489661923};
    template<> inline constexpr float pi_4<float>{0.78539816339744830962};
    template<> inline constexpr float one_pi<float>{0.31830988618379067154};
    template<> inline constexpr float two_pi<float>{0.63661977236758134308};
    template<> inline constexpr float two_sqrtpi<float>{1.12837916709551257390};
    template<> inline constexpr float sqrt2<float>{1.41421356237309504880};
    template<> inline constexpr float one_sqrt2<float>{0.70710678118654752440};

#endif  // defined(__cpp_inline_variables)

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::trailing_bits

    namespace _numeric_impl {
        template<class Integer>
        constexpr int trailing_bits_positive(Integer value, int mask_bits = sizeof(Integer)*CHAR_BIT/2)
        {
            return ((value & ((Integer{1} << mask_bits)-1))==0)
                   ? mask_bits+trailing_bits_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? trailing_bits_positive(value, mask_bits/2)
                     : 0;
        }

        template<class Integer, class Enable = void>
        struct trailing_bits {
            static constexpr int f(Integer value)
            {
                return value ? trailing_bits_positive(value) : 0;
            }
        };

        template<class Integer>
        struct trailing_bits<Integer, _impl::enable_if_t<numeric_limits<Integer>::is_signed>> {
            static constexpr int f(Integer value)
            {
                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
                return (value>0)
                       ? trailing_bits_positive(value)
                       : (value<0)
                         ? trailing_bits_positive(-value)
                         : 0;
            }
        };
    }

    template<class Integer>
    constexpr int trailing_bits(Integer value)
    {
        return _numeric_impl::trailing_bits<Integer>::f(value);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::used_bits

    namespace _numeric_impl {
        template<class Integer>
        constexpr int used_bits_positive(Integer value, int mask_bits = sizeof(Integer)*CHAR_BIT/2)
        {
            static_assert(std::numeric_limits<Integer>::is_integer,
                          "Integer parameter of used_bits_positive() must be a fundamental integer.");

            return (value>=(Integer{1} << mask_bits))
                   ? mask_bits+used_bits_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? used_bits_positive(value, mask_bits/2)
                     : 1;
        }
    }

    namespace _impl {
        template<class Integer>
        constexpr int used_bits_symmetric(Integer value)
        {
            // Most negative number is not exploited;
            // thus negating the result or subtracting it from something else
            // will less likely result in overflow.
            return (value>0)
                   ? _numeric_impl::used_bits_positive<Integer>(value)
                   : (value<0)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4146)
#endif
                     ? _numeric_impl::used_bits_positive<Integer>(-value)
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
                     : 0;
        }
    }

    namespace _numeric_impl {
        struct used_bits {
            template<class Integer>
            constexpr _impl::enable_if_t<!numeric_limits<Integer>::is_signed, int> operator()(Integer value) const
            {
                return value ? used_bits_positive(value) : 0;
            }

            template<class Integer, class = _impl::enable_if_t<numeric_limits<Integer>::is_signed, int>>
            constexpr int operator()(Integer value) const
            {
                static_assert(std::numeric_limits<Integer>::is_integer,
                              "Integer parameter of used_bits()() must be a fundamental integer.");

                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
                return (value>0)
                       ? used_bits_positive(value)
                       : (value==0)
                         ? 0
                         : used_bits()(Integer(-1)-value);
            }
        };
    }

    template<class Integer>
    constexpr int used_bits(Integer value)
    {
        return for_rep<int>(_numeric_impl::used_bits(), value);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::leading_bits

#if !defined(_MSC_VER) && !defined(CNL_DISABLE_GCC_BUILTINS)
    constexpr int leading_bits(int value)
    {
        return (value>0)
               ? __builtin_clz(value)-1
               : digits<int>::value-used_bits(value);
    }
#endif

    template<class Integer>
    constexpr int leading_bits(Integer const&value)
    {
        return digits<Integer>::value-used_bits(value);
    }
}

#endif  // CNL_NUMERIC_H
