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
    template <typename LhsRepr, typename LhsOverflowPolicy, typename RhsRepr, typename RhsOverflowPolicy> \
    friend constexpr bool operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) { \
        return lhs.data() OP rhs.data(); } \
    \
    template <typename Lhs, typename RhsRepr, typename RhsOverflowPolicy, typename std::enable_if<std::is_arithmetic<Lhs>::value, int>::type dummy = 0> \
    friend constexpr bool operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) { \
        using common_type = typename std::common_type<Lhs, RhsRepr>::type; \
        return static_cast<common_type>(lhs) OP static_cast<common_type>(rhs.data()); } \
    \
    template <typename LhsRepr, typename LhsOverflowPolicy, typename Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type dummy = 0> \
    friend constexpr bool operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) { \
        using common_type = typename std::common_type<LhsRepr, Rhs>::type; \
        return static_cast<common_type>(lhs.data()) OP static_cast<common_type>(rhs); }

#define _SG14_INTEGER_BINARY_ARITHMETIC_DEFINE(OP) \
    template <typename LhsRepr, typename LhsOverflowPolicy, typename RhsRepr, typename RhsOverflowPolicy> \
    friend constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> typename sg14::_integer_impl::common_type<integer<LhsRepr, LhsOverflowPolicy>, integer<RhsRepr, RhsOverflowPolicy>>::type { \
        using Result = typename sg14::_integer_impl::common_type<integer<LhsRepr, LhsOverflowPolicy>, integer<RhsRepr, RhsOverflowPolicy>>::type; \
        return static_cast<Result>(lhs.data() OP rhs.data()); } \
    \
    template <typename Lhs, typename RhsRepr, typename RhsOverflowPolicy, typename std::enable_if<std::is_arithmetic<Lhs>::value, int>::type dummy = 0> \
    friend constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<typename sg14::_integer_impl::common_type<Lhs, RhsRepr>::type, RhsOverflowPolicy> { \
        return lhs OP rhs.data(); } \
    \
    template <typename LhsRepr, typename LhsOverflowPolicy, typename Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type dummy = 0> \
    friend constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<typename sg14::_integer_impl::common_type<LhsRepr, Rhs>::type, LhsOverflowPolicy> { \
        return lhs.data() OP rhs; }

#define _SG14_INTEGER_BIT_SHIFT_DEFINE(OP) \
    template <typename LhsRepr, typename LhsOverflowPolicy, typename RhsRepr, typename RhsOverflowPolicy> \
    friend constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return lhs.data() OP rhs.data(); } \
    \
    template <typename Lhs, typename RhsRepr, typename RhsOverflowPolicy, typename std::enable_if<std::is_arithmetic<Lhs>::value, int>::type dummy = 0> \
    friend constexpr auto operator OP (const Lhs& lhs, const integer<RhsRepr, RhsOverflowPolicy>& rhs) \
    -> Lhs { \
        return lhs OP rhs.data(); } \
    \
    template <typename LhsRepr, typename LhsOverflowPolicy, typename Rhs, typename std::enable_if<std::is_arithmetic<Rhs>::value, int>::type dummy = 0> \
    friend constexpr auto operator OP (const integer<LhsRepr, LhsOverflowPolicy>& lhs, const Rhs& rhs) \
    -> integer<LhsRepr, LhsOverflowPolicy> { \
        return lhs.data() OP rhs; }

  ////////////////////////////////////////////////////////////////////////////////
  // forward-declarations

  template<typename Repr, typename OverflowPolicy>
  class integer;

  namespace _integer_impl {
    template<typename Lhs, typename Rhs>
    struct common_type;

    template<typename LhsRepr, typename RhsRepr, typename OverflowPolicy>
    struct common_type<
            integer<LhsRepr, OverflowPolicy>,
            integer<RhsRepr, OverflowPolicy>> {
      using type = integer<typename std::common_type<LhsRepr, RhsRepr>::type, OverflowPolicy>;
    };
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
            typename std::enable_if<!(positive_digits<Destination>::value<positive_digits<Source>::value),
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
            typename std::enable_if<positive_digits<Destination>::value<positive_digits<Source>::value, int>::type
                                    dummy = 0>
    constexpr bool is_positive_overflow(Source const& source)
    {
        static_assert(! is_integer_class<Destination>::value, "this function helps convert values *to* sg14::integer");
        static_assert(! is_integer_class<Source>::value, "this function helps convert values *to* sg14::integer");

        return source>static_cast<Source>(std::numeric_limits<Destination>::max());
    }

    // is_negative_overflow
    template<
            typename Destination, typename Source,
            typename std::enable_if<!(negative_digits<Destination>::value<negative_digits<Source>::value),
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
            typename std::enable_if<negative_digits<Destination>::value<negative_digits<Source>::value, int>::type
                                    dummy = 0>
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

      template<typename RhsRepr, typename std::enable_if<std::is_integral<RhsRepr>::value, int>::type dummy = 0>
      constexpr integer(const RhsRepr& rhs)
              :repr(OverflowPolicy{}.template convert<repr_type>(rhs))
      {
      }

      template<typename RhsRepr, typename std::enable_if<! std::is_integral<RhsRepr>::value, int>::type dummy = 0>
      constexpr explicit integer(const RhsRepr& rhs)
              :repr(OverflowPolicy{}.template convert<repr_type>(rhs))
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

      constexpr repr_type const & data() const
      {
          return repr;
      }

  private:
      ////////////////////////////////////////////////////////////////////////////////
      // variables

      repr_type repr;
  };

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
          : integral_constant<bool, true> {};

  // std::is_unsigned<sg14::integer<>>
  template <typename Repr, typename OverflowPolicy>
  struct is_unsigned<sg14::integer<Repr, OverflowPolicy>>
          : integral_constant<bool, std::is_unsigned<Repr>::value> {};

  // std::is_unsigned<sg14::integer<>>
  template <typename Repr, typename OverflowPolicy>
  struct is_signed<sg14::integer<Repr, OverflowPolicy>>
          : integral_constant<bool, std::is_signed<Repr>::value> {};
}

#endif	// defined(_SG14_INTEGER)
