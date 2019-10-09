
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.SIMD for use with @ref cnl::scaled_integer

#if !defined(CNL_BOOST_SIMD_H)
#define CNL_BOOST_SIMD_H

#include "../scaled_integer.h"

#include <boost/simd/pack.hpp>

namespace cnl {
    template<class T, std::size_t N, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, scaled_integer<boost::simd::pack<T, N>, power<Exponent>> const& fp)
    {
        return out << static_cast<boost::simd::pack<long double, N>>(fp);
    }

    template<class T, std::size_t N>
    struct digits<boost::simd::pack<T, N>> : digits<T> {
    };

    template<class T, std::size_t N, int Digits>
    struct set_digits<boost::simd::pack<T, N>, Digits> {
        using type = boost::simd::pack<set_digits_t<T, Digits>, N>;
    };

    template<class T, std::size_t N>
    struct to_rep<boost::simd::pack<T, N>> : _impl::default_to_rep<boost::simd::pack<T, N>> {
    };

    template<int Digits, int Radix, class T, std::size_t N>
    struct scale<Digits, Radix, boost::simd::pack<T, N>> : _impl::default_scale<Digits, Radix, boost::simd::pack<T, N>> {
    };

    template<class T, std::size_t N>
    struct add_signedness<boost::simd::pack<T, N>> {
        using type = boost::simd::pack<add_signedness_t<T>, N>;
    };

    template<class T, std::size_t N>
    struct is_signed<boost::simd::pack<T, N>> : is_signed<T> {
    };

    template<class T, std::size_t N>
    struct remove_signedness<boost::simd::pack<T, N>> {
        using type = boost::simd::pack<remove_signedness_t<T>, N>;
    };
}

namespace std {
    template<class T, std::size_t N>
    struct numeric_limits<boost::simd::pack<T, N>> : numeric_limits<T> {};
}

#endif  // CNL_BOOST_SIMD_H
