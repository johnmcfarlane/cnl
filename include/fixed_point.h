#if ! defined(_SG14_FIXED_POINT)
#define _SG14_FIXED_POINT 1

#include <algorithm>
#include <climits>
#include <cmath>
#include <cinttypes>
#include <limits>
#include <type_traits>
#include <ostream>
#include <istream>

#if defined(__clang__) || defined(__GNUG__)
// sg14::float_point only fully supports 64-bit types with the help of 128-bit ints.
// Clang and GCC use (__int128) and (unsigned __int128) for 128-bit ints.
#define _SG14_FIXED_POINT_128
#endif

namespace sg14
{
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

		template <bool SIGNED, int NUM_BYTES>
		struct get_int;

		template <bool SIGNED, int NUM_BYTES>
		using get_int_t = typename get_int<SIGNED, NUM_BYTES>::type;

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

		template <int NUM_BYTES>
		struct get_float;

		template <int NUM_BYTES>
		using get_float_t = typename get_float<NUM_BYTES>::type;

		// specializations
		template <> struct get_float<1> { using type = float; };
		template <> struct get_float<2> { using type = float; };
		template <> struct get_float<4> { using type = float; };
		template <> struct get_float<8> { using type = double; };
		template <> struct get_float<16> { using type = long double; };

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_integral

		template <typename T>
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

		template <typename T>
		struct is_integral : std::is_integral<T> { };

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_signed

		template <typename T>
		struct is_signed
		{
			static_assert(is_integral<T>::value, "sg14::_impl::is_signed only intended for use with integral types");
			static constexpr bool value = std::is_same<typename get_int<true, sizeof(T)>::type, T>::value;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::is_unsigned

		template <typename T>
		struct is_unsigned
		{
			static_assert(is_integral<T>::value, "sg14::_impl::is_unsigned only intended for use with integral types");
			static constexpr bool value = std::is_same<typename get_int<false, sizeof(T)>::type, T>::value;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::make_signed

		template <typename T>
		struct make_signed
		{
			using type = typename get_int<true, sizeof(T)>::type;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::make_unsigned

		template <typename T>
		struct make_unsigned
		{
			using type = typename get_int<false, sizeof(T)>::type;
		};

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::next_size_t

		// given an integral type, INT_TYPE,
		// provides the integral type of the equivalent type with twice the size
		template <typename INT_TYPE>
		using next_size_t = get_int_t<_impl::is_signed<INT_TYPE>::value, sizeof(INT_TYPE) * 2>;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::previous_size_t

		// given an integral type, INT_TYPE,
		// provides the integral type of the equivalent type with half the size
		template <typename INT_TYPE>
		using previous_size_t = get_int_t<_impl::is_signed<INT_TYPE>::value, sizeof(INT_TYPE) / 2>;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::shift_left and sg14::_impl::shift_right

		// performs a shift operation by a fixed number of bits avoiding two pitfalls:
		// 1) shifting by a negative amount causes undefined behavior
		// 2) converting between integer types of different sizes can lose significant bits during shift right

		// EXPONENT == 0
		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				(EXPONENT == 0),
				int>::type dummy = 0>
		constexpr OUTPUT shift_left(INPUT i) noexcept
		{
			static_assert(_impl::is_integral<INPUT>::value, "INPUT must be integral type");
			static_assert(_impl::is_integral<OUTPUT>::value, "OUTPUT must be integral type");

			// cast only
			return static_cast<OUTPUT>(i);
		}

		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				EXPONENT == 0,
				int>::type dummy = 0>
		constexpr OUTPUT shift_right(INPUT i) noexcept
		{
			static_assert(_impl::is_integral<INPUT>::value, "INPUT must be integral type");
			static_assert(_impl::is_integral<OUTPUT>::value, "OUTPUT must be integral type");

			// cast only
			return static_cast<OUTPUT>(i);
		}

		// sizeof(INPUT) > sizeof(OUTPUT)
		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				!(EXPONENT <= 0) && sizeof(OUTPUT) <= sizeof(INPUT) && _impl::is_unsigned<INPUT>::value,
				int>::type dummy = 0>
		constexpr OUTPUT shift_left(INPUT i) noexcept
		{
			return shift_left<0, OUTPUT, INPUT>(i) << EXPONENT;
		}

		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				!(EXPONENT <= 0) && sizeof(OUTPUT) <= sizeof(INPUT),
				int>::type dummy = 0>
		constexpr OUTPUT shift_right(INPUT i) noexcept
		{
			return shift_right<0, OUTPUT, INPUT>(i >> EXPONENT);
		}

		// sizeof(INPUT) <= sizeof(OUTPUT)
		template <
			int EXPONENT, 
			typename OUTPUT, 
			typename INPUT, 
			typename std::enable_if<
				!(EXPONENT <= 0) && !(sizeof(OUTPUT) <= sizeof(INPUT)) && _impl::is_unsigned<INPUT>::value,
				char>::type dummy = 0>
		constexpr OUTPUT shift_left(INPUT i) noexcept
		{
			return shift_left<0, OUTPUT, INPUT>(i) << EXPONENT;
		}

		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				!(EXPONENT <= 0) && !(sizeof(OUTPUT) <= sizeof(INPUT)),
				char>::type dummy = 0>
		constexpr OUTPUT shift_right(INPUT i) noexcept
		{
			return shift_right<0, OUTPUT, INPUT>(i) >> EXPONENT;
		}

		// is_signed<INPUT>
		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				!(EXPONENT <= 0) && _impl::is_signed<INPUT>::value,
				int>::type dummy = 0>
		constexpr OUTPUT shift_left(INPUT i) noexcept
		{
			using unsigned_input = typename _impl::make_unsigned<INPUT>::type;
			using signed_output = typename _impl::make_signed<OUTPUT>::type;

			return (i >= 0)
				? shift_left<EXPONENT, signed_output, unsigned_input>(i)
				: -shift_left<EXPONENT, signed_output, unsigned_input>(-i);
		}

		// EXPONENT < 0
		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				(EXPONENT < 0),
				int>::type dummy = 0>
		constexpr OUTPUT shift_left(INPUT i) noexcept
		{
			// negate EXPONENT and flip from left to right
			return shift_right<-EXPONENT, OUTPUT, INPUT>(i);
		}

		template <
			int EXPONENT,
			typename OUTPUT,
			typename INPUT,
			typename std::enable_if<
				EXPONENT < 0,
				int>::type dummy = 0>
		constexpr OUTPUT shift_right(INPUT i) noexcept
		{
			// negate EXPONENT and flip from right to left
			return shift_left<-EXPONENT, OUTPUT, INPUT>(i);
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::default_exponent

		template <typename REPR_TYPE>
		constexpr int default_exponent() noexcept
		{
			return num_bits<REPR_TYPE>() / -2;
		}

		////////////////////////////////////////////////////////////////////////////////
		// sg14::_impl::pow2

		// returns given power of 2
		template <typename S, int EXPONENT, typename std::enable_if<EXPONENT == 0, int>::type dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return 1;
		}

		template <typename S, int EXPONENT, typename std::enable_if<!(EXPONENT <= 0), int>::type dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return pow2<S, EXPONENT - 1>() * S(2);
		}

		template <typename S, int EXPONENT, typename std::enable_if<!(EXPONENT >= 0), int>::type dummy = 0>
		constexpr S pow2() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return pow2<S, EXPONENT + 1>() * S(.5);
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
		template <typename REPR_TYPE_1, typename REPR_TYPE_2>
		using common_repr_type = typename _impl::get_int<
			_impl::is_signed<REPR_TYPE_1>::value | _impl::is_signed<REPR_TYPE_2>::value,
			_impl::max(sizeof(REPR_TYPE_1), sizeof(REPR_TYPE_2))>::type;

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
		template <unsigned REQUIRED_BITS, bool IS_SIGNED>
		using necessary_repr_t 
			= typename get_int<IS_SIGNED, 1 << (capacity<((REQUIRED_BITS + 7) / 8) - 1>::value)>::type;

		////////////////////////////////////////////////////////////////////////////////
		// sg14::sqrt helper functions

		template <typename REPR_TYPE>
		constexpr REPR_TYPE sqrt_bit(
			REPR_TYPE n,
			REPR_TYPE bit = REPR_TYPE(1) << (num_bits<REPR_TYPE>() - 2)) noexcept
		{
			return (bit > n) ? sqrt_bit<REPR_TYPE>(n, bit >> 2) : bit;
		}

		template <typename REPR_TYPE>
		constexpr REPR_TYPE sqrt_solve3(
			REPR_TYPE n,
			REPR_TYPE bit,
			REPR_TYPE result) noexcept
		{
			return bit
				   ? (n >= result + bit)
					 ? sqrt_solve3<REPR_TYPE>(n - (result + bit), bit >> 2, (result >> 1) + bit)
					 : sqrt_solve3<REPR_TYPE>(n, bit >> 2, result >> 1)
				   : result;
		}

		template <typename REPR_TYPE>
		constexpr REPR_TYPE sqrt_solve1(REPR_TYPE n) noexcept
		{
			return sqrt_solve3<REPR_TYPE>(n, sqrt_bit<REPR_TYPE>(n), 0);
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point class template definition
	//
	// approximates a real number using a built-in integral type;
	// somewhat like a floating-point number but with exponent determined at run-time

	template <typename REPR_TYPE = int, int EXPONENT = _impl::default_exponent<REPR_TYPE>()>
	class fixed_point
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		// types

		using repr_type = REPR_TYPE;

		////////////////////////////////////////////////////////////////////////////////
		// constants

		constexpr static int exponent = EXPONENT;
		constexpr static int digits = _impl::num_bits<REPR_TYPE>() - _impl::is_signed<repr_type>::value;
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
		template <typename S, typename std::enable_if<_impl::is_integral<S>::value, int>::type dummy = 0>
		explicit constexpr fixed_point(S s) noexcept
			: _repr(integral_to_repr(s))
		{
		}

		// c'tor taking a floating-point type
		template <typename S, typename std::enable_if<std::is_floating_point<S>::value, int>::type dummy = 0>
		explicit constexpr fixed_point(S s) noexcept
			: _repr(floating_point_to_repr(s))
		{
		}

		// c'tor taking a fixed-point type
		template <typename FROM_REPR_TYPE, int FROM_EXPONENT>
		explicit constexpr fixed_point(fixed_point<FROM_REPR_TYPE, FROM_EXPONENT> const & rhs) noexcept
			: _repr(fixed_point_to_repr(rhs))
		{
		}

		// copy assignment operator taking an integer type
		template <typename S, typename std::enable_if<_impl::is_integral<S>::value, int>::type dummy = 0>
		fixed_point & operator=(S s) noexcept
		{
			_repr = integral_to_repr(s);
			return *this;
		}

		// copy assignment operator taking a floating-point type
		template <typename S, typename std::enable_if<std::is_floating_point<S>::value, int>::type dummy = 0>
		fixed_point & operator=(S s) noexcept
		{
			_repr = floating_point_to_repr(s);
			return *this;
		}

		// copy assignement operator taking a fixed-point type
		template <typename FROM_REPR_TYPE, int FROM_EXPONENT>
		fixed_point & operator=(fixed_point<FROM_REPR_TYPE, FROM_EXPONENT> const & rhs) noexcept
		{
			_repr = fixed_point_to_repr(rhs);
			return *this;
		}

		// returns value represented as a floating-point
		template <typename S, typename std::enable_if<_impl::is_integral<S>::value, int>::type dummy = 0>
		explicit constexpr operator S() const noexcept
		{
			return repr_to_integral<S>(_repr);
		}

		// returns value represented as integral
		template <typename S, typename std::enable_if<std::is_floating_point<S>::value, int>::type dummy = 0>
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
		// TODO: constexpr with c++14?
		static constexpr fixed_point from_data(repr_type repr) noexcept
		{
			return fixed_point(repr, 0);
		}

		// comparison
		friend constexpr bool operator==(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return lhs._repr == rhs._repr;
		}
		friend constexpr bool operator!=(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return ! (lhs == rhs);
		}

		friend constexpr bool operator>(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return lhs._repr > rhs._repr;
		}
		friend constexpr bool operator<(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return lhs._repr < rhs._repr;
		}

		friend constexpr bool operator>=(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return lhs._repr >= rhs._repr;
		}
		friend constexpr bool operator<=(fixed_point const & lhs, fixed_point const & rhs) noexcept
		{
			return lhs._repr <= rhs._repr;
		}

	private:
		template <typename S, typename std::enable_if<std::is_floating_point<S>::value, int>::type dummy = 0>
		static constexpr S one() noexcept
		{
			return _impl::pow2<S, - exponent>();
		}

		template <typename S, typename std::enable_if<_impl::is_integral<S>::value, int>::type dummy = 0>
		static constexpr S one() noexcept
		{
			return integral_to_repr<S>(1);
		}

		template <typename S>
		static constexpr S inverse_one() noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return _impl::pow2<S, exponent>();
		}

		template <typename S>
		static constexpr repr_type integral_to_repr(S s) noexcept
		{
			static_assert(_impl::is_integral<S>::value, "S must be unsigned integral type");

			return _impl::shift_right<exponent, repr_type>(s);
		}

		template <typename S>
		static constexpr S repr_to_integral(repr_type r) noexcept
		{
			static_assert(_impl::is_integral<S>::value, "S must be unsigned integral type");

			return _impl::shift_left<exponent, S>(r);
		}

		template <typename S>
		static constexpr repr_type floating_point_to_repr(S s) noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return static_cast<repr_type>(s * one<S>());
		}

		template <typename S>
		static constexpr S repr_to_floating_point(repr_type r) noexcept
		{
			static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
			return S(r) * inverse_one<S>();
		}

		template <typename FROM_REPR_TYPE, int FROM_EXPONENT>
		static constexpr repr_type fixed_point_to_repr(fixed_point<FROM_REPR_TYPE, FROM_EXPONENT> const & rhs) noexcept
		{
			return _impl::shift_right<(exponent - FROM_EXPONENT), repr_type>(rhs.data());
		}

		////////////////////////////////////////////////////////////////////////////////
		// variables

		repr_type _repr;
	};

	////////////////////////////////////////////////////////////////////////////////
	// sg14::is_fixed_point

	template <typename T>
	struct is_fixed_point;

	template <typename T>
	struct is_fixed_point
		: public std::integral_constant<bool, false> {};

	template <typename REPR_TYPE, int EXPONENT>
	struct is_fixed_point <fixed_point<REPR_TYPE, EXPONENT>>
		: public std::integral_constant<bool, true> {};

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_fixed

	// given the desired number of integer and fractional digits,
	// generates a fixed_point type such that:
	//   fixed_point<>::integer_digits == INTEGER_DIGITS,
	// and
	//   fixed_point<>::fractional_digits >= FRACTIONAL_DIGITS,
	template <unsigned INTEGER_DIGITS, unsigned FRACTIONAL_DIGITS = 0, bool IS_SIGNED = true>
	using make_fixed = fixed_point<
		typename _impl::necessary_repr_t<INTEGER_DIGITS + FRACTIONAL_DIGITS + IS_SIGNED, IS_SIGNED>,
		(signed)(INTEGER_DIGITS + IS_SIGNED) - _impl::num_bits<typename _impl::necessary_repr_t<INTEGER_DIGITS + FRACTIONAL_DIGITS + IS_SIGNED, IS_SIGNED>>()>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_ufixed

	// unsigned short-hanrd for make_fixed
	template <unsigned INTEGER_DIGITS, unsigned FRACTIONAL_DIGITS = 0>
	using make_ufixed = make_fixed<INTEGER_DIGITS, FRACTIONAL_DIGITS, false>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::make_fixed_from_repr

	// yields a float_point with EXPONENT calculated such that
	// fixed_point<REPR_TYPE, EXPONENT>::integer_bits == INTEGER_BITS
	template <typename REPR_TYPE, int INTEGER_BITS>
	using make_fixed_from_repr = fixed_point<
		REPR_TYPE,
		INTEGER_BITS + _impl::is_signed<REPR_TYPE>::value - (signed)sizeof(REPR_TYPE) * CHAR_BIT>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::common_type

	// given two fixed-point types, produces the type that is best suited to both of them
	template <typename LHS_FP, typename RHS_FP>
	using common_type = make_fixed_from_repr<
		_impl::common_repr_type<typename LHS_FP::repr_type, typename RHS_FP::repr_type>,
		_impl::max(
			LHS_FP::integer_digits,
			RHS_FP::integer_digits)>;

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point_promotion_t / promote

	// given template parameters of a fixed_point specialization, 
	// yields alternative specialization with twice the fractional bits
	// and twice the integral/sign bits
	template <typename FIXED_POINT>
	using fixed_point_promotion_t = fixed_point<
		_impl::next_size_t<typename FIXED_POINT::repr_type>,
		FIXED_POINT::exponent * 2>;

	// as fixed_point_promotion_t but promotes parameter, from
	template <typename FIXED_POINT>
	fixed_point_promotion_t<FIXED_POINT>
	constexpr promote(const FIXED_POINT & from) noexcept
	{
		return fixed_point_promotion_t<FIXED_POINT>(from);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point_demotion_t / demote

	// given template parameters of a fixed_point specialization, 
	// yields alternative specialization with half the fractional bits
	// and half the integral/sign bits (assuming EXPONENT is even)
	template <typename FIXED_POINT>
	using fixed_point_demotion_t = fixed_point<
		_impl::previous_size_t<typename FIXED_POINT::repr_type>,
		FIXED_POINT::exponent / 2>;

	// as fixed_point_demotion_t but demotes parameter, from
	template <typename FIXED_POINT>
	fixed_point_demotion_t<FIXED_POINT>
	constexpr demote(const FIXED_POINT & from) noexcept
	{
		return fixed_point_demotion_t<FIXED_POINT>(from);
	}

	////////////////////////////////////////////////////////////////////////////////
	// heterogeneous comparison operators
	//
	// compare two objects of different fixed_point specializations

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator ==(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) == static_cast<fixed_point>(rhs);
	}

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator !=(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) != static_cast<fixed_point>(rhs);
	}

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator <(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) < static_cast<fixed_point>(rhs);
	}

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator >(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) > static_cast<fixed_point>(rhs);
	}

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator >=(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) >= static_cast<fixed_point>(rhs);
	}

	template <typename LHS_REPR_TYPE, int LHS_EXPONENT, typename RHS_REPR_TYPE, int RHS_EXPONENT>
	constexpr bool operator <=(
		fixed_point<LHS_REPR_TYPE, LHS_EXPONENT> const & lhs,
		fixed_point<RHS_REPR_TYPE, RHS_EXPONENT> const & rhs) noexcept
	{
		using fixed_point = common_type<fixed_point<LHS_REPR_TYPE, LHS_EXPONENT>, fixed_point<RHS_REPR_TYPE, RHS_EXPONENT>>;
		return static_cast<fixed_point>(lhs) <= static_cast<fixed_point>(rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::abs

	template <typename REPR_TYPE, int EXPONENT, typename std::enable_if<_impl::is_signed<REPR_TYPE>::value, int>::type dummy = 0>
	constexpr fixed_point<REPR_TYPE, EXPONENT>
	abs(fixed_point<REPR_TYPE, EXPONENT> const & x) noexcept
	{
		return (x.data() >= 0) ? x : - x;
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::sqrt

	// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
	// slow when calculated at run-time?
	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT>
	sqrt(fixed_point<REPR_TYPE, EXPONENT> const & x) noexcept
	{
		return fixed_point<REPR_TYPE, EXPONENT>::from_data(
			static_cast<REPR_TYPE>(_impl::sqrt_solve1(promote(x).data())));
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trig
	//
	// Placeholder implementations fall back on <cmath> functions which is slow
	// due to conversion to and from floating-point types; also inconvenient as
	// many <cmath> functions are not constexpr.

	namespace _impl
	{
		template <typename REPR_TYPE, int EXPONENT, _impl::get_float_t<sizeof(REPR_TYPE)>(*F)(_impl::get_float_t<sizeof(REPR_TYPE)>)>
		constexpr fixed_point<REPR_TYPE, EXPONENT>
			crib(fixed_point<REPR_TYPE, EXPONENT> const & x) noexcept
		{
			using floating_point = _impl::get_float_t<sizeof(REPR_TYPE)>;
			return static_cast<fixed_point<REPR_TYPE, EXPONENT>>(F(static_cast<floating_point>(x)));
		}
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT>
		sin(fixed_point<REPR_TYPE, EXPONENT> const & x) noexcept
	{
		return _impl::crib<REPR_TYPE, EXPONENT, std::sin>(x);
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT>
		cos(fixed_point<REPR_TYPE, EXPONENT> const & x) noexcept
	{
		return _impl::crib<REPR_TYPE, EXPONENT, std::cos>(x);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::_impl::multiply

	namespace _impl
	{
		template <typename RESULT, typename LHS, typename RHS>
		constexpr RESULT multiply(LHS const & lhs, RHS const & rhs) noexcept
		{
			using result_repr_type = typename RESULT::repr_type;
			using intermediate_repr_type = _impl::next_size_t<typename common_type<LHS, RHS>::repr_type>;
			return RESULT::from_data(
				_impl::shift_left<(LHS::exponent + RHS::exponent - RESULT::exponent), result_repr_type>(
					static_cast<intermediate_repr_type>(lhs.data()) * static_cast<intermediate_repr_type>(rhs.data())));
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_add_result_t / trunc_add

	// yields specialization of fixed_point with integral bits necessary to store
	// result of an addition between N values of fixed_point<REPR_TYPE, EXPONENT>
	template <typename FIXED_POINT, unsigned N = 2>
	using trunc_add_result_t = make_fixed_from_repr<
		typename FIXED_POINT::repr_type,
		fixed_point<
			typename FIXED_POINT::repr_type,
			FIXED_POINT::exponent>::integer_digits + _impl::capacity<N - 1>::value>;

	namespace _impl
	{
		template <typename RESULT_TYPE, typename FIXED_POINT, typename HEAD>
		constexpr RESULT_TYPE add(HEAD const & addend_head)
		{
			static_assert(std::is_same<FIXED_POINT, HEAD>::value, "mismatched trunc_add parameters");
			return static_cast<RESULT_TYPE>(addend_head);
		}

		template <typename RESULT_TYPE, typename FIXED_POINT, typename HEAD, typename ... TAIL>
		constexpr RESULT_TYPE add(HEAD const & addend_head, TAIL const & ... addend_tail)
		{
			static_assert(std::is_same<FIXED_POINT, HEAD>::value, "mismatched trunc_add parameters");
			return add<RESULT_TYPE, FIXED_POINT, TAIL ...>(addend_tail ...) + static_cast<RESULT_TYPE>(addend_head);
		}
	}

	template <typename FIXED_POINT, typename ... TAIL>
	trunc_add_result_t<FIXED_POINT, sizeof...(TAIL) + 1>
	constexpr trunc_add(FIXED_POINT const & addend1, TAIL const & ... addend_tail)
	{
		using output_type = trunc_add_result_t<FIXED_POINT, sizeof...(TAIL) + 1>;
		return _impl::add<output_type, FIXED_POINT>(addend1, addend_tail ...);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_subtract_result_t / trunc_subtract

	// yields specialization of fixed_point with integral bits necessary to store
	// result of an subtraction between N values of fixed_point<REPR_TYPE, EXPONENT>
	template <typename LHS, typename RHS = LHS>
	using trunc_subtract_result_t = make_fixed_from_repr<
		_impl::get_int_t<true, _impl::max(sizeof(typename LHS::repr_type), sizeof(typename RHS::repr_type))>,
		_impl::max(LHS::integer_digits, RHS::integer_digits) + 1>;

	template <typename LHS, typename RHS>
	trunc_subtract_result_t<LHS, RHS>
	constexpr trunc_subtract(LHS const & minuend, RHS const & subtrahend)
	{
		using output_type = trunc_subtract_result_t<LHS, RHS>;
		return static_cast<output_type>(minuend) - static_cast<output_type>(subtrahend);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_multiply_result_t / trunc_multiply

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<REPR_TYPE, EXPONENT>
	template <typename LHS, typename RHS = LHS>
	using trunc_multiply_result_t = make_fixed_from_repr<
		_impl::common_repr_type<typename LHS::repr_type, typename RHS::repr_type>,
		LHS::integer_digits + RHS::integer_digits>;

	// as trunc_multiply_result_t but converts parameter, factor,
	// ready for safe binary multiply
	template <typename LHS, typename RHS>
	trunc_multiply_result_t<LHS, RHS>
	constexpr trunc_multiply(LHS const & lhs, RHS const & rhs) noexcept
	{
		using result_type = trunc_multiply_result_t<LHS, RHS>;
		return _impl::multiply<result_type>(lhs, rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_square_result_t / trunc_square

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<REPR_TYPE, EXPONENT>
	// whose sign bit is set to the same value
	template <typename FIXED_POINT>
	using trunc_square_result_t = make_fixed_from_repr<
		typename _impl::make_unsigned<typename FIXED_POINT::repr_type>::type,
		FIXED_POINT::integer_digits * 2>;

	// as trunc_square_result_t but converts parameter, factor,
	// ready for safe binary multiply-by-self
	template <typename FIXED_POINT>
	trunc_square_result_t<FIXED_POINT>
	constexpr trunc_square(const FIXED_POINT & root) noexcept
	{
		using result_type = trunc_square_result_t<FIXED_POINT>;
		return _impl::multiply<result_type>(root, root);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trunc_sqrt_result_t / trunc_sqrt

	// yields specialization of fixed_point with integral bits necessary to store
	// the positive result of a square root operation on an object of type,
	// fixed_point<REPR_TYPE, EXPONENT>
	template <typename FIXED_POINT>
	using trunc_sqrt_result_t = make_fixed_from_repr<
		typename _impl::make_unsigned<typename FIXED_POINT::repr_type>::type,
		(FIXED_POINT::integer_digits + 1) / 2>;

	// as trunc_sqrt_result_t but converts parameter, factor,
	// ready for safe sqrt operation
	template <typename FIXED_POINT>
	trunc_sqrt_result_t<FIXED_POINT>
	constexpr trunc_sqrt(const FIXED_POINT & square) noexcept
	{
		using output_type = trunc_sqrt_result_t<FIXED_POINT>;
		return output_type(sqrt(square));
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::promote_multiply_result_t / promote_multiply

	// yields specialization of fixed_point with capacity necessary to store
	// result of a multiply between values of fixed_point<REPR_TYPE, EXPONENT>
	template <typename LHS, typename RHS = LHS>
	using promote_multiply_result_t = fixed_point_promotion_t<common_type<LHS, RHS>>;

	// as promote_multiply_result_t but converts parameter, factor,
	// ready for safe binary multiply
	template <typename LHS, typename RHS>
	promote_multiply_result_t<LHS, RHS>
		constexpr promote_multiply(const LHS & lhs, const RHS & rhs) noexcept
	{
		using result_type = promote_multiply_result_t<LHS, RHS>;
		return _impl::multiply<result_type>(lhs, rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::promote_square_result_t / promote_square

	// yields specialization of fixed_point with integral bits necessary to store
	// result of a multiply between values of fixed_point<REPR_TYPE, EXPONENT>
	// whose sign bit is set to the same value
	template <typename FIXED_POINT>
	using promote_square_result_t = make_ufixed<
		FIXED_POINT::integer_digits * 2,
		FIXED_POINT::fractional_digits * 2>;

	// as promote_square_result_t but converts parameter, factor,
	// ready for safe binary multiply-by-self
	template <typename FIXED_POINT>
	promote_square_result_t<FIXED_POINT>
		constexpr promote_square(const FIXED_POINT & root) noexcept
	{
		using output_type = promote_square_result_t<FIXED_POINT>;
		using output_repr_type = typename output_type::repr_type;
		return output_type::from_data(
			_impl::shift_left<(FIXED_POINT::exponent * 2 - output_type::exponent), output_repr_type>(
				static_cast<output_repr_type>(root.data()) * static_cast<output_repr_type>(root.data())));
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point streaming - (placeholder implementation)

	template <typename REPR_TYPE, int EXPONENT>
	::std::ostream & operator << (::std::ostream & out, fixed_point<REPR_TYPE, EXPONENT> const & fp)
	{
		return out << static_cast<long double>(fp);
	}

	template <typename REPR_TYPE, int EXPONENT>
	::std::istream & operator >> (::std::istream & in, fixed_point<REPR_TYPE, EXPONENT> & fp)
	{
		long double ld;
		in >> ld;
		fp = ld;
		return in;
	}

	////////////////////////////////////////////////////////////////////////////////
	// fixed_point arithmetic operator overloads

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT> operator-(
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		static_assert(_impl::is_signed<REPR_TYPE>::value, "unary negation of unsigned value");

		return fixed_point<REPR_TYPE, EXPONENT>::from_data(-rhs.data());
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT> operator+(
		fixed_point<REPR_TYPE, EXPONENT> const & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return fixed_point<REPR_TYPE, EXPONENT>::from_data(lhs.data() + rhs.data());
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT> operator-(
		fixed_point<REPR_TYPE, EXPONENT> const & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return fixed_point<REPR_TYPE, EXPONENT>::from_data(lhs.data() - rhs.data());
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT> operator*(
		fixed_point<REPR_TYPE, EXPONENT> const & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return _impl::multiply<fixed_point<REPR_TYPE, EXPONENT>>(lhs, rhs);
	}

	template <typename REPR_TYPE, int EXPONENT>
	constexpr fixed_point<REPR_TYPE, EXPONENT> operator/(
		fixed_point<REPR_TYPE, EXPONENT> const & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return fixed_point<REPR_TYPE, EXPONENT>::from_data(
			REPR_TYPE(_impl::shift_right<EXPONENT, _impl::next_size_t<REPR_TYPE>>(lhs.data()) / rhs.data()));
	}

	template <typename REPR_TYPE, int EXPONENT>
	fixed_point<REPR_TYPE, EXPONENT> & operator+=(
		fixed_point<REPR_TYPE, EXPONENT> & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return lhs = lhs + rhs;
	}

	template <typename REPR_TYPE, int EXPONENT>
	fixed_point<REPR_TYPE, EXPONENT> & operator-=(
		fixed_point<REPR_TYPE, EXPONENT> & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return lhs = lhs - rhs;
	}

	template <typename REPR_TYPE, int EXPONENT>
	fixed_point<REPR_TYPE, EXPONENT> & operator*=(
		fixed_point<REPR_TYPE, EXPONENT> & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return lhs = lhs * rhs;
	}

	template <typename REPR_TYPE, int EXPONENT>
	fixed_point<REPR_TYPE, EXPONENT> & operator/=(
		fixed_point<REPR_TYPE, EXPONENT> & lhs,
		fixed_point<REPR_TYPE, EXPONENT> const & rhs) noexcept
	{
		return lhs = lhs / rhs;
	}
}

#endif	// defined(_SG14_FIXED_POINT)
