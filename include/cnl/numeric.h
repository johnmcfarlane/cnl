
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
    template<typename T> inline constexpr T pi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T invpi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T invsqrtpi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T ln2{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T ln10{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T sqrt2{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T sqrt3{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T invsqrt2{sqrt2<T>/2};
    template<typename T> inline constexpr T invsqrt3{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T radian{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T egamma{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T phi{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T catalan{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T apery{_impl::deleted_fn<T>()};
    template<typename T> inline constexpr T glaisher{_impl::deleted_fn<T>()};

    template<> inline constexpr long double e<long double>{2.718281828459045235360287471352662498L};
    template<> inline constexpr long double log2e<long double>{1.442695040888963407359924681001892137L};
    template<> inline constexpr long double log10e<long double>{0.434294481903251827651128918916605082L};
    template<> inline constexpr long double pi<long double>{3.141592653589793238462643383279502884L};
    template<> inline constexpr long double invpi<long double>{0.318309886183790671537767526745028724L};
    template<> inline constexpr long double invsqrtpi<long double>{0.564189583547756286948079451560772585844050629329L};
    template<> inline constexpr long double ln2<long double>{0.693147180559945309417232121458176568L};
    template<> inline constexpr long double ln10<long double>{2.302585092994045684017991454684364208L};
    template<> inline constexpr long double sqrt2<long double>{1.414213562373095048801688724209698079L};
    template<> inline constexpr long double sqrt3<long double>{1.73205080756887729352744634150587236694280525381038062805580L};
    template<> inline constexpr long double invsqrt3<long double>{0.57735026918962576450914878050195745564760175127013L};
    template<> inline constexpr long double radian<long double>{57.295779513082320876798154814105170332405472466564L};
    template<> inline constexpr long double egamma<long double>{0.5772156649015328606065120900824024L};
    template<> inline constexpr long double phi<long double>{1.6180339887498948482045868343656381L};
    template<> inline constexpr long double catalan<long double>{0.915965594177219015054603514932384110774L};
    template<> inline constexpr long double apery<long double>{1.202056903159594285399738161511449990L};
    template<> inline constexpr long double glaisher<long double>{1.282427129100622636875342568869791727L};

    template<> inline constexpr double e<double>{2.7182818284590452354};
    template<> inline constexpr double log2e<double>{1.4426950408889634074};
    template<> inline constexpr double log10e<double>{0.43429448190325182765};
    template<> inline constexpr double pi<double>{3.14159265358979323846};
    template<> inline constexpr double invpi<double>{0.31830988618379067154};
    template<> inline constexpr double invsqrtpi<double>{0.564189583547756286948079451560772585844050629329L};
    template<> inline constexpr double ln2<double>{0.69314718055994530942};
    template<> inline constexpr double ln10<double>{2.30258509299404568402};
    template<> inline constexpr double sqrt2<double>{1.41421356237309504880};
    template<> inline constexpr double sqrt3<double>{1.73205080756887729352744634150587236694280525381038062805580L};
    template<> inline constexpr double invsqrt3<double>{0.57735026918962576450914878050195745564760175127013L};
    template<> inline constexpr double radian<double>{57.295779513082320876798154814105170332405472466564L};
    template<> inline constexpr double egamma<double>{0.5772156649015328606065120900824024L};
    template<> inline constexpr double phi<double>{1.6180339887498948482045868343656381L};
    template<> inline constexpr double catalan<double>{0.915965594177219015054603514932384110774L};
    template<> inline constexpr double apery<double>{1.202056903159594285399738161511449990L};
    template<> inline constexpr double glaisher<double>{1.282427129100622636875342568869791727L};

    template<> inline constexpr float e<float>{2.7182818284590452354};
    template<> inline constexpr float log2e<float>{1.4426950408889634074};
    template<> inline constexpr float log10e<float>{0.43429448190325182765};
    template<> inline constexpr float pi<float>{3.14159265358979323846};
    template<> inline constexpr float invpi<float>{0.31830988618379067154};
    template<> inline constexpr float invsqrtpi<float>{0.564189583547756286948079451560772585844050629329L};
    template<> inline constexpr float ln2<float>{0.69314718055994530942};
    template<> inline constexpr float ln10<float>{2.30258509299404568402};
    template<> inline constexpr float sqrt2<float>{1.41421356237309504880};
    template<> inline constexpr float sqrt3<float>{1.73205080756887729352744634150587236694280525381038062805580L};
    template<> inline constexpr float invsqrt3<float>{0.57735026918962576450914878050195745564760175127013L};
    template<> inline constexpr float radian<float>{57.295779513082320876798154814105170332405472466564L};
    template<> inline constexpr float egamma<float>{0.5772156649015328606065120900824024L};
    template<> inline constexpr float phi<float>{1.6180339887498948482045868343656381L};
    template<> inline constexpr float catalan<float>{0.915965594177219015054603514932384110774L};
    template<> inline constexpr float apery<float>{1.202056903159594285399738161511449990L};
    template<> inline constexpr float glaisher<float>{1.282427129100622636875342568869791727L};

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
