#if !defined(_SG14_INTEGER)
#define _SG14_INTEGER 1

#include <limits>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
// _SG14_INTEGER_EXCEPTIONS_ENABLED macro definition 

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)
#error _SG14_INTEGER_EXCEPTIONS_ENABLED already defined
#endif

#if defined(_MSC_VER)
#if defined(_CPPUNWIND)
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif
#elif defined(__clang__) || defined(__GNUG__)
#if defined(__EXCEPTIONS)
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif
#else
#define _SG14_INTEGER_EXCEPTIONS_ENABLED
#endif

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

namespace sg14 {
  ////////////////////////////////////////////////////////////////////////////////
  // macros

  // basic definitions
#define _SG14_INTEGER_COMPARISON_DEFINE(OP) \
    template <class Lhs, class Rhs> \
    constexpr auto operator OP (const Lhs& lhs, const Rhs& rhs) \
    -> typename std::enable_if<sg14::_integer_impl::is_integer_class<Lhs>::value || sg14::_integer_impl::is_integer_class<Rhs>::value, bool>::type { \
        using common_type = typename std::common_type<Lhs, Rhs>::type; \
        return static_cast<common_type>(lhs).data() OP static_cast<common_type>(rhs).data(); }

#define _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(OP) \
    template <class LhsRepr, class RhsRepr, class OverflowPolicy> \
    constexpr auto operator OP (const integer<LhsRepr, OverflowPolicy>& lhs, const integer<RhsRepr, OverflowPolicy>& rhs) \
    -> typename sg14::_integer_impl::common_type<integer<LhsRepr, OverflowPolicy>, integer<RhsRepr, OverflowPolicy>>::type { \
        using Result = typename sg14::_integer_impl::common_type<integer<LhsRepr, OverflowPolicy>, integer<RhsRepr, OverflowPolicy>>::type; \
        return static_cast<Result>(lhs.data() OP rhs.data()); } \
    \
    template <class Lhs, class RhsRepr, class RhsOverflowPolicy, typename std::enable_if<! sg14::_integer_impl::is_integer_class<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<typename sg14::_integer_impl::common_type<Lhs, RhsRepr>::type, RhsOverflowPolicy> { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRepr, class LhsOverflowPolicy, class Rhs, typename std::enable_if<! sg14::_integer_impl::is_integer_class<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<typename sg14::_integer_impl::common_type<LhsRepr, Rhs>::type, LhsOverflowPolicy> { \
        return lhs.data() OP rhs; }

#define _SG14_INTEGER_BIT_SHIFT_DEFINE(OP) \
    template <class LhsRepr, class LhsOverflowPolicy, class RhsRepr, class RhsOverflowPolicy> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return lhs.data() OP rhs.data(); } \
    \
    template <class Lhs, class RhsRepr, class RhsOverflowPolicy, typename std::enable_if<! sg14::_integer_impl::is_integer_class<Lhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> Lhs { \
        return lhs OP rhs.data(); } \
    \
    template <class LhsRepr, class LhsOverflowPolicy, class Rhs, typename std::enable_if<! sg14::_integer_impl::is_integer_class<Rhs>::value, int>::type dummy = 0> \
    constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return integer<LhsRepr, LhsOverflowPolicy>(lhs.data() OP rhs); }

  ////////////////////////////////////////////////////////////////////////////////
  // forward-declarations

  template<typename Repr, typename OverflowPolicy>
  class integer;

  namespace _integer_impl {
    template<class, class, class = void>
    struct common_type;
  };

  namespace _integer_impl {
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::_integer_impl::is_integer_class - trait to identify sg14::integer<>

    template <typename T>
    struct is_integer_class;

    template <typename T>
    struct is_integer_class
            : std::false_type {};

    template <typename Repr, typename OverflowPolicy>
    struct is_integer_class<integer<Repr, OverflowPolicy>>
            : std::true_type {};

    ////////////////////////////////////////////////////////////////////////////////
    // overflow detection

    // positive_digits
    template<typename T>
    struct positive_digits: public std::integral_constant<int, std::numeric_limits<T>::digits> { };

    template<typename T>
    struct negative_digits: public std::integral_constant<int,
                                                          std::is_signed<T>::value ? std::numeric_limits<T>::digits
                                                                                   : 0> {
    };

    // is_positive_overflow
    template<
            typename Destination, typename Source,
            typename std::enable_if<
                    !(positive_digits<Destination>::value<positive_digits<Source>::value),
                    int>::type dummy = 0>
    constexpr bool is_positive_overflow(Source const&)
    {
        static_assert(! is_integer_class<Destination>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

        // If positive capacity of Destination is equal to or exceeds that of Source,
        // positive overflow cannot occur.
        return false;
    }

    template<
            typename Destination, typename Source,
            typename std::enable_if<
                    (positive_digits<Destination>::value<positive_digits<Source>::value),
                    int>::type dummy = 0>
    constexpr bool is_positive_overflow(Source const& source)
    {
        static_assert(! is_integer_class<Destination>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

        return source>static_cast<Source>(std::numeric_limits<Destination>::max());
    }

    // is_negative_overflow
    template<
            typename Destination, typename Source,
            typename std::enable_if<
                    !(negative_digits<Destination>::value<negative_digits<Source>::value),
                    int>::type dummy = 0>
    constexpr bool is_negative_overflow(Source const&)
    {
        static_assert(! is_integer_class<Destination>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

        // If positive capacity of Destination is equal to or exceeds that of Source,
        // positive overflow cannot occur.
        return false;
    }

    template<
            typename Destination, typename Source,
            typename std::enable_if<
                    (negative_digits<Destination>::value < negative_digits<Source>::value),
                    int>::type dummy = 0>
    constexpr bool is_negative_overflow(Source const& source)
    {
        static_assert(! is_integer_class<Destination>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

        return source<static_cast<Source>(std::numeric_limits<Destination>::lowest());
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  // policies

  struct undefined_overflow_policy {
    template<typename Lhs, typename Rhs>
    constexpr Lhs convert(const Rhs& rhs) const
    {
        return rhs;
    }
  };

#if defined(_SG14_INTEGER_EXCEPTIONS_ENABLED)
  struct throwing_overflow_policy {
      template <typename Lhs, typename Rhs>
      constexpr Lhs convert(const Rhs& rhs) const
      {
          return _integer_impl::is_positive_overflow<Lhs>(rhs)
                 ? throw std::overflow_error("positive overflow in coversion")
                 : _integer_impl::is_negative_overflow<Lhs>(rhs)
                   ? throw std::overflow_error("negative overflow in coversion")
                   : rhs;
      }
  };
#else
  using throwing_overflow_policy = undefined_overflow_policy;
#endif

  struct saturated_overflow_policy {
    template<typename Lhs, typename Rhs>
    constexpr Lhs convert(const Rhs& rhs) const
    {
        static_assert(! _integer_impl::is_integer_class<Lhs>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! _integer_impl::is_integer_class<Rhs>::value, "this function helps convert values *to* sg14::integer");

        using LhsNumericLimits = std::numeric_limits<Lhs>;
        return
                _integer_impl::is_positive_overflow<Lhs>(rhs)
                ? LhsNumericLimits::max()
                : _integer_impl::is_negative_overflow<Lhs>(rhs)
                  ? LhsNumericLimits::lowest()
                  : rhs;
    }
  };

  namespace _integer_impl {
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::_integer_impl::common_type

    // given two integer<>, produces the type that is best suited to both of them
    template<class LhsRepr, class RhsRepr, class OverflowPolicy>
    struct common_type<
            integer<LhsRepr, OverflowPolicy>,
            integer<RhsRepr, OverflowPolicy>> {
      using type = integer<
              typename std::common_type<LhsRepr, RhsRepr>::type,
              OverflowPolicy>;
    };

    // given a integer<> and a built-in integer type,
    // generates a integer<> type that is as big as both of them (or as close as possible)
    template<class LhsRepr, class LhsOverflowPolicy, class RhsInteger>
    struct common_type<
            integer<LhsRepr, LhsOverflowPolicy>,
            RhsInteger,
            typename std::enable_if<
                    ! _integer_impl::is_integer_class<RhsInteger>::value
                            && std::is_integral<RhsInteger>::value
            >::type> {
      using type = typename sg14::integer<typename std::common_type<LhsRepr, RhsInteger>::type, LhsOverflowPolicy>;
    };

    // given a integer<> and a floating-point type,
    // generates a floating-point type that is as big as both of them (or as close as possible)
    template<class LhsRepr, class LhsOverflowPolicy, class Float>
    struct common_type<
            integer<LhsRepr, LhsOverflowPolicy>,
            Float,
            typename std::enable_if<std::is_floating_point<Float>::value>::type> {
      using type = typename std::common_type<LhsRepr, Float>::type;
    };

    // when first type is not integer<> and second type is, reverse the order
    template<class Lhs, class RhsRepr, class RhsOverflowPolicy>
    struct common_type<Lhs, integer<RhsRepr, RhsOverflowPolicy>>
            : common_type<integer<RhsRepr, RhsOverflowPolicy>, Lhs> {
    };
  }

  ////////////////////////////////////////////////////////////////////////////////
  // sg14::integer<>

  // an integer which can be customized to react in different ways to overflow
  template<typename Repr, typename OverflowPolicy>
  class integer {
  public:
      ////////////////////////////////////////////////////////////////////////////////
      // types

      using repr_type = Repr;
      using overflow = OverflowPolicy;

      ////////////////////////////////////////////////////////////////////////////////
      // functions

      template<class RhsRepr, typename std::enable_if<! _integer_impl::is_integer_class<RhsRepr>::value, int>::type dummy = 0>
      constexpr explicit integer(const RhsRepr& rhs)
              :repr(OverflowPolicy{}.template convert<repr_type>(rhs))
      {
      }

      template<class Rhs, typename std::enable_if<_integer_impl::is_integer_class<Rhs>::value, int>::type dummy = 0>
      constexpr explicit integer(const Rhs& rhs)
              :repr(OverflowPolicy{}.template convert<repr_type>(rhs.data()))
      {
      }

      template<typename LhsRepr>
      constexpr explicit operator LhsRepr() const
      {
          return repr;
      }

      constexpr friend integer operator-(const integer& rhs) {
          return integer(- rhs.repr);
      }

      constexpr repr_type const& data() const
      {
          return repr;
      }

  private:
      ////////////////////////////////////////////////////////////////////////////////
      // variables

      repr_type repr;
  };

    _SG14_INTEGER_COMPARISON_DEFINE(==);
    _SG14_INTEGER_COMPARISON_DEFINE(!=);
    _SG14_INTEGER_COMPARISON_DEFINE(<);
    _SG14_INTEGER_COMPARISON_DEFINE(>);
    _SG14_INTEGER_COMPARISON_DEFINE(<=);
    _SG14_INTEGER_COMPARISON_DEFINE(>=);

    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(+);
    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(-);
    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(*);
    _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(/);

    _SG14_INTEGER_BIT_SHIFT_DEFINE(>>);
    _SG14_INTEGER_BIT_SHIFT_DEFINE(<<);

  ////////////////////////////////////////////////////////////////////////////////
  // integer<> partial specializations

  template<typename Repr>
  using undefined_integer = integer<Repr, undefined_overflow_policy>;

  template<typename Repr>
  using throwing_integer = integer<Repr, throwing_overflow_policy>;

  template<typename Repr>
  using saturated_integer = integer<Repr, saturated_overflow_policy>;
}

////////////////////////////////////////////////////////////////////////////////
// sg14::integer-specific specializations to std templates

namespace std
{
  // std::is_integral<sg14::integer<>>
  template <typename Repr, typename OverflowPolicy>
  struct is_integral<sg14::integer<Repr, OverflowPolicy>>
          : integral_constant<bool, std::is_integral<Repr>::value> {
  };

  // std::is_unsigned<sg14::integer<>>
  template <typename Repr, typename OverflowPolicy>
  struct is_unsigned<sg14::integer<Repr, OverflowPolicy>>
          : integral_constant<bool, std::is_unsigned<Repr>::value> {
  };

  // std::is_signed<sg14::integer<>>
  template <typename Repr, typename OverflowPolicy>
  struct is_signed<sg14::integer<Repr, OverflowPolicy>>
          : integral_constant<bool, std::is_signed<Repr>::value> {};

    // std::common_type<T, sg14::integer>
    template<
            class Lhs,
            class RhsRepr, class RhsOverflowPolicy>
    struct common_type<
            Lhs,
            sg14::integer<RhsRepr, RhsOverflowPolicy>>
            : sg14::_integer_impl::common_type<
                    Lhs,
                    sg14::integer<RhsRepr, RhsOverflowPolicy>> { };

    // std::common_type<sg14::integer, T>
    template<
            class LhsRepr, class LhsOverflowPolicy,
            class Rhs>
    struct common_type<
            sg14::integer<LhsRepr, LhsOverflowPolicy>,
            Rhs>
            : sg14::_integer_impl::common_type<
                    sg14::integer<LhsRepr, LhsOverflowPolicy>,
                    Rhs> { };

    // std::common_type<sg14::integer, sg14::integer>
    template<
            class LhsRepr, class LhsOverflowPolicy,
            class RhsRepr, class RhsOverflowPolicy>
    struct common_type<
            sg14::integer<LhsRepr, LhsOverflowPolicy>,
            sg14::integer<RhsRepr, RhsOverflowPolicy>>
            : sg14::_integer_impl::common_type<
                    sg14::integer<LhsRepr, LhsOverflowPolicy>,
                    sg14::integer<RhsRepr, RhsOverflowPolicy>> { };
}

#endif	// defined(_SG14_INTEGER)
