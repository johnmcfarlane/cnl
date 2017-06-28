
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief functions that might belong in the <numeric> header

#ifndef SG14_NUMERIC_H
#define SG14_NUMERIC_H

#include <sg14/num_traits.h>

#include <limits.h>

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trailing_bits

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
        struct trailing_bits<Integer, _impl::enable_if_t<std::numeric_limits<Integer>::is_signed>> {
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
    // sg14::used_bits

    namespace _numeric_impl {
        template<class Integer>
        constexpr int used_bits_positive(Integer value, int mask_bits = sizeof(Integer)*CHAR_BIT/2)
        {
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
            constexpr _impl::enable_if_t<!std::numeric_limits<Integer>::is_signed, int> operator()(Integer value) const
            {
                return value ? used_bits_positive(value) : 0;
            }

            template<class Integer, class = _impl::enable_if_t<std::numeric_limits<Integer>::is_signed, int>>
            constexpr int operator()(Integer value) const
            {
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
    // sg14::leading_bits

#if !defined(_MSC_VER) && !defined(SG14_DISABLE_GCC_BUILTINS)
    constexpr int leading_bits(int value)
    {
        return (value>0)
               ? __builtin_clz(value)-1
               : digits<int>::value-used_bits(value);
    }
#endif

    template<class Integer>
    constexpr int leading_bits(const Integer& value)
    {
        return digits<Integer>::value-used_bits(value);
    }
}

#endif  // SG14_NUMERIC_H
