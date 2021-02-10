
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing definitions equivalent to those in \verbatim<bit>\endverbatim

#if !defined(CNL_BIT_H)
#define CNL_BIT_H

#include "_impl/num_traits/digits.h"
#include "_impl/numbers/set_signedness.h"
#include "_impl/numbers/signedness.h"

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // loosely based on P0553R1
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0553r1.html

    namespace _bit_impl {
        template<typename T>
        CNL_NODISCARD constexpr auto is_integral_unsigned()
        {
            return numeric_limits<T>::is_integer && !numbers::signedness<T>::value;
        }

        template<typename T>
        CNL_NODISCARD constexpr auto is_integral_signed()
        {
            return numeric_limits<T>::is_integer && numbers::signedness<T>::value;
        }

        template<typename T>
        CNL_NODISCARD constexpr auto rotl(T x, unsigned int s, unsigned int width)
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");

            return static_cast<T>((x << (s % width)) | (x >> (width - (s % width))));
        }

        template<typename T>
        CNL_NODISCARD constexpr auto rotr(T x, unsigned int s, unsigned int width)
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");

            return static_cast<T>((x >> (s % width)) | (x << (width - (s % width))));
        }

        template<typename T>
        CNL_NODISCARD constexpr auto countr_zero(T x) -> int
        {
            static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

            return (x & 1) ? 0 : countr_zero<T>(static_cast<T>(x >> 1)) + 1;
        }
    }

    // rotl - rotate bits to the left
    template<typename T>
    CNL_NODISCARD constexpr auto rotl(T x, unsigned int s)
    {
        return _bit_impl::rotl(x, s, cnl::digits<T>);
    }

    // rotr - rotate bits to the right
    template<typename T>
    CNL_NODISCARD constexpr auto rotr(T x, unsigned int s)
    {
        return _bit_impl::rotr(x, s, cnl::digits<T>);
    }

    // countl_zero - count 0-bits to the left
    template<typename T>
    CNL_NODISCARD constexpr auto countl_zero(T x) -> int;

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr auto countl_zero(unsigned int x) -> int
    {
        return x ? __builtin_clz(x) : cnl::digits<unsigned int>;
    }

    template<>
    CNL_NODISCARD constexpr auto countl_zero(unsigned long x) -> int
    {
        return x ? __builtin_clzl(x) : cnl::digits<unsigned long>;
    }

    template<>
    CNL_NODISCARD constexpr auto countl_zero(unsigned long long x) -> int
    {
        return x ? __builtin_clzll(x) : cnl::digits<unsigned long long>;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr auto countl_zero(T x) -> int
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? countl_zero<T>(static_cast<T>(x >> 1)) - 1 : cnl::digits<T>;
    }

    // countl_one - count 1-bits to the left
    template<typename T>
    CNL_NODISCARD constexpr auto countl_one(T x) -> int;

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr auto countl_one(unsigned int x) -> int
    {
        return ~x ? __builtin_clz(~x) : cnl::digits<unsigned int>;
    }

    template<>
    CNL_NODISCARD constexpr auto countl_one(unsigned long x) -> int
    {
        return ~x ? __builtin_clzl(~x) : cnl::digits<unsigned long>;
    }

    template<>
    CNL_NODISCARD constexpr auto countl_one(unsigned long long x) -> int
    {
        return ~x ? __builtin_clzll(~x) : cnl::digits<unsigned long long>;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr auto countl_one(T x) -> int
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return (x & (T{1} << (cnl::digits<T> - 1)))
                     ? countl_one<T>(static_cast<T>(x << 1)) + 1
                     : 0;
    }

    // countr_zero - count 0-bits to the right
    template<typename T>
    CNL_NODISCARD constexpr auto countr_zero(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED) && !defined(__clang__)

    template<>
    CNL_NODISCARD constexpr auto countr_zero(unsigned int x)
    {
        return int{__builtin_ctz(x)};
    }

    template<>
    CNL_NODISCARD constexpr auto countr_zero(unsigned long x)
    {
        return x ? __builtin_ctzl(x) : cnl::digits<unsigned long>;
    }

    template<>
    CNL_NODISCARD constexpr auto countr_zero(unsigned long long x)
    {
        return x ? __builtin_ctzll(x) : cnl::digits<unsigned long long>;
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr auto countr_zero(T x)
    {
        return x ? _bit_impl::countr_zero(x) : cnl::digits<T>;
    }

    // countr_one - count 1-bits to the right
    template<typename T>
    CNL_NODISCARD constexpr auto countr_one(T x) -> int;

    template<>
    CNL_NODISCARD constexpr auto countr_one(unsigned int x) -> int
    {
        return countr_zero(~x);
    }

    template<typename T>
    CNL_NODISCARD constexpr auto countr_one(T x) -> int
    {
        return (x & T{1}) ? countr_one(x >> 1) + 1 : 0;
    }

    // popcount - count total number of 1-bits
    template<typename T>
    CNL_NODISCARD constexpr auto popcount(T x) -> int;

#if defined(CNL_GCC_INTRINSICS_ENABLED)

    template<>
    CNL_NODISCARD constexpr auto popcount(unsigned int x) -> int
    {
        return __builtin_popcount(x);
    }

    template<>
    CNL_NODISCARD constexpr auto popcount(unsigned long x) -> int
    {
        return __builtin_popcountl(x);
    }

    template<>
    CNL_NODISCARD constexpr auto popcount(unsigned long long x) -> int
    {
        return __builtin_popcountll(x);
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr auto popcount(T x) -> int
    {
        return x ? popcount(x & (x - 1)) + 1 : 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // loosely based on P0556R1
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0556r1.html

    // ispow2
    template<class T>
    CNL_NODISCARD constexpr auto ispow2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x && !(x & (x - 1));
    }

    // ceil2 - lowest power of 2 no less than x
    template<class T>
    CNL_NODISCARD constexpr auto ceil2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? static_cast<T>(T{1} << (digits<T> - countl_zero(T(x - T(1))))) : T{0};
    }

    // floor2 - greatest power of 2 no greater than x
    template<class T>
    CNL_NODISCARD constexpr auto floor2(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return x ? static_cast<T>(T{1} << (digits<T> - 1 - countl_zero(x))) : T{0};
    }

    // log2p1 - one plus log2(x)
    template<class T>
    CNL_NODISCARD constexpr auto log2p1(T x)
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");

        return digits<T> - countl_zero(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // additional bit-level functions needed by CNL

    // countl_rsb - count redundant sign bits to the left
    template<typename T>
    CNL_NODISCARD constexpr auto countl_rsb(T x);

#if defined(CNL_GCC_INTRINSICS_ENABLED) && !defined(__clang__)

    template<>
    CNL_NODISCARD constexpr auto countl_rsb(int x)
    {
        return __builtin_clrsb(x);
    }

    template<>
    CNL_NODISCARD constexpr auto countl_rsb(long x)
    {
        return __builtin_clrsbl(x);
    }

    template<>
    CNL_NODISCARD constexpr auto countl_rsb(long long x)
    {
        return __builtin_clrsbll(x);
    }

#endif

    template<typename T>
    CNL_NODISCARD constexpr auto countl_rsb(T x)
    {
        static_assert(_bit_impl::is_integral_signed<T>(), "T must be signed integer");

        using unsigned_type = numbers::set_signedness_t<T, false>;

        return ((x < 0) ? countl_one(static_cast<unsigned_type>(x))
                        : countl_zero(static_cast<unsigned_type>(x)))
             - 1;
    }

    // countl_rb - count redundant bits to the left
    namespace _bit_impl {
        template<bool IsSigned>
        struct countl_rb {
            template<class Integer>
            CNL_NODISCARD constexpr auto operator()(Integer const& value) const
            {
                static_assert(
                        _bit_impl::is_integral_unsigned<Integer>(), "T must be unsigned integer");

                return countl_zero(value);
            }
        };

        template<>
        struct countl_rb<true> {
            template<class Integer>
            CNL_NODISCARD constexpr auto operator()(Integer const& value) const
            {
                static_assert(_bit_impl::is_integral_signed<Integer>(), "T must be signed integer");

                return countl_rsb(value);
            }
        };
    }

    template<typename T>
    CNL_NODISCARD constexpr auto countl_rb(T x)  // NOLINT(misc-unused-parameters)
    {
        return _bit_impl::countl_rb<numbers::signedness<T>::value>()(x);
    }

    // countr_used - count total used bits to the right
    template<typename T>
    CNL_NODISCARD constexpr auto countr_used(T x)
    {
        return digits<T> - countl_rb(x);
    }

}

#endif  // CNL_BIT_H
