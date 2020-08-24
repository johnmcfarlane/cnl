
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBERS_17_H)
#define CNL_IMPL_NUMBERS_17_H

#include "config.h"

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
        CNL_NODISCARD constexpr T deleted_fn() = delete;
    }

    namespace numbers {
        /// partial implementation of [P0631](http://wg21.link/p0631)

        // values taken from c++ (GCC) 7.2.0 implementation of math.h; disclaimer: may be a bit or two off

        ////////////////////////////////////////////////////////////////////////////////
        // primary templates

        /// approximation of [e_v](https://en.wikipedia.org/wiki/E_(mathematical_constant));
        /// primary variable template
        template<typename T> inline constexpr T e_v{_impl::deleted_fn<T>()};

        /// approximation of the [binary logarithm](https://en.wikipedia.org/wiki/Binary_logarithm) of e_v, `log2(e_v)`;
        /// primary variable template
        template<typename T> inline constexpr T log2e_v{_impl::deleted_fn<T>()};

        /// approximation of the [decimal logarithm](https://en.wikipedia.org/wiki/Binary_logarithm) of e_v, `log10(e_v)`;
        /// primary variable template
        template<typename T> inline constexpr T log10e_v{_impl::deleted_fn<T>()};

        /// approximation of [pi_v](https://en.wikipedia.org/wiki/Pi);
        /// primary variable template
        template<typename T> inline constexpr T pi_v{_impl::deleted_fn<T>()};

        /// approximation of `1/pi_v`;
        /// primary variable template
        template<typename T> inline constexpr T inv_pi_v{_impl::deleted_fn<T>()};

        /// approximation of `1/sqrt(pi_v)`;
        /// primary variable template
        template<typename T> inline constexpr T inv_sqrtpi_v{_impl::deleted_fn<T>()};

        /// approximation of the [natural logarithm](https://en.wikipedia.org/wiki/Natural_logarithm) of 2, `ln(2)`;
        /// primary variable template
        template<typename T> inline constexpr T ln2_v{_impl::deleted_fn<T>()};

        /// approximation of the [natural logarithm](https://en.wikipedia.org/wiki/Natural_logarithm) of 10, `ln(10)`;
        /// primary variable template
        template<typename T> inline constexpr T ln10_v{_impl::deleted_fn<T>()};

        /// approximation of the square root of 2, `sqrt(2)`;
        /// primary variable template
        template<typename T> inline constexpr T sqrt2_v{_impl::deleted_fn<T>()};

        /// approximation of the square root of 3, `sqrt(3)`;
        /// primary variable template
        template<typename T> inline constexpr T sqrt3_v{_impl::deleted_fn<T>()};

        /// approximation of the inverse of the square root of 3, `1/sqrt(3)`;
        /// primary variable template
        template<typename T> inline constexpr T inv_sqrt3_v{_impl::deleted_fn<T>()};

        /// approximation of the [Euler–Mascheroni constant](https://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant);
        /// primary variable template
        template<typename T> inline constexpr T egamma_v{_impl::deleted_fn<T>()};

        /// approximation of the [golden ratio](https://en.wikipedia.org/wiki/Golden_ratio);
        /// primary variable template
        template<typename T> inline constexpr T phi_v{_impl::deleted_fn<T>()};

        ////////////////////////////////////////////////////////////////////////////////
        // long double specializations

        /// specialization of \ref cnl::e_v for `long double`
        template<> inline constexpr long double e_v<long double>{2.718281828459045235360287471352662498L};

        /// specialization of \ref cnl::log2e_v for `long double`
        template<> inline constexpr long double log2e_v<long double>{1.442695040888963407359924681001892137L};

        /// specialization of \ref cnl::log10e_v for `long double`
        template<> inline constexpr long double log10e_v<long double>{0.434294481903251827651128918916605082L};

        /// specialization of \ref cnl::pi_v for `long double`
        template<> inline constexpr long double pi_v<long double>{3.141592653589793238462643383279502884L};

        /// specialization of \ref cnl::inv_pi_v for `long double`
        template<> inline constexpr long double inv_pi_v<long double>{0.318309886183790671537767526745028724L};

        /// specialization of \ref cnl::inv_sqrtpi_v for `long double`
        template<> inline constexpr long double inv_sqrtpi_v<long double>{
                0.564189583547756286948079451560772585844050629329L};

        /// specialization of \ref cnl::ln2_v for `long double`
        template<> inline constexpr long double ln2_v<long double>{0.693147180559945309417232121458176568L};

        /// specialization of \ref cnl::ln10_v for `long double`
        template<> inline constexpr long double ln10_v<long double>{2.302585092994045684017991454684364208L};

        /// specialization of \ref cnl::sqrt2_v for `long double`
        template<> inline constexpr long double sqrt2_v<long double>{1.414213562373095048801688724209698079L};

        /// specialization of \ref cnl::sqrt3_v for `long double`
        template<> inline constexpr long double sqrt3_v<long double>{
                1.73205080756887729352744634150587236694280525381038062805580L};

        /// specialization of \ref cnl::inv_sqrt3_v for `long double`
        template<> inline constexpr long double inv_sqrt3_v<long double>{
                0.57735026918962576450914878050195745564760175127013L};

        /// specialization of \ref cnl::egamma_v for `long double`
        template<> inline constexpr long double egamma_v<long double>{0.5772156649015328606065120900824024L};

        /// specialization of \ref cnl::phi_v for `long double`
        template<> inline constexpr long double phi_v<long double>{1.6180339887498948482045868343656381L};

        ////////////////////////////////////////////////////////////////////////////////
        // double specializations

        /// specialization of \ref cnl::e_v for `double`
        template<> inline constexpr double e_v<double>{2.7182818284590452354};

        /// specialization of \ref cnl::log2e_v for `double`
        template<> inline constexpr double log2e_v<double>{1.4426950408889634074};

        /// specialization of \ref cnl::log10e_v for `double`
        template<> inline constexpr double log10e_v<double>{0.43429448190325182765};

        /// specialization of \ref cnl::pi_v for `double`
        template<> inline constexpr double pi_v<double>{3.14159265358979323846};

        /// specialization of \ref cnl::inv_pi_v for `double`
        template<> inline constexpr double inv_pi_v<double>{0.31830988618379067154};

        /// specialization of \ref cnl::inv_sqrtpi_v for `double`
        template<> inline constexpr double inv_sqrtpi_v<double>{0.564189583547756286948079451560772585844050629329};

        /// specialization of \ref cnl::ln2_v for `double`
        template<> inline constexpr double ln2_v<double>{0.69314718055994530942};

        /// specialization of \ref cnl::ln10_v for `double`
        template<> inline constexpr double ln10_v<double>{2.30258509299404568402};

        /// specialization of \ref cnl::sqrt2_v for `double`
        template<> inline constexpr double sqrt2_v<double>{1.41421356237309504880};

        /// specialization of \ref cnl::sqrt3_v for `double`
        template<> inline constexpr double sqrt3_v<double>{
                1.73205080756887729352744634150587236694280525381038062805580};

        /// specialization of \ref cnl::inv_sqrt3_v for `double`
        template<> inline constexpr double inv_sqrt3_v<double>{0.57735026918962576450914878050195745564760175127013};

        /// specialization of \ref cnl::egamma_v for `double`
        template<> inline constexpr double egamma_v<double>{0.5772156649015328606065120900824024};

        /// specialization of \ref cnl::phi_v for `double`
        template<> inline constexpr double phi_v<double>{1.6180339887498948482045868343656381};

        ////////////////////////////////////////////////////////////////////////////////
        // float specializations

        /// specialization of \ref cnl::e_v for `float`
        template<> inline constexpr float e_v<float>{2.7182818284590452354f};

        /// specialization of \ref cnl::log2e_v for `float`
        template<> inline constexpr float log2e_v<float>{1.4426950408889634074f};

        /// specialization of \ref cnl::log10e_v for `float`
        template<> inline constexpr float log10e_v<float>{0.43429448190325182765f};

        /// specialization of \ref cnl::pi_v for `float`
        template<> inline constexpr float pi_v<float>{3.14159265358979323846f};

        /// specialization of \ref cnl::inv_pi_v for `float`
        template<> inline constexpr float inv_pi_v<float>{0.31830988618379067154f};

        /// specialization of \ref cnl::inv_sqrtpi_v for `float`
        template<> inline constexpr float inv_sqrtpi_v<float>{0.564189583547756286948079451560772585844050629329f};

        /// specialization of \ref cnl::ln2_v for `float`
        template<> inline constexpr float ln2_v<float>{0.69314718055994530942f};

        /// specialization of \ref cnl::ln10_v for `float`
        template<> inline constexpr float ln10_v<float>{2.30258509299404568402f};

        /// specialization of \ref cnl::sqrt2_v for `float`
        template<> inline constexpr float sqrt2_v<float>{1.41421356237309504880f};

        /// specialization of \ref cnl::sqrt3_v for `float`
        template<> inline constexpr float sqrt3_v<float>{
                1.73205080756887729352744634150587236694280525381038062805580f};

        /// specialization of \ref cnl::inv_sqrt3_v for `float`
        template<> inline constexpr float inv_sqrt3_v<float>{0.57735026918962576450914878050195745564760175127013f};

        /// specialization of \ref cnl::egamma_v for `float`
        template<> inline constexpr float egamma_v<float>{0.5772156649015328606065120900824024f};

        /// specialization of \ref cnl::phi_v for `float`
        template<> inline constexpr float phi_v<float>{1.6180339887498948482045868343656381f};
    }
#endif  // defined(__cpp_inline_variables)
}

#endif
