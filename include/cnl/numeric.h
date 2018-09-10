
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/numeric.h
/// \brief functions that might belong in the \verbatim<numeric>\endverbatim header

#if !defined(CNL_NUMERIC_H)
#define CNL_NUMERIC_H

#include "bit.h"

#include "_impl/num_traits/unwrap.h"
#include "_impl/used_digits.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::math_constants
    
#if defined(__cpp_inline_variables)
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::deleted_fn

        // a great function to call if you need a compiler error
        template<typename T>
        constexpr T deleted_fn() = delete;
    }

    /// partial implementation of [P0631](http://wg21.link/p0631)

    // values taken from c++ (GCC) 7.2.0 implementation of math.h; disclaimer: may be a bit or two off

    ////////////////////////////////////////////////////////////////////////////////
    // primary templates

    /// approximation of [e](https://en.wikipedia.org/wiki/E_(mathematical_constant));
    /// primary variable template
    template<typename T> inline constexpr T e{_impl::deleted_fn<T>()};

    /// approximation of the [binary logarithm](https://en.wikipedia.org/wiki/Binary_logarithm) of e, `log2(e)`;
    /// primary variable template
    template<typename T> inline constexpr T log2e{_impl::deleted_fn<T>()};

    /// approximation of the [decimal logarithm](https://en.wikipedia.org/wiki/Binary_logarithm) of e, `log10(e)`;
    /// primary variable template
    template<typename T> inline constexpr T log10e{_impl::deleted_fn<T>()};

    /// approximation of [pi](https://en.wikipedia.org/wiki/Pi);
    /// primary variable template
    template<typename T> inline constexpr T pi{_impl::deleted_fn<T>()};

    /// approximation of `1/pi`;
    /// primary variable template
    template<typename T> inline constexpr T invpi{_impl::deleted_fn<T>()};

    /// approximation of `1/sqrt(pi)`;
    /// primary variable template
    template<typename T> inline constexpr T invsqrtpi{_impl::deleted_fn<T>()};

    /// approximation of the [natural logarithm](https://en.wikipedia.org/wiki/Natural_logarithm) of 2, `ln(2)`;
    /// primary variable template
    template<typename T> inline constexpr T ln2{_impl::deleted_fn<T>()};

    /// approximation of the [natural logarithm](https://en.wikipedia.org/wiki/Natural_logarithm) of 10, `ln(10)`;
    /// primary variable template
    template<typename T> inline constexpr T ln10{_impl::deleted_fn<T>()};

    /// approximation of the square root of 2, `sqrt(2)`;
    /// primary variable template
    template<typename T> inline constexpr T sqrt2{_impl::deleted_fn<T>()};

    /// approximation of the square root of 3, `sqrt(3)`;
    /// primary variable template
    template<typename T> inline constexpr T sqrt3{_impl::deleted_fn<T>()};

    /// approximation of the inverse of the square root of 2, `1/sqrt(2)`;
    /// primary variable template
    template<typename T> inline constexpr T invsqrt2{sqrt2<T>/2};

    /// approximation of the inverse of the square root of 3, `1/sqrt(3)`;
    /// primary variable template
    template<typename T> inline constexpr T invsqrt3{_impl::deleted_fn<T>()};

    /// approximation of a radian in degrees, `180/pi`;
    /// primary variable template
    template<typename T> inline constexpr T radian{_impl::deleted_fn<T>()};

    /// approximation of the [Euler–Mascheroni constant](https://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant);
    /// primary variable template
    template<typename T> inline constexpr T egamma{_impl::deleted_fn<T>()};

    /// approximation of the [golden ratio](https://en.wikipedia.org/wiki/Golden_ratio);
    /// primary variable template
    template<typename T> inline constexpr T phi{_impl::deleted_fn<T>()};

    /// approximation of [Catalan's constant](https://en.wikipedia.org/wiki/Catalan%27s_constant);
    /// primary variable template
    template<typename T> inline constexpr T catalan{_impl::deleted_fn<T>()};

    /// approximation of [Apéry's constant](https://en.wikipedia.org/wiki/Ap%C3%A9ry%27s_constant);
    /// primary variable template
    template<typename T> inline constexpr T apery{_impl::deleted_fn<T>()};

    /// approximation of the [Glaisher–Kinkelin constant](https://en.wikipedia.org/wiki/Glaisher%E2%80%93Kinkelin_constant);
    /// primary variable template
    template<typename T> inline constexpr T glaisher{_impl::deleted_fn<T>()};

    ////////////////////////////////////////////////////////////////////////////////
    // long double specializations

    /// specialization of \ref cnl::e for `long double`
    template<> inline constexpr long double e<long double>{2.718281828459045235360287471352662498L};

    /// specialization of \ref cnl::log2e for `long double`
    template<> inline constexpr long double log2e<long double>{1.442695040888963407359924681001892137L};

    /// specialization of \ref cnl::log10e for `long double`
    template<> inline constexpr long double log10e<long double>{0.434294481903251827651128918916605082L};

    /// specialization of \ref cnl::pi for `long double`
    template<> inline constexpr long double pi<long double>{3.141592653589793238462643383279502884L};

    /// specialization of \ref cnl::invpi for `long double`
    template<> inline constexpr long double invpi<long double>{0.318309886183790671537767526745028724L};

    /// specialization of \ref cnl::invsqrtpi for `long double`
    template<> inline constexpr long double invsqrtpi<long double>{
            0.564189583547756286948079451560772585844050629329L};

    /// specialization of \ref cnl::ln2 for `long double`
    template<> inline constexpr long double ln2<long double>{0.693147180559945309417232121458176568L};

    /// specialization of \ref cnl::ln10 for `long double`
    template<> inline constexpr long double ln10<long double>{2.302585092994045684017991454684364208L};

    /// specialization of \ref cnl::sqrt2 for `long double`
    template<> inline constexpr long double sqrt2<long double>{1.414213562373095048801688724209698079L};

    /// specialization of \ref cnl::sqrt3 for `long double`
    template<> inline constexpr long double sqrt3<long double>{
            1.73205080756887729352744634150587236694280525381038062805580L};

    /// specialization of \ref cnl::invsqrt3 for `long double`
    template<> inline constexpr long double invsqrt3<long double>{
            0.57735026918962576450914878050195745564760175127013L};

    /// specialization of \ref cnl::radian for `long double`
    template<> inline constexpr long double radian<long double>{
            57.295779513082320876798154814105170332405472466564L};

    /// specialization of \ref cnl::egamma for `long double`
    template<> inline constexpr long double egamma<long double>{0.5772156649015328606065120900824024L};

    /// specialization of \ref cnl::phi for `long double`
    template<> inline constexpr long double phi<long double>{1.6180339887498948482045868343656381L};

    /// specialization of \ref cnl::catalan for `long double`
    template<> inline constexpr long double catalan<long double>{0.915965594177219015054603514932384110774L};

    /// specialization of \ref cnl::apery for `long double`
    template<> inline constexpr long double apery<long double>{1.202056903159594285399738161511449990L};

    /// specialization of \ref cnl::glaisher for `long double`
    template<> inline constexpr long double glaisher<long double>{1.282427129100622636875342568869791727L};

    ////////////////////////////////////////////////////////////////////////////////
    // double specializations

    /// specialization of \ref cnl::e for `double`
    template<> inline constexpr double e<double>{2.7182818284590452354};

    /// specialization of \ref cnl::log2e for `double`
    template<> inline constexpr double log2e<double>{1.4426950408889634074};

    /// specialization of \ref cnl::log10e for `double`
    template<> inline constexpr double log10e<double>{0.43429448190325182765};

    /// specialization of \ref cnl::pi for `double`
    template<> inline constexpr double pi<double>{3.14159265358979323846};

    /// specialization of \ref cnl::invpi for `double`
    template<> inline constexpr double invpi<double>{0.31830988618379067154};

    /// specialization of \ref cnl::invsqrtpi for `double`
    template<> inline constexpr double invsqrtpi<double>{0.564189583547756286948079451560772585844050629329};

    /// specialization of \ref cnl::ln2 for `double`
    template<> inline constexpr double ln2<double>{0.69314718055994530942};

    /// specialization of \ref cnl::ln10 for `double`
    template<> inline constexpr double ln10<double>{2.30258509299404568402};

    /// specialization of \ref cnl::sqrt2 for `double`
    template<> inline constexpr double sqrt2<double>{1.41421356237309504880};

    /// specialization of \ref cnl::sqrt3 for `double`
    template<> inline constexpr double sqrt3<double>{
            1.73205080756887729352744634150587236694280525381038062805580};

    /// specialization of \ref cnl::invsqrt3 for `double`
    template<> inline constexpr double invsqrt3<double>{0.57735026918962576450914878050195745564760175127013};

    /// specialization of \ref cnl::radian for `double`
    template<> inline constexpr double radian<double>{57.295779513082320876798154814105170332405472466564};

    /// specialization of \ref cnl::egamma for `double`
    template<> inline constexpr double egamma<double>{0.5772156649015328606065120900824024};

    /// specialization of \ref cnl::phi for `double`
    template<> inline constexpr double phi<double>{1.6180339887498948482045868343656381};

    /// specialization of \ref cnl::catalan for `double`
    template<> inline constexpr double catalan<double>{0.915965594177219015054603514932384110774};

    /// specialization of \ref cnl::apery for `double`
    template<> inline constexpr double apery<double>{1.202056903159594285399738161511449990};

    /// specialization of \ref cnl::glaisher for `double`
    template<> inline constexpr double glaisher<double>{1.282427129100622636875342568869791727};

    ////////////////////////////////////////////////////////////////////////////////
    // float specializations

    /// specialization of \ref cnl::e for `float`
    template<> inline constexpr float e<float>{2.7182818284590452354f};

    /// specialization of \ref cnl::log2e for `float`
    template<> inline constexpr float log2e<float>{1.4426950408889634074f};

    /// specialization of \ref cnl::log10e for `float`
    template<> inline constexpr float log10e<float>{0.43429448190325182765f};

    /// specialization of \ref cnl::pi for `float`
    template<> inline constexpr float pi<float>{3.14159265358979323846f};

    /// specialization of \ref cnl::invpi for `float`
    template<> inline constexpr float invpi<float>{0.31830988618379067154f};

    /// specialization of \ref cnl::invsqrtpi for `float`
    template<> inline constexpr float invsqrtpi<float>{0.564189583547756286948079451560772585844050629329f};

    /// specialization of \ref cnl::ln2 for `float`
    template<> inline constexpr float ln2<float>{0.69314718055994530942f};

    /// specialization of \ref cnl::ln10 for `float`
    template<> inline constexpr float ln10<float>{2.30258509299404568402f};

    /// specialization of \ref cnl::sqrt2 for `float`
    template<> inline constexpr float sqrt2<float>{1.41421356237309504880f};

    /// specialization of \ref cnl::sqrt3 for `float`
    template<> inline constexpr float sqrt3<float>{1.73205080756887729352744634150587236694280525381038062805580f};

    /// specialization of \ref cnl::invsqrt3 for `float`
    template<> inline constexpr float invsqrt3<float>{0.57735026918962576450914878050195745564760175127013f};

    /// specialization of \ref cnl::radian for `float`
    template<> inline constexpr float radian<float>{57.295779513082320876798154814105170332405472466564f};

    /// specialization of \ref cnl::egamma for `float`
    template<> inline constexpr float egamma<float>{0.5772156649015328606065120900824024f};

    /// specialization of \ref cnl::phi for `float`
    template<> inline constexpr float phi<float>{1.6180339887498948482045868343656381f};

    /// specialization of \ref cnl::catalan for `float`
    template<> inline constexpr float catalan<float>{0.915965594177219015054603514932384110774f};

    /// specialization of \ref cnl::apery for `float`
    template<> inline constexpr float apery<float>{1.202056903159594285399738161511449990f};

    /// specialization of \ref cnl::glaisher for `float`
    template<> inline constexpr float glaisher<float>{1.282427129100622636875342568869791727f};
#endif  // defined(__cpp_inline_variables)

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::trailing_bits

    namespace _numeric_impl {
        template<class Integer, bool IsSigned>
        struct trailing_bits {
            constexpr int operator()(Integer const& integer) const noexcept
            {
                return countr_zero(integer);
            }
        };

        template<class Integer>
        struct trailing_bits<Integer, true> {
            constexpr int operator()(Integer const& integer) const noexcept
            {
                using unsigned_type = make_unsigned_t<Integer>;
                return countr_zero(static_cast<unsigned_type>(integer));
            }
        };
    }

    // count of the right redundant trailing bits
    template<class Integer>
    constexpr int trailing_bits(Integer const& value)
    {
        return value ? _numeric_impl::trailing_bits<Integer, is_signed<Integer>::value>()(value) : 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::used_digits

    template<typename Integer>
    constexpr int used_digits(Integer const& value, int radix = numeric_limits<Integer>::radix)
    {
        return _impl::used_digits_signed<is_signed<Integer>::value>{}(unwrap(value), radix);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::leading_bits

    template<class Integer>
    constexpr int leading_bits(Integer const& value)
    {
        return digits<Integer>::value-cnl::used_digits(value);
    }
}

#endif  // CNL_NUMERIC_H
