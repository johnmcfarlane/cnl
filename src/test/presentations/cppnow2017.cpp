
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if (__cplusplus>=201402L)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/elastic_integer.h>
#include <cnl/overflow_integer.h>

#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <map>
#include <memory>

using namespace std;
using cnl::elastic_integer;
using cnl::_impl::identical;

template<typename Rep>
using overflow_integer = cnl::overflow_integer<Rep, cnl::throwing_overflow_tag>;

namespace stl {
    template<typename T>
    using Composite = map<string, vector<unique_ptr<T>>>;
}

namespace prototypes {
    template<typename Rep>
    class overflow_integer {
    public:
        template<typename Input>
        explicit constexpr overflow_integer(Input const& r)
                :_rep(r) { }

        CNL_NODISCARD constexpr Rep const& data() const { return _rep; }

    private:
        Rep _rep;
    };

    template<int Digits, typename Narrowest>
    class elastic_integer {
        // ...
    private:
        using Rep = Narrowest;
        Rep _rep;  // Narrowest or something wider
    };

    // good
    template<typename Rep>
    class good_overflow_int;

    using good1 = good_overflow_int<int>;

    using good2 = overflow_integer<int32_t>;

    // bad
    template<int Digits, bool IsSigned>
    class bad_overflow_int;

    using bad1 = bad_overflow_int<31, true>;

    using bad2 = bad_overflow_int<cnl::numeric_limits<int>::digits, true>;
}

#if defined(CNL_EXCEPTIONS_ENABLED)
TEST(cppnow2017, overflow_int_example)
{
    // multiplication of overflow_integer<int> cannot exceed numeric limits
    EXPECT_THROW((void)(overflow_integer<int32_t>{cnl::numeric_limits<int32_t>::max()}*2), overflow_error);

    // difference from overflow_integer<unsigned> cannot be negative
    EXPECT_THROW((void)(overflow_integer<unsigned>{0}-1), overflow_error);

    // conversion to overflow_integer<char> cannot exceed numeric limits
    EXPECT_THROW((void)(overflow_integer<short>{cnl::numeric_limits<double>::max()}), overflow_error);

    // value of overflow_integer<int> cannot be indeterminate
    //auto d = overflow_integer<int>{};  // compiler error? exception? zero-initialization?
}
#endif

namespace elastic_integer_example {
    // elastic_integer holding 4 digits
    constexpr auto a = elastic_integer<4, unsigned>{10};
    static_assert(identical(a, elastic_integer<4, unsigned>{10}), "error in CppNow 2017 slide");

    // result of addition is 1 digit wider
    constexpr auto b = a+a;  // elastic_integer<5, unsigned>;
    static_assert(identical(b, elastic_integer<5, unsigned>{20}), "error in CppNow 2017 slide");

    // result of subtraction is signed
    constexpr auto c = -b;  // elastic_integer<5, signed>;
    static_assert(identical(c, elastic_integer<5, signed>{-20}), "error in CppNow 2017 slide");

    // run-time overflow is not my concern
    constexpr auto d = elastic_integer<8, signed>{256};
    static_assert(identical(d, elastic_integer<8, signed>{256}), "error in CppNow 2017 slide");
    static_assert(d>cnl::numeric_limits<decltype(d)>::max(), "error in CppNow 2017 slide");
}

namespace acme_ndebug {
#define _NDEBUG

    namespace acme {
#if defined(_NDEBUG)
        template<typename Rep>
        using integer = Rep;
#else
        template<typename Rep>
        using integer = overflow_integer<Rep>;
#endif
    }

    static_assert(is_same<acme::integer<int>, int>::value, "error in CppNow 2017 slide");

    auto square(acme::integer<short> f)
    {
        return f*f;
    }

    static_assert(is_same<decltype(square(2)), int>::value, "error in CppNow 2017 slide");
}

namespace acme_debug {
    namespace acme {
#if defined(NDEBUG)
        template<typename Rep>
        using integer = Rep;
#else
        template<typename Rep>
        using integer = overflow_integer<Rep>;
#endif
    }

#if defined(NDEBUG)
    static_assert(is_same<acme::integer<int>, int>::value, "error in CppNow 2017 slide");
#else
    static_assert(is_same<acme::integer<int>, overflow_integer<int>>::value, "error in CppNow 2017 slide");
#endif

    auto square(acme::integer<short> f)
    {
        return f*f;
    }

#if defined(NDEBUG)
    static_assert(is_same<decltype(square(2)), int>::value, "error in CppNow 2017 slide");
#else
    static_assert(is_same<decltype(square(2)), overflow_integer<int>>::value, "error in CppNow 2017 slide");
#endif
}

namespace operator_overload1 {
    using prototypes::overflow_integer;

    template<typename Rep>
    auto operator*(overflow_integer<Rep> const& a, overflow_integer<Rep> const& b)
    {
        Rep product = a.data()*b.data();

        // do some overflow checking

        return overflow_integer<Rep>{product};
    }

    static_assert(
            is_same<decltype(overflow_integer<short>{2}*overflow_integer<short>{3}), overflow_integer<short>>::value,
            "error in CppNow 2017 slide");

    // error: no match for ‘operator*’
    //auto x = overflow<short>{6} * overflow<int>{7};
}

namespace operator_overload2 {
    using prototypes::overflow_integer;

    template<typename Rep1, typename Rep2>
    auto operator*(overflow_integer<Rep1> const& a, overflow_integer<Rep2> const& b)
    {
        auto product = a.data()*b.data();

        // do some overflow checking

        return overflow_integer<decltype(product)>{product};
    }

    static_assert(
            is_same<decltype(overflow_integer<short>{2}*overflow_integer<short>{3}), overflow_integer<int>>::value,
            "error in CppNow 2017 slide");

    static_assert(
            is_same<decltype(overflow_integer<short>{6}*overflow_integer<int>{7}), overflow_integer<int>>::value,
            "error in CppNow 2017 slide");
}

namespace composite {
    using prototypes::overflow_integer;

    template<int Digits, typename Narrowest = int>
    using elastic_integer = cnl::elastic_integer<Digits, Narrowest>;

    template<int Digits, typename Narrowest = int>
    using safe_elastic_integer =
    overflow_integer<elastic_integer<Digits, Narrowest>>;

    template<typename Rep1, typename Rep2>
    CNL_NODISCARD constexpr auto operator*(overflow_integer<Rep1> const& a, overflow_integer<Rep2> const& b)
    {
        auto product = a.data()*b.data();

        if (cnl::numeric_limits<Rep1>::digits+cnl::numeric_limits<Rep2>::digits
                >cnl::numeric_limits<decltype(product)>::digits) {
            // do some overflow checking
        }

        return overflow_integer<decltype(product)>{product};
    }

    auto a = safe_elastic_integer<4>{14}*safe_elastic_integer<3>{6};
    static_assert(is_same<decltype(a), safe_elastic_integer<7>>::value, "error in CppNow 2017 slide");
}

#endif  // (__cplusplus>=201402L)
