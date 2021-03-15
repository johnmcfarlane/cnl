
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt Boost.Multiprecision for use with @ref
/// cnl::scaled_integer

#if !defined(CNL_BOOST_MULTIPRECISION_H)
#define CNL_BOOST_MULTIPRECISION_H

#include "../_impl/numbers/set_signedness.h"
#include "../_impl/numbers/signedness.h"
#include "../_impl/scaled/power.h"
#include "../constant.h"
#include "../num_traits.h"
#include "../rounding_integer.h"

#include <boost/multiprecision/cpp_int.hpp>

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _bmp = boost::multiprecision;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // generic operators of boost::multiprecision::number

    namespace _impl {
        template<typename T>
        struct is_bmp_number : std::false_type {
        };

        template<class Backend>
        struct is_bmp_number<_bmp::number<Backend>> : std::true_type {
        };
    }

    template<class Backend>
    struct is_integer<_bmp::number<Backend>> : std::true_type {
    };

    template<
            unsigned MinBits,
            unsigned MaxBits,
            _bmp::cpp_integer_type SignType,
            _bmp::cpp_int_check_type Checked,
            class Allocator>
    struct is_integer<_bmp::cpp_int_backend<MinBits, MaxBits, SignType, Checked, Allocator>>
        : std::true_type {
    };

    template<
            int DestExponent, int SrcExponent, int Radix, unsigned NumBits,
            _bmp::cpp_integer_type SignType>
    struct custom_operator<
            _impl::convert_op,
            op_value<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, SignType>>, cnl::power<SrcExponent, Radix>>,
            op_value<_bmp::cpp_int_backend<NumBits, NumBits, SignType>, cnl::power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(
                _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, SignType>> const& input) const
        {
            // when converting *from* scaled_integer
            return input.backend();
        }
    };

    /// \cond
    template<
            int DestExponent, int SrcExponent, int Radix, unsigned NumBits,
            _bmp::cpp_integer_type SignType, typename Input>
    requires(!_impl::is_bmp_number<Input>::value) struct custom_operator<
            _impl::convert_op,
            op_value<Input, cnl::power<SrcExponent, Radix>>,
            op_value<_bmp::cpp_int_backend<NumBits, NumBits, SignType>, cnl::power<DestExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(Input const& input) const
                -> _bmp::cpp_int_backend<NumBits, NumBits, SignType>
        {
            return input;
        }
    };
    /// \endcond

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // type trait specializations of boost::multiprecision::number

    namespace _impl {
        template<typename T>
        struct is_boost_multiprecision : std::false_type {
        };

        template<typename Backend>
        struct is_boost_multiprecision<_bmp::number<Backend>> : std::true_type {
        };
    }
}

/// compositional numeric library, numbers header/namespace
namespace cnl::numbers {
    template<unsigned NumBits>
    struct signedness<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>>
        : std::true_type {
    };

    template<unsigned NumBits>
    struct signedness<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>>
        : std::false_type {
    };

    template<unsigned NumBits>
    struct set_signedness<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>,
            true> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct set_signedness<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>,
            true> {
        using type = _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct set_signedness<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>,
            false> {
        using type =
                _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;
    };

    template<unsigned NumBits>
    struct set_signedness<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>,
            false> {
        using type =
                _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;
    };
}

namespace cnl {
    template<unsigned NumBits>
    inline constexpr int
            digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>> = NumBits - 1;

    template<unsigned NumBits>
    inline constexpr int
            digits<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>> = NumBits;

    template<unsigned NumBits, int MinNumDigits>
    struct set_digits<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>,
            MinNumDigits> {
        using type = _bmp::number<
                _bmp::cpp_int_backend<MinNumDigits, MinNumDigits, _bmp::unsigned_magnitude>>;
    };

    template<unsigned NumBits, int MinNumDigits>
    struct set_digits<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>,
            MinNumDigits> {
        using type = _bmp::number<
                _bmp::cpp_int_backend<MinNumDigits + 1, MinNumDigits + 1, _bmp::signed_magnitude>>;
    };

    template<unsigned NumBits>
    struct to_rep<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>> {
        using _number_type =
                _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>;

        [[nodiscard]] constexpr auto& operator()(_number_type& n) const
        {
            return n;
        };

        [[nodiscard]] constexpr auto const& operator()(_number_type const& n) const
        {
            return n;
        };

        [[nodiscard]] constexpr auto operator()(_number_type&& n) const
        {
            return n;
        };
    };

    template<unsigned NumBits>
    struct to_rep<_bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>> {
        using _number_type =
                _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>;

        [[nodiscard]] constexpr auto& operator()(_number_type& n) const
        {
            return n;
        };

        [[nodiscard]] constexpr auto const& operator()(_number_type const& n) const
        {
            return n;
        };

        [[nodiscard]] constexpr auto operator()(_number_type&& n) const
        {
            return std::move(n);
        };
    };

    /// \cond
    template<unsigned NumBits, class Value>
    requires(!_impl::is_boost_multiprecision<Value>::value) struct from_value<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::signed_magnitude>>,
            Value> {
    private:
        static constexpr auto _bits = digits<Value> + 1;

    public:
        [[nodiscard]] constexpr auto operator()(Value const& value) const
                -> _bmp::number<_bmp::cpp_int_backend<_bits, _bits, _bmp::signed_magnitude>>
        {
            return value;
        }
    };

    template<unsigned NumBits, class Value>
    requires(!_impl::is_boost_multiprecision<Value>::value) struct from_value<
            _bmp::number<_bmp::cpp_int_backend<NumBits, NumBits, _bmp::unsigned_magnitude>>,
            Value> {
    private:
        static constexpr auto _bits = digits<Value>;

    public:
        [[nodiscard]] constexpr auto operator()(Value const& value) const
                -> _bmp::number<_bmp::cpp_int_backend<_bits, _bits, _bmp::unsigned_magnitude>>
        {
            return value;
        }
    };
    /// \endcond

    template<class LhsBackend, class RhsBackend>
    struct from_value<_bmp::number<LhsBackend>, _bmp::number<RhsBackend>> {
        [[nodiscard]] constexpr auto operator()(_bmp::number<RhsBackend> const& value) const
                -> _bmp::number<RhsBackend>
        {
            return value;
        }
    };

    template<int Digits, int Radix, class Backend>
    struct scale<Digits, Radix, _bmp::number<Backend>>
        : _impl::default_scale<Digits, Radix, _bmp::number<Backend>> {
    };

    template<class Backend>
    struct rounding<_bmp::number<Backend>> : std::type_identity<native_rounding_tag> {
    };

    template<class Backend>
    struct rep_of<_bmp::number<Backend>> : std::type_identity<Backend> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // aliases of boost::multiprecision types

    namespace _sized_integer_impl {
        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using backend = _bmp::cpp_int_backend<NumBits, NumBits, SignType>;

        template<unsigned NumBits, _bmp::cpp_integer_type SignType>
        using number = _bmp::number<_sized_integer_impl::backend<NumBits, SignType>, _bmp::et_off>;
    }

    /// \brief alias to a signed, fixed-size Boost.Multiprecision type
    /// suitable for specializing \ref cnl::scaled_integer
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::unsigned_multiprecision, cnl::multiprecision
    template<unsigned NumDigits = digits<int>>
    using signed_multiprecision =
            _sized_integer_impl::number<NumDigits + 1, _bmp::signed_magnitude>;

    /// \brief alias to an unsigned, fixed-size Boost.Multiprecision type
    /// suitable for specializing \ref cnl::scaled_integer
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::signed_multiprecision, cnl::multiprecision
    template<unsigned NumDigits = digits<unsigned>>
    using unsigned_multiprecision =
            _sized_integer_impl::number<NumDigits, _bmp::unsigned_magnitude>;

    /// \brief alias to a signed, fixed-size Boost.Multiprecision type
    /// suitable for specializing \ref cnl::scaled_integer
    ///
    /// \tparam NumDigits number of digits constituting the integer value (excluding sign bit)
    /// \sa cnl::signed_multiprecision, cnl::unsigned_multiprecision
    template<unsigned NumDigits = digits<int>>
    using multiprecision = signed_multiprecision<NumDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // boost::multiprecision bitwise shift operators

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator<<(
            _sized_integer_impl::number<NumBits, _bmp::signed_magnitude> const& lhs,
            constant<Value>)
    {
        return lhs << Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator<<(
            _sized_integer_impl::number<NumBits, _bmp::unsigned_magnitude> const& lhs,
            constant<Value>)
    {
        return lhs << Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator>>(
            _sized_integer_impl::number<NumBits, _bmp::unsigned_magnitude> const& lhs,
            constant<Value>)
    {
        return lhs >> Value;
    }

    template<unsigned NumBits, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator>>(
            _sized_integer_impl::number<NumBits, _bmp::signed_magnitude> const& lhs,
            constant<Value>)
    {
        return lhs >> Value;
    }
}

#endif  // CNL_BOOST_MULTIPRECISION_H
