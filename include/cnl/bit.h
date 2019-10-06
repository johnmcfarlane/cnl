
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing definitions equivalent to those in \verbatim<bit>\endverbatim

#if !defined(CNL_BIT_H)
#define CNL_BIT_H

#include "_impl/num_traits/digits.h"
#include "_impl/type_traits/is_signed.h"
#include "_impl/type_traits/remove_signedness.h"
#include "cstdint.h"

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // loosely based on P0553R1
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0553r1.html

    namespace _bit_impl {
        template<typename T>
        CNL_NODISCARD constexpr bool is_integral_unsigned()
        {
            return numeric_limits<T>::is_integer && !is_signed<T>::value;
        }

        template<typename T>
        CNL_NODISCARD constexpr bool is_integral_signed()
        {
            return numeric_limits<T>::is_integer && is_signed<T>::value;
        }

        template<typename T>
        CNL_NODISCARD constexpr T rotl(T x, unsigned int s, unsigned int width)
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");

            return static_cast<T>((x << (s%width)) | (x >> (width-(s%width))));
        }

        template<typename T>
        CNL_NODISCARD constexpr T rotr(T x, unsigned int s, unsigned int width)
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");

            return static_cast<T>((x >> (s%width)) | (x << (width-(s%width))));
        }

        template<typename T>
        CNL_NODISCARD constexpr int countr_zero(T x)
        {
            static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

            return (x & 1) ? 0 : countr_zero<T>(static_cast<T>(x >> 1))+1;
        }
    }

    // rotl - rotate bits to the left
    template<typename T>
    CNL_NODISCARD constexpr T rotl(T x, unsigned int s)
    {
        return _bit_impl::rotl(x, s, cnl::digits<T>::value);
    }

    // rotr - rotate bits to the right
    template<typename T>
    CNL_NODISCARD constexpr T rotr(T x, unsigned int s)
    {
        return _bit_impl::rotr(x, s, cnl::digits<T>::value);
    }

    // countl_zero - count 0-bits to the left
    template<typename T>
    CNL_NODISCARD constexpr int countl_zero(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr int countl_zero(unsigned int x)
    {
        return x ? __builtin_clz(x) : cnl::digits<unsigned int>::value;
    }

    template<>
    CNL_NODISCARD constexpr int countl_zero(unsigned long x)
    {
        return x ? __builtin_clzl(x) : cnl::digits<unsigned long>::value;
    }

    template<>
    CNL_NODISCARD constexpr int countl_zero(unsigned long long x)
    {
        return x ? __builtin_clzll(x) : cnl::digits<unsigned long long>::value;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr int countl_zero(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? countl_zero<T>(static_cast<T>(x >> 1))-1 : cnl::digits<T>::value;
    }

    // countl_one - count 1-bits to the left
    template<typename T>
    CNL_NODISCARD constexpr int countl_one(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr int countl_one(unsigned int x)
    {
        return ~x ? __builtin_clz(~x) : cnl::digits<unsigned int>::value;
    }

    template<>
    CNL_NODISCARD constexpr int countl_one(unsigned long x)
    {
        return ~x ? __builtin_clzl(~x) : cnl::digits<unsigned long>::value;
    }

    template<>
    CNL_NODISCARD constexpr int countl_one(unsigned long long x)
    {
        return ~x ? __builtin_clzll(~x) : cnl::digits<unsigned long long>::value;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr int countl_one(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return (x & (T{1} << (cnl::digits<T>::value-1))) ? countl_one<T>(static_cast<T>(x << 1))+1 : 0;
    }

    // countr_zero - count 0-bits to the right
    template<typename T>
    CNL_NODISCARD constexpr int countr_zero(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED) && !defined(__clang__)

    template<>
    CNL_NODISCARD constexpr int countr_zero(unsigned int x)
    {
        return __builtin_ctz(x);
    }

    template<>
    CNL_NODISCARD constexpr int countr_zero(unsigned long x)
    {
        return x ? __builtin_ctzl(x) : cnl::digits<unsigned long>::value;
    }

    template<>
    CNL_NODISCARD constexpr int countr_zero(unsigned long long x)
    {
        return x ? __builtin_ctzll(x) : cnl::digits<unsigned long long>::value;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr int countr_zero(T x)
    {
        return x ? _bit_impl::countr_zero(x) : cnl::digits<T>::value;
    }

    // countr_one - count 1-bits to the right
    template<typename T>
    CNL_NODISCARD constexpr int countr_one(T x);

    template<>
    CNL_NODISCARD constexpr int countr_one(unsigned int x)
    {
        return countr_zero(~x);
    }

    template<typename T>
    CNL_NODISCARD constexpr int countr_one(T x)
    {
        return (x & T{1}) ? countr_one(x >> 1)+1 : 0;
    }

    // popcount - count total number of 1-bits
    template<typename T>
    CNL_NODISCARD constexpr int popcount(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr int popcount(unsigned int x)
    {
        return __builtin_popcount(x);
    }

    template<>
    CNL_NODISCARD constexpr int popcount(unsigned long x)
    {
        return __builtin_popcountl(x);
    }

    template<>
    CNL_NODISCARD constexpr int popcount(unsigned long long x)
    {
        return __builtin_popcountll(x);
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr int popcount(T x)
    {
        return x ? popcount(x & (x-1))+1 : 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // loosely based on P0556R1
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0556r1.html

    // ispow2
    template<class T>
    CNL_NODISCARD constexpr bool ispow2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x && !(x & (x-1));
    }

    // ceil2 - lowest power of 2 no less than x
    template<class T>
    CNL_NODISCARD constexpr T ceil2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? static_cast<T>(T{1} << (digits<T>::value-countl_zero(T(x-T(1))))) : T{0};
    }

    // floor2 - greatest power of 2 no greater than x
    template<class T>
    CNL_NODISCARD constexpr T floor2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? static_cast<T>(T{1} << (digits<T>::value-1-countl_zero(x))) : T{0};
    }

    // log2p1 - one plus log2(x)
    template<class T>
    CNL_NODISCARD constexpr int log2p1(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return digits<T>::value-countl_zero(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // additional bit-level functions needed by CNL

    // countl_rsb - count redundant sign bits to the left
    template<typename T>
    CNL_NODISCARD constexpr int countl_rsb(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED) && !defined(__clang__)

    template<>
    CNL_NODISCARD constexpr int countl_rsb(int x)
    {
        return __builtin_clrsb(x);
    }

    template<>
    CNL_NODISCARD constexpr int countl_rsb(long x)
    {
        return __builtin_clrsbl(x);
    }

    template<>
    CNL_NODISCARD constexpr int countl_rsb(long long x)
    {
        return __builtin_clrsbll(x);
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr int countl_rsb(T x)
    {
        static_assert(_bit_impl::is_integral_signed<T>(), "T must be signed integer");

        using unsigned_type = typename remove_signedness<T>::type;

        return ((x<0)
                ? countl_one(static_cast<unsigned_type>(x))
                : countl_zero(static_cast<unsigned_type>(x))) - 1;
    }

    // countl_rb - count redundant bits to the left
    namespace _bit_impl {
        template<bool IsSigned>
        struct countl_rb {
            template<class Integer>
            CNL_NODISCARD constexpr int operator()(Integer const& value) const
            {
                static_assert(_bit_impl::is_integral_unsigned<Integer>(), "T must be unsigned integer");

                return countl_zero(value);
            }
        };

        template<>
        struct countl_rb<true> {
            template<class Integer>
            CNL_NODISCARD constexpr int operator()(Integer const& value) const
            {
                static_assert(_bit_impl::is_integral_signed<Integer>(), "T must be signed integer");

                return countl_rsb(value);
            }
        };
    }

    template<typename T>
    CNL_NODISCARD constexpr int countl_rb(T x)  // NOLINT(misc-unused-parameters)
    {
        return _bit_impl::countl_rb<is_signed<T>::value>()(x);
    }

    // countr_used - count total used bits to the right
    template<typename T>
    CNL_NODISCARD constexpr int countr_used(T x)
    {
        return digits<T>::value - countl_rb(x);
    }

}

#endif //CNL_BIT_H
