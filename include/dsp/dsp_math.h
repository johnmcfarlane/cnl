//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_MATH)
#define CNL_DSP_MATH

#include <cstdint>
#include "dsp_types.h"

namespace cnl
{
namespace dsp
{
namespace math
{

static double constexpr c_pi{3.141592653589793238462643383279502884197169399L};
static double constexpr c_pi_2 = c_pi / 2.0;
static double constexpr c_2_pi = c_pi * 2.0;

template<class T1, class T2>
class multiplies
{
public:
    T1 operator()(const T2& a, const T2& b) const;
};

template<class T1, class T2>
inline T1 multiplies<T1, T2>::operator()(const T2& a, const T2& b) const
{
    return static_cast<T1>(a * b);
}

template<class T>
class plus
{
public:
    T operator()(const T& a, const T& b) const;
};

template<class T> inline
T plus<T>::operator()(const T& a, const T& b) const
{
    return static_cast<T>(a + b);
}

template<class T>
class minus
{
public:
    T operator()(const T& a, const T& b) const;
};

template<class T> inline
T minus<T>::operator()(const T& a, const T& b) const
{
    return static_cast<T>(a - b);
}

template<class T>
class negate
{
public:
    T operator()(const T& a) const;
};

template<class T> inline
T negate<T>::operator()(const T& a) const
{
    return -a;
}

template <class T1, class T2>
class divides
{
public:
    T1 operator()(const T1& a, const T2& b) const;
};

template <class T1, class T2>
inline T1 divides<T1, T2>::operator()(const T1& a, const T2& b) const
{
    return static_cast<T1>(a / b);
}

template <>
inline q4_20 divides<q4_20,q4_20>::operator()(const q4_20& a, const q4_20& b) const
{
    return static_cast<q4_20>(cnl::make_fixed_point(cnl::make_fractional(a, b)));
}

#if defined(CNL_INT128_ENABLED)
template <>
inline q8_40 divides<q8_40,q8_40>::operator()(const q8_40& a, const q8_40& b) const
{
    return static_cast<q8_40>(cnl::make_fixed_point(cnl::make_fractional(a, b)));
}

template <>
inline q8_40 divides<q8_40,q4_20>::operator()(const q8_40& a, const q4_20& b) const
{
    return static_cast<q8_40>(cnl::make_fixed_point(cnl::make_fractional(a, b)));
}
#endif

template<typename T>
T sqrt(const T a)
{
    return std::sqrt(a);
}

template<class Rep, int Exponent>
inline cnl::fixed_point<Rep, Exponent> sqrt(const cnl::fixed_point<Rep, Exponent>& x)
{
    return cnl::sqrt(x);
}

template<typename T>
T floor(const T a)
{
    return std::floor(a);
}

template<class Rep, int Exponent>
inline cnl::fixed_point<Rep, Exponent> floor(const cnl::fixed_point<Rep, Exponent>& x)
{
    return cnl::fixed_point<Rep, Exponent>((cnl::to_rep(x)) >> -Exponent);
}

template<typename T>
T arithmetic_right_shift(const T a, unsigned int shift)
{
    return a / static_cast<T>(std::pow(2.0, shift));
}

template<class Rep, int Exponent>
inline cnl::fixed_point<Rep, Exponent> arithmetic_right_shift(const cnl::fixed_point<Rep, Exponent> a, unsigned int shift)
{
    return a >> shift;
}

// Default leading bits implementation taken by float and double
template<typename T>
int leading_bits(const T)
{
    return 0;
}

template<class Rep, int Exponent>
inline int leading_bits(const cnl::fixed_point<Rep, Exponent> a)
{
    return cnl::leading_bits(a);
}

} // namespace math
} // namespace dsp
} // namespace cnl


#endif //CNL_DSP_MATH
