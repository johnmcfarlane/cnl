/*
   Reference implementation of the  <math> header for https://wg21.link/P0631.

   The following concept needs to be present in the <concepts> header:

   template<class T> concept FloatingPoint = is_floating_point_v<T>;

   Copyright © 2019 Lev Minkovsky

   MIT License

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#if __has_include(<concepts>)
#include <concepts>
#else
#include <type_traits>
template<class T> concept FloatingPoint = std::is_floating_point_v<T>;
#endif

namespace cnl {
    namespace math {

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

        template <FloatingPoint T> inline constexpr T e_v<T>          = T(2.718281828459045235360287471352662498L);
        template <FloatingPoint T> inline constexpr T log2e_v<T>      = T(1.442695040888963407359924681001892137L);
        template <FloatingPoint T> inline constexpr T log10e_v<T>     = T(0.434294481903251827651128918916605082L);
        template <FloatingPoint T> inline constexpr T pi_v<T>         = T(3.141592653589793238462643383279502884L);
        template <FloatingPoint T> inline constexpr T inv_pi_v<T>     = T(0.318309886183790671537767526745028724L);
        template <FloatingPoint T> inline constexpr T inv_sqrtpi_v<T> = T(0.564189583547756286948079451560772586L);
        template <FloatingPoint T> inline constexpr T ln2_v<T>        = T(0.693147180559945309417232121458176568L);
        template <FloatingPoint T> inline constexpr T ln10_v<T>       = T(2.302585092994045684017991454684364208L);
        template <FloatingPoint T> inline constexpr T sqrt2_v<T>      = T(1.414213562373095048801688724209698078L);
        template <FloatingPoint T> inline constexpr T sqrt3_v<T>      = T(1.732050807568877293527446341505872366L);
        template <FloatingPoint T> inline constexpr T inv_sqrt3_v<T>  = T(0.577350269189625764509148780501957456L);
        template <FloatingPoint T> inline constexpr T egamma_v<T>     = T(0.577215664901532860606512090082402431L);
        template <FloatingPoint T> inline constexpr T phi_v<T>        = T(1.618033988749894848204586834365638117L);

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
