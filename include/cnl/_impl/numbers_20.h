
//          Copyright Lev Minkovsky 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Reference implementation of the \verbatim<math>\endverbatim header for https://wg21.link/P0631.
///
/// The following concept needs to be present in the concepts header:
///
///     template<class T> concept floating_point = is_floating_point_v<T>;

#if !defined(CNL_IMPL_NUMBERS_20_H)
#define CNL_IMPL_NUMBERS_20_H

#if __has_include(<concepts>)
#include <concepts>
#endif
#include <type_traits>

namespace cnl {
    namespace _impl {
#if __has_include(<concepts>)
        using std::floating_point;
#else
        template<class T> concept floating_point = std::is_floating_point_v<T>;
#endif
    }

    namespace numbers {

        template<typename> inline constexpr bool __always_false = false;

        template<typename T> inline constexpr T __invalidTemplate() {
            static_assert(__always_false<T>, "This template needs to be specialized."); return T();
        }

        template <typename T> inline constexpr T e_v          = __invalidTemplate<T>();
        template <typename T> inline constexpr T log2e_v      = __invalidTemplate<T>();
        template <typename T> inline constexpr T log10e_v     = __invalidTemplate<T>();
        template <typename T> inline constexpr T pi_v         = __invalidTemplate<T>();
        template <typename T> inline constexpr T inv_pi_v     = __invalidTemplate<T>();
        template <typename T> inline constexpr T inv_sqrtpi_v = __invalidTemplate<T>();
        template <typename T> inline constexpr T ln2_v        = __invalidTemplate<T>();
        template <typename T> inline constexpr T ln10_v       = __invalidTemplate<T>();
        template <typename T> inline constexpr T sqrt2_v      = __invalidTemplate<T>();
        template <typename T> inline constexpr T sqrt3_v      = __invalidTemplate<T>();
        template <typename T> inline constexpr T inv_sqrt3_v  = __invalidTemplate<T>();
        template <typename T> inline constexpr T egamma_v     = __invalidTemplate<T>();
        template <typename T> inline constexpr T phi_v        = __invalidTemplate<T>();

        template <_impl::floating_point T> inline constexpr T e_v<T>          = T(2.718281828459045235360287471352662498L);
        template <_impl::floating_point T> inline constexpr T log2e_v<T>      = T(1.442695040888963407359924681001892137L);
        template <_impl::floating_point T> inline constexpr T log10e_v<T>     = T(0.434294481903251827651128918916605082L);
        template <_impl::floating_point T> inline constexpr T pi_v<T>         = T(3.141592653589793238462643383279502884L);
        template <_impl::floating_point T> inline constexpr T inv_pi_v<T>     = T(0.318309886183790671537767526745028724L);
        template <_impl::floating_point T> inline constexpr T inv_sqrtpi_v<T> = T(0.564189583547756286948079451560772586L);
        template <_impl::floating_point T> inline constexpr T ln2_v<T>        = T(0.693147180559945309417232121458176568L);
        template <_impl::floating_point T> inline constexpr T ln10_v<T>       = T(2.302585092994045684017991454684364208L);
        template <_impl::floating_point T> inline constexpr T sqrt2_v<T>      = T(1.414213562373095048801688724209698078L);
        template <_impl::floating_point T> inline constexpr T sqrt3_v<T>      = T(1.732050807568877293527446341505872366L);
        template <_impl::floating_point T> inline constexpr T inv_sqrt3_v<T>  = T(0.577350269189625764509148780501957456L);
        template <_impl::floating_point T> inline constexpr T egamma_v<T>     = T(0.577215664901532860606512090082402431L);
        template <_impl::floating_point T> inline constexpr T phi_v<T>        = T(1.618033988749894848204586834365638117L);

        inline constexpr double e          = e_v<double>;
        inline constexpr double log2e      = log2e_v<double>;
        inline constexpr double log10e     = log10e_v<double>;
        inline constexpr double pi         = pi_v<double>;
        inline constexpr double inv_pi     = inv_pi_v<double>;
        inline constexpr double inv_sqrtpi = inv_sqrtpi_v<double>;
        inline constexpr double ln2        = ln2_v<double>;
        inline constexpr double ln10       = ln10_v<double>;
        inline constexpr double sqrt2      = sqrt2_v<double>;
        inline constexpr double sqrt3      = sqrt3_v<double>;
        inline constexpr double inv_sqrt3  = inv_sqrt3_v<double>;
        inline constexpr double egamma     = egamma_v<double>;
        inline constexpr double phi        = phi_v<double>;

    }
}

#endif
