
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.SIMD for use with @ref sg14::fixed_point

#if !defined(SG14_BOOST_SIMD_H)
#define SG14_BOOST_SIMD_H 1

#include <sg14/fixed_point>

#include <boost/simd/pack.hpp>

namespace sg14 {
    template<class T, std::size_t N, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, const fixed_point<boost::simd::pack<T, N>, Exponent>& fp)
    {
        return out << static_cast<boost::simd::pack<long double, N>>(fp);
    }

    template<class T, std::size_t N, int Digits>
    struct set_digits<boost::simd::pack<T, N>, Digits> {
        using type = boost::simd::pack<set_digits_t<T, Digits>, N>;
    };

    template<class T, std::size_t N>
    struct make_signed<boost::simd::pack<T, N>> {
        using type = boost::simd::pack<make_signed_t<T>, N>;
    };

    template<class T, std::size_t N>
    struct make_unsigned<boost::simd::pack<T, N>> {
        using type = boost::simd::pack<make_unsigned_t<T>, N>;
    };
}

namespace std {
    template<class T, std::size_t N>
    struct numeric_limits<boost::simd::pack<T, N>> : numeric_limits<T> {};
}

#endif // SG14_BOOST_SIMD_H
