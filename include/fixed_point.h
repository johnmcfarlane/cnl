#if ! defined(_SG14_FIXED_POINT)
#define _SG14_FIXED_POINT 1

#include <climits>
#include <cinttypes>
#include <type_traits>

#if defined(__clang__) || defined(__GNUG__)
// sg14::float_point only fully supports 64-bit types with the help of 128-bit ints.
// Clang and GCC use (__int128) and (unsigned __int128) for 128-bit ints.
#define _SG14_FIXED_POINT_128
#endif

namespace sg14
{
	////////////////////////////////////////////////////////////////////////////////
	// general-purpose _impl definitions

	namespace _impl
	{
		////////////////////////////////////////////////////////////////////////////////
		// num_bits

		template <typename T>
		constexpr int num_bits()
		{
			return sizeof(T) * CHAR_BIT;
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::get_int_t

		template <bool Signed, int NumBytes>
		struct get_int;

		template <bool Signed, int NumBytes>
		using get_int_t = typename get_int<Signed, NumBytes>::type;

		// specializations
		template <> struct get_int<false, 1> { using type = std::uint8_t; };
		template <> struct get_int<true, 1> { using type = std::int8_t; };
		template <> struct get_int<false, 2> { using type = std::uint16_t; };
		template <> struct get_int<true, 2> { using type = std::int16_t; };
		template <> struct get_int<false, 4> { using type = std::uint32_t; };
		template <> struct get_int<true, 4> { using type = std::int32_t; };
		template <> struct get_int<false, 8> { using type = std::uint64_t; };
		template <> struct get_int<true, 8> { using type = std::int64_t; };
#if defined(_SG14_FIXED_POINT_128)
		template <> struct get_int<false, 16> { using type = unsigned __int128; };
		template <> struct get_int<true, 16> { using type = __int128; };
#endif

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::get_float_t

		template <int NumBytes>
		struct get_float;

		template <int NumBytes>
		using get_float_t = typename get_float<NumBytes>::type;

		// specializations
		template <> struct get_float<1> { using type = float; };
		template <> struct get_float<2> { using type = float; };
		template <> struct get_float<4> { using type = float; };
		template <> struct get_float<8> { using type = double; };
		template <> struct get_float<16> { using type = long double; };

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_integral

		template <class T>
		struct is_integral;

		// exception to std::is_integral as fixed_point::operator bool is a special case
		template <>
		struct is_integral<bool> : std::false_type { };

#if defined(_SG14_FIXED_POINT_128)
		// addresses https://llvm.org/bugs/show_bug.cgi?id=23156
		template <>
		struct is_integral<__int128> : std::true_type { };

		template <>
		struct is_integral<unsigned __int128> : std::true_type { };
#endif

		template <class T>
		struct is_integral : std::is_integral<T> { };

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_signed

		template <class T>
		struct is_signed
		{
			static_assert(is_integral<T>::value, "sg14::_impl::is_signed only intended for use with integral types");
			static constexpr bool value = std::is_same<typename get_int<true, sizeof(T)>::type, T>::value;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_unsigned

		template <class T>
		struct is_unsigned
		{
			static_assert(is_integral<T>::value, "sg14::_impl::is_unsigned only intended for use with integral types");
			static constexpr bool value = std::is_same<typename get_int<false, sizeof(T)>::type, T>::value;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::make_signed

		template <class T>
		struct make_signed
		{
			using type = typename get_int<true, sizeof(T)>::type;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::make_unsigned

		template <class T>
		struct make_unsigned
		{
			using type = typename get_int<false, sizeof(T)>::type;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::next_size_t

		// given an integral type, IntType,
		// provides the integral type of the equivalent type with twice the size
		template <class IntType>
		using next_size_t = get_int_t<_impl::is_signed<IntType>::value, sizeof(IntType) * 2>;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::previous_size_t

		// given an integral type, IntType,
		// provides the integral type of the equivalent type with half the size
		template <class IntType>
		using previous_size_t = get_int_t<_impl::is_signed<IntType>::value, sizeof(IntType) / 2>;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::shift_left and sg14::_impl::shift_right

		// performs a shift operation by a fixed number of bits avoiding two pitfalls:
		// 1) shifting by a negative amount causes undefined behavior
		// 2) converting between integer types of different sizes can lose significant bits during shift right

		// Exponent == 0
		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				(Exponent == 0),
				int>::type Dummy = 0>
		constexpr Output shift_left(Input i) noexcept
		{
			static_assert(_impl::is_integral<Input>::value, "Input must be integral type");
			static_assert(_impl::is_integral<Output>::value, "Output must be integral type");

			// cast only
			return static_cast<Output>(i);
		}

		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				Exponent == 0,
				int>::type Dummy = 0>
		constexpr Output shift_right(Input i) noexcept
		{
			static_assert(_impl::is_integral<Input>::value, "Input must be integral type");
			static_assert(_impl::is_integral<Output>::value, "Output must be integral type");

			// cast only
			return static_cast<Output>(i);
		}

		// sizeof(Input) > sizeof(Output)
		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				!(Exponent <= 0) && sizeof(Output) <= sizeof(Input) && _impl::is_unsigned<Input>::value,
				int>::type Dummy = 0>
		constexpr Output shift_left(Input i) noexcept
		{
			return shift_left<0, Output, Input>(i) << Exponent;
		}

		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				!(Exponent <= 0) && sizeof(Output) <= sizeof(Input),
				int>::type Dummy = 0>
		constexpr Output shift_right(Input i) noexcept
		{
			return shift_right<0, Output, Input>(i >> Exponent);
		}

		// sizeof(Input) <= sizeof(Output)
		template <
			int Exponent, 
			class Output, 
			class Input, 
			typename std::enable_if<
				!(Exponent <= 0) && !(sizeof(Output) <= sizeof(Input)) && _impl::is_unsigned<Input>::value,
				char>::type Dummy = 0>
		constexpr Output shift_left(Input i) noexcept
		{
			return shift_left<0, Output, Input>(i) << Exponent;
		}

		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				!(Exponent <= 0) && !(sizeof(Output) <= sizeof(Input)),
				char>::type Dummy = 0>
		constexpr Output shift_right(Input i) noexcept
		{
			return shift_right<0, Output, Input>(i) >> Exponent;
		}

		// is_signed<Input>
		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				!(Exponent <= 0) && _impl::is_signed<Input>::value,
				int>::type Dummy = 0>
		constexpr Output shift_left(Input i) noexcept
		{
			using unsigned_input = typename _impl::make_unsigned<Input>::type;
			using signed_output = typename _impl::make_signed<Output>::type;

			return (i >= 0)
				? shift_left<Exponent, signed_output, unsigned_input>(i)
				: -shift_left<Exponent, signed_output, unsigned_input>(-i);
		}

		// Exponent < 0
		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				(Exponent < 0),
				int>::type Dummy = 0>
		constexpr Output shift_left(Input i) noexcept
		{
			// negate Exponent and flip from left to right
			return shift_right<-Exponent, Output, Input>(i);
		}

		template <
			int Exponent,
			class Output,
			class Input,
			typename std::enable_if<
				Exponent < 0,
				int>::type Dummy = 0>
		constexpr Output shift_right(Input i) noexcept
		{
			// negate Exponent and flip from right to left
			return shift_left<-Exponent, Output, Input>(i);
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::pow2

		// returns given power of 2
		template <class S, int Exponent, typename std::enable_if<Exponent == 0, int>::type Dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return 1;
		}

		template <class S, int Exponent, typename std::enable_if<!(Exponent <= 0), int>::type Dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return pow2<S, Exponent - 1>() * S(2);
		}

		template <class S, int Exponent, typename std::enable_if<!(Exponent >= 0), int>::type Dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return pow2<S, Exponent + 1>() * S(.5);
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::max

		template <class T>
		constexpr const T& max( const T& a, const T& b )
		{
			return (a < b) ? b : a;
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::common_repr_type

		// given two integral types, produces a common type with enough capacity to
		// store values of either EXCEPT when one is signed and both are same size
		template <class ReprType1, class ReprType2>
		using common_repr_type = typename _impl::get_int<
			_impl::is_signed<ReprType1>::value | _impl::is_signed<ReprType2>::value,
			_impl::max(sizeof(ReprType1), sizeof(ReprType2))>::type;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::capacity

		// has value that, given a value N, 
		// returns number of bits necessary to represent it in binary
		template <unsigned N> struct capacity;

		template <>
		struct capacity<0>
		{
			static constexpr int value = 0;
		};

		template <unsigned N>
		struct capacity
		{
			static constexpr int value = capacity<N / 2>::value + 1;
		};

		////////////////////////////////////////////////////////////////////////////////
		// _impl::necessary_repr_t

		// given a required number of bits a type should have and whether it is signed,
		// provides a built-in integral type with necessary capacity
		template <unsigned RequiredBits, bool IsSigned>
		using necessary_repr_t 
			= typename get_int<IsSigned, 1 << (capacity<((RequiredBits + 7) / 8) - 1>::value)>::type;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::sqrt helper functions

		template <class ReprType>
		constexpr ReprType sqrt_bit(
			ReprType n,
			ReprType bit = ReprType(1) << (num_bits<ReprType>() - 2)) noexcept
		{
			return (bit > n) ? sqrt_bit<ReprType>(n, bit >> 2) : bit;
		}

		template <class ReprType>
		constexpr ReprType sqrt_solve3(
			ReprType n,
			ReprType bit,
			ReprType result) noexcept
		{
			return bit
				   ? (n >= result + bit)
					 ? sqrt_solve3<ReprType>(n - (result + bit), bit >> 2, (result >> 1) + bit)
					 : sqrt_solve3<ReprType>(n, bit >> 2, result >> 1)
				   : result;
		}

		template <class ReprType>
		constexpr ReprType sqrt_solve1(ReprType n) noexcept
		{
			return sqrt_solve3<ReprType>(n, sqrt_bit<ReprType>(n), 0);
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point class template definition
	//
	// approximates a real number using a built-in integral type;
	// somewhat like a floating-point number but with exponent determined at run-time

	template <class ReprType = int, int Exponent = 0>
	class fixed_point
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		// types

		using repr_type = ReprType;

		////////////////////////////////////////////////////////////////////////////////
		// constants

		constexpr static int exponent = Exponent;
		constexpr static int digits = _impl::num_bits<ReprType>() - _impl::is_signed<repr_type>::value;
		constexpr static int integer_digits = digits + exponent;
		constexpr static int fractional_digits = digits - integer_digits;

		////////////////////////////////////////////////////////////////////////////////
		// functions

	private:
		// constructor taking representation explicitly using operator++(int)-style trick
		constexpr fixed_point(repr_type repr, int) noexcept
			: _repr(repr)
		{
		}
	public:
		// default c'tor
		fixed_point() noexcept {}

		// c'tor taking an integer type
		template <class S, typename std::enable_if<_impl::is_integral<S>::value, int>::type Dummy = 0>
		explicit constexpr fixed_point(S s) noexcept
			: _repr(integral_to_repr(s))
		{
		}

		// c'tor taking a floating-point type
		template <class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
		explicit constexpr fixed_point(S s) noexcept
			: _repr(floating_point_to_repr(s))
		{
		}

		// c'tor taking a fixed-point type
		template <class FromReprType, int FromExponent>
		explicit constexpr fixed_point(const fixed_point<FromReprType, FromExponent> & rhs) noexcept
			: _repr(fixed_point_to_repr(rhs))
		{
		}

		// copy assignment operator taking an integer type
		template <class S, typename std::enable_if<_impl::is_integral<S>::value, int>::type Dummy = 0>
		fixed_point & operator=(S s) noexcept
		{
			_repr = integral_to_repr(s);
			return *this;
		}

		// copy assignment operator taking a floating-point type
		template <class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
		fixed_point & operator=(S s) noexcept
		{
			_repr = floating_point_to_repr(s);
			return *this;
		}

		// copy assignement operator taking a fixed-point type
		template <class FromReprType, int FromExponent>
		fixed_point & operator=(const fixed_point<FromReprType, FromExponent> & rhs) noexcept
		{
			_repr = fixed_point_to_repr(rhs);
			return *this;
		}

		// returns value represented as a floating-point
		template <class S, typename std::enable_if<_impl::is_integral<S>::value, int>::type Dummy = 0>
		explicit constexpr operator S() const noexcept
		{
			return repr_to_integral<S>(_repr);
		}

		// returns value represented as integral
		template <class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
		explicit constexpr operator S() const noexcept
		{
			return repr_to_floating_point<S>(_repr);
		}

		// returns non-zeroness represented as boolean
		explicit constexpr operator bool() const noexcept
		{
			return _repr != 0;
		}

		// returns internal representation of value
		constexpr repr_type data() const noexcept
		{
			return _repr;
		}

		// creates an instance given the underlying representation value
		static constexpr fixed_point from_data(repr_type repr) noexcept
		{
			return fixed_point(repr, 0);
		}

	private:
		template <class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
		static constexpr S one() noexcept
		{
			return _impl::pow2<S, - exponent>();
		}

		template <class S, typename std::enable_if<_impl::is_integral<S>::value, int>::type Dummy = 0>
		static constexpr S one() noexcept
		{
			return integral_to_repr<S>(1);
		}

		template <class S>
		static constexpr S inverse_one() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return _impl::pow2<S, exponent>();
		}

		template <class S>
		static constexpr repr_type integral_to_repr(S s) noexcept
		{
			static_assert(_impl::is_integral<S>::value, "S must be unsigned integral type");

			return _impl::shift_right<exponent, repr_type>(s);
		}

		template <class S>
		static constexpr S repr_to_integral(repr_type r) noexcept
		{
			static_assert(_impl::is_integral<S>::value, "S must be unsigned integral type");

			return _impl::shift_left<exponent, S>(r);
		}

		template <class S>
		static constexpr repr_type floating_point_to_repr(S s) noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return static_cast<repr_type>(s * one<S>());
		}

		template <class S>
		static constexpr S repr_to_floating_point(repr_type r) noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return S(r) * inverse_one<S>();
		}

		template <class FromReprType, int FromExponent>
		static constexpr repr_type fixed_point_to_repr(const fixed_point<FromReprType, FromExponent> & rhs) noexcept
		{
			return _impl::shift_right<(exponent - FromExponent), repr_type>(rhs.data());
		}

		////////////////////////////////////////////////////////////////////////////////
		// variables

		repr_type _repr;
	};

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_fixed

	// given the desired number of integer and fractional digits,
	// generates a fixed_point type such that:
	//   fixed_point<>::integer_digits == IntegerDigits,
	// and
	//   fixed_point<>::fractional_digits >= FractionalDigits,
	template <unsigned IntegerDigits, unsigned FractionalDigits = 0, bool IsSigned = true>
	using make_fixed = fixed_point<
		typename _impl::necessary_repr_t<IntegerDigits + FractionalDigits + IsSigned, IsSigned>,
		(signed)(IntegerDigits + IsSigned) - _impl::num_bits<typename _impl::necessary_repr_t<IntegerDigits + FractionalDigits + IsSigned, IsSigned>>()>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_ufixed

	// unsigned short-hanrd for make_fixed
	template <unsigned IntegerDigits, unsigned FractionalDigits = 0>
	using make_ufixed = make_fixed<IntegerDigits, FractionalDigits, false>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_fixed_from_repr

	// yields a float_point with Exponent calculated such that
	// fixed_point<ReprType, Exponent>::integer_bits == IntegerDigits
	template <class ReprType, int IntegerDigits>
	using make_fixed_from_repr = fixed_point<
		ReprType,
		IntegerDigits + _impl::is_signed<ReprType>::value - (signed)sizeof(ReprType) * CHAR_BIT>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point_promotion_t / promote

	// given template parameters of a fixed_point specialization, 
	// yields alternative specialization with twice the fractional bits
	// and twice the integral/sign bits
	template <class FixedPoint>
	using fixed_point_promotion_t = fixed_point<
		_impl::next_size_t<typename FixedPoint::repr_type>,
		FixedPoint::exponent * 2>;

	// as fixed_point_promotion_t but promotes parameter, from
	template <class FixedPoint>
	fixed_point_promotion_t<FixedPoint>
	constexpr promote(const FixedPoint & from) noexcept
	{
		return fixed_point_promotion_t<FixedPoint>(from);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point_demotion_t / demote

	// given template parameters of a fixed_point specialization, 
	// yields alternative specialization with half the fractional bits
	// and half the integral/sign bits (assuming Exponent is even)
	template <class FixedPoint>
	using fixed_point_demotion_t = fixed_point<
		_impl::previous_size_t<typename FixedPoint::repr_type>,
		FixedPoint::exponent / 2>;

	// as fixed_point_demotion_t but demotes parameter, from
	template <class FixedPoint>
	fixed_point_demotion_t<FixedPoint>
	constexpr demote(const FixedPoint & from) noexcept
	{
		return fixed_point_demotion_t<FixedPoint>(from);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point-aware _impl definitions

	namespace _impl
	{
		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_fixed_point

		template <class T>
		struct is_fixed_point;

		template <class T>
		struct is_fixed_point
			: public std::integral_constant<bool, false> {};

		template <class ReprType, int Exponent>
		struct is_fixed_point <fixed_point<ReprType, Exponent>>
			: public std::integral_constant<bool, true> {};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::_common_type

		template <class Lhs, class Rhs, class _Enable = void>
		struct _common_type;

		// given two fixed-point, produces the type that is best suited to both of them
		template <class LhsReprType, int LhsExponent, class RhsReprType, int RhsExponent>
		struct _common_type<fixed_point<LhsReprType, LhsExponent>, fixed_point<RhsReprType, RhsExponent>>
		{
			using type = make_fixed_from_repr<
				_impl::common_repr_type<LhsReprType, RhsReprType>,
				_impl::max(
					fixed_point<LhsReprType, LhsExponent>::integer_digits,
					fixed_point<RhsReprType, RhsExponent>::integer_digits)>;
		};

		// given a fixed-point and a integer type, 
		// generates a floating-point that is as big as both of them (or as close as possible)
		template <class LhsReprType, int LhsExponent, class Integer>
		struct _common_type<
			fixed_point<LhsReprType, LhsExponent>,
			Integer,
			typename std::enable_if<_impl::is_integral<Integer>::value>::type>
		: _common_type<
			fixed_point<LhsReprType, LhsExponent>,
			fixed_point<Integer>>
		{
		};

		// given a fixed-point and a floating-point type, 
		// generates a floating-point that is as big as both of them (or as close as possible)
		template <class LhsReprType, int LhsExponent, class Float>
		struct _common_type<
			fixed_point<LhsReprType, LhsExponent>,
			Float,
			typename std::enable_if<std::is_floating_point<Float>::value>::type>
		: std::common_type<_impl::get_float_t<sizeof(LhsReprType)>, Float>
		{
		};

		// when first type is not fixed-point and second type is, reverse the order
		template <class Lhs, class RhsReprType, int Exponent>
		struct _common_type<Lhs, fixed_point<RhsReprType, Exponent>>
		: _common_type<fixed_point<RhsReprType, Exponent>, Lhs>
		{
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::common_type

		// similar to std::common_type 
		// but one or both input types must be fixed_point
		template <typename Lhs, typename Rhs>
		using common_type = typename _common_type<Lhs, Rhs>::type;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::multiply

		template <class Result, class Lhs, class Rhs>
		constexpr Result multiply(const Lhs & lhs, const Rhs & rhs) noexcept
		{
			using result_repr_type = typename Result::repr_type;
			using intermediate_repr_type = _impl::next_size_t<typename common_type<Lhs, Rhs>::repr_type>;
			return Result::from_data(
				_impl::shift_left<(Lhs::exponent + Rhs::exponent - Result::exponent), result_repr_type>(
					static_cast<intermediate_repr_type>(lhs.data()) * static_cast<intermediate_repr_type>(rhs.data())));
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::add

		template <class Result, class FixedPoint, class Head>
		constexpr Result add(const Head & addend_head)
		{
			static_assert(std::is_same<FixedPoint, Head>::value, "mismatched trunc_add parameters");
			return static_cast<Result>(addend_head);
		}

		template <class Result, class FixedPoint, class Head, class ... Tail>
		constexpr Result add(const Head & addend_head, const Tail & ... addend_tail)
		{
			static_assert(std::is_same<FixedPoint, Head>::value, "mismatched trunc_add parameters");
			return add<Result, FixedPoint, Tail ...>(addend_tail ...) + static_cast<Result>(addend_head);
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// heterogeneous comparison operators
	//
	// compare two objects of different fixed_point specializations

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator==(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() == static_cast<common_type>(rhs).data();
	}

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator!=(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() != static_cast<common_type>(rhs).data();
	}

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator<(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() < static_cast<common_type>(rhs).data();
	}

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator>(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() > static_cast<common_type>(rhs).data();
	}

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator>=(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() >= static_cast<common_type>(rhs).data();
	}

	template <class Lhs, class Rhs, typename std::enable_if<_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, int>::type Dummy = 0>
	constexpr bool operator<=(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using common_type = _impl::common_type<Lhs, Rhs>;
		return static_cast<common_type>(lhs).data() <= static_cast<common_type>(rhs).data();
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::sqrt

	// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
	// slow when calculated at run-time?
	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent>
	sqrt(const fixed_point<ReprType, Exponent> & x) noexcept
	{
		return fixed_point<ReprType, Exponent>::from_data(
			static_cast<ReprType>(_impl::sqrt_solve1(promote(x).data())));
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_add_result_t / trunc_add

	// yields specialization of fixed_point with integral bits necessary to store
	// result of an addition between N values of fixed_point<ReprType, Exponent>
	template <class FixedPoint, unsigned N = 2>
	using trunc_add_result_t = make_fixed_from_repr<
		typename FixedPoint::repr_type,
		fixed_point<
			typename FixedPoint::repr_type,
			FixedPoint::exponent>::integer_digits + _impl::capacity<N - 1>::value>;

	template <class FixedPoint, class ... Tail>
	trunc_add_result_t<FixedPoint, sizeof...(Tail) + 1>
	constexpr trunc_add(const FixedPoint & addend1, const Tail & ... addend_tail)
	{
		using output_type = trunc_add_result_t<FixedPoint, sizeof...(Tail) + 1>;
		return _impl::add<output_type, FixedPoint>(addend1, addend_tail ...);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_subtract_result_t / trunc_subtract

	// yields specialization of fixed_point with integral bits necessary to store
	// result of an subtraction between N values of fixed_point<ReprType, Exponent>
	template <class Lhs, class Rhs = Lhs>
	using trunc_subtract_result_t = make_fixed_from_repr<
		_impl::get_int_t<true, _impl::max(sizeof(typename Lhs::repr_type), sizeof(typename Rhs::repr_type))>,
		_impl::max(Lhs::integer_digits, Rhs::integer_digits) + 1>;

	template <class Lhs, class Rhs>
	trunc_subtract_result_t<Lhs, Rhs>
	constexpr trunc_subtract(const Lhs & minuend, const Rhs & subtrahend)
	{
		using output_type = trunc_subtract_result_t<Lhs, Rhs>;
		return static_cast<output_type>(minuend) - static_cast<output_type>(subtrahend);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_multiply_result_t / trunc_multiply

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<ReprType, Exponent>
	template <class Lhs, class Rhs = Lhs>
	using trunc_multiply_result_t = make_fixed_from_repr<
		_impl::common_repr_type<typename Lhs::repr_type, typename Rhs::repr_type>,
		Lhs::integer_digits + Rhs::integer_digits>;

	// as trunc_multiply_result_t but converts parameter, factor,
	// ready for safe binary multiply
	template <class Lhs, class Rhs>
	trunc_multiply_result_t<Lhs, Rhs>
	constexpr trunc_multiply(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using result_type = trunc_multiply_result_t<Lhs, Rhs>;
		return _impl::multiply<result_type>(lhs, rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_square_result_t / trunc_square

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<ReprType, Exponent>
	// whose sign bit is set to the same value
	template <class FixedPoint>
	using trunc_square_result_t = make_fixed_from_repr<
		typename _impl::make_unsigned<typename FixedPoint::repr_type>::type,
		FixedPoint::integer_digits * 2>;

	// as trunc_square_result_t but converts parameter, factor,
	// ready for safe binary multiply-by-self
	template <class FixedPoint>
	trunc_square_result_t<FixedPoint>
	constexpr trunc_square(const FixedPoint & root) noexcept
	{
		using result_type = trunc_square_result_t<FixedPoint>;
		return _impl::multiply<result_type>(root, root);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_sqrt_result_t / trunc_sqrt

	// yields specialization of fixed_point with integral bits necessary to store
	// the positive result of a square root operation on an object of type,
	// fixed_point<ReprType, Exponent>
	template <class FixedPoint>
	using trunc_sqrt_result_t = make_fixed_from_repr<
		typename _impl::make_unsigned<typename FixedPoint::repr_type>::type,
		(FixedPoint::integer_digits + 1) / 2>;

	// as trunc_sqrt_result_t but converts parameter, factor,
	// ready for safe sqrt operation
	template <class FixedPoint>
	trunc_sqrt_result_t<FixedPoint>
	constexpr trunc_sqrt(const FixedPoint & square) noexcept
	{
		using output_type = trunc_sqrt_result_t<FixedPoint>;
		return output_type(sqrt(square));
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::promote_multiply_result_t / promote_multiply

	// yields specialization of fixed_point with capacity necessary to store
	// result of a multiply between values of fixed_point<ReprType, Exponent>
	template <class Lhs, class Rhs = Lhs>
	using promote_multiply_result_t = fixed_point_promotion_t<_impl::common_type<Lhs, Rhs>>;

	// as promote_multiply_result_t but converts parameter, factor,
	// ready for safe binary multiply
	template <class Lhs, class Rhs>
	promote_multiply_result_t<Lhs, Rhs>
		constexpr promote_multiply(const Lhs & lhs, const Rhs & rhs) noexcept
	{
		using result_type = promote_multiply_result_t<Lhs, Rhs>;
		return _impl::multiply<result_type>(lhs, rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::promote_square_result_t / promote_square

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<ReprType, Exponent>
	// whose sign bit is set to the same value
	template <class FixedPoint>
	using promote_square_result_t = make_ufixed<
		FixedPoint::integer_digits * 2,
		FixedPoint::fractional_digits * 2>;

	// as promote_square_result_t but converts parameter, factor,
	// ready for safe binary multiply-by-self
	template <class FixedPoint>
	promote_square_result_t<FixedPoint>
		constexpr promote_square(const FixedPoint & root) noexcept
	{
		using output_type = promote_square_result_t<FixedPoint>;
		using output_repr_type = typename output_type::repr_type;
		return output_type::from_data(
			_impl::shift_left<(FixedPoint::exponent * 2 - output_type::exponent), output_repr_type>(
				static_cast<output_repr_type>(root.data()) * static_cast<output_repr_type>(root.data())));
	}

	////////////////////////////////////////////////////////////////////////////////
	// fixed_point arithmetic operator overloads

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent> operator-(
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		static_assert(_impl::is_signed<ReprType>::value, "unary negation of unsigned value");

		return fixed_point<ReprType, Exponent>::from_data(-rhs.data());
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent> operator+(
		const fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return fixed_point<ReprType, Exponent>::from_data(lhs.data() + rhs.data());
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent> operator-(
		const fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return fixed_point<ReprType, Exponent>::from_data(lhs.data() - rhs.data());
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent> operator*(
		const fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return _impl::multiply<fixed_point<ReprType, Exponent>>(lhs, rhs);
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent> operator/(
		const fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return fixed_point<ReprType, Exponent>::from_data(
			ReprType(_impl::shift_right<Exponent, _impl::next_size_t<ReprType>>(lhs.data()) / rhs.data()));
	}

	template <class ReprType, int Exponent>
	fixed_point<ReprType, Exponent> & operator+=(
		fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return lhs = lhs + rhs;
	}

	template <class ReprType, int Exponent>
	fixed_point<ReprType, Exponent> & operator-=(
		fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return lhs = lhs - rhs;
	}

	template <class ReprType, int Exponent>
	fixed_point<ReprType, Exponent> & operator*=(
		fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return lhs = lhs * rhs;
	}

	template <class ReprType, int Exponent>
	fixed_point<ReprType, Exponent> & operator/=(
		fixed_point<ReprType, Exponent> & lhs,
		const fixed_point<ReprType, Exponent> & rhs) noexcept
	{
		return lhs = lhs / rhs;
	}
}

#endif	// defined(_SG14_FIXED_POINT)
