
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if (__cplusplus>=201402L)

#include <cnl/elastic_integer.h>
#include <cnl/safe_integer.h>

#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <map>
#include <memory>

using namespace cnl;
using namespace std;
using _impl::identical;

namespace stl {
    template<typename T>
    using Composite = map<string, vector<unique_ptr<T>>>;
}

namespace prototypes {
    template<typename Rep>
    class safe_integer {
    public:
        template<typename Input>
        constexpr safe_integer(Input const& r)
                :_rep(r) { }

        constexpr Rep const& data() const { return _rep; }

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
    class good_safe_integer;

    using good1 = good_safe_integer<int>;

    using good2 = safe_integer<int32_t>;

    // bad
    template<int Digits, bool IsSigned>
    class bad_safe_integer;

    using bad1 = bad_safe_integer<31, true>;

    using bad2 = bad_safe_integer<cnl::numeric_limits<int>::digits, true>;
}

#if defined(CNL_EXCEPTIONS_ENABLED)
TEST(cppnow2017, safe_integer_example)
{
    // multiplication of safe_integer<int> cannot exceed numeric limits
    EXPECT_THROW(safe_integer<int32_t>{cnl::numeric_limits<int32_t>::max()}*2, overflow_error);

    // difference from safe_integer<unsigned> cannot be negative
    EXPECT_THROW(safe_integer<unsigned>{0}-1, overflow_error);

    // conversion to safe_integer<char> cannot exceed numeric limits
    EXPECT_THROW(safe_integer<short>{cnl::numeric_limits<double>::max()}, overflow_error);

    // value of safe_integer<int> cannot be indeterminate
    //auto d = safe_integer<int>{};  // compiler error? exception? zero-initialization?
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
        using integer = safe_integer<Rep>;
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
        using integer = safe_integer<Rep>;
#endif
    }

#if defined(NDEBUG)
    static_assert(is_same<acme::integer<int>, int>::value, "error in CppNow 2017 slide");
#else
    static_assert(is_same<acme::integer<int>, safe_integer<int>>::value, "error in CppNow 2017 slide");
#endif

    auto square(acme::integer<short> f)
    {
        return f*f;
    }

#if defined(NDEBUG)
    static_assert(is_same<decltype(square(2)), int>::value, "error in CppNow 2017 slide");
#else
    static_assert(is_same<decltype(square(2)), safe_integer<int>>::value, "error in CppNow 2017 slide");
#endif
}

namespace operator_overload1 {
    using prototypes::safe_integer;

    template<typename Rep>
    auto operator*(safe_integer<Rep> const& a, safe_integer<Rep> const& b)
    {
        Rep product = a.data()*b.data();

        // do some overflow checking

        return safe_integer<Rep>{product};
    }

    static_assert(
            is_same<decltype(safe_integer<short>{2}*safe_integer<short>{3}), safe_integer<short>>::value,
            "error in CppNow 2017 slide");

    // error: no match for ‘operator*’
    //auto x = safe_integer<short>{6} * safe_integer<int>{7};
}

namespace operator_overload2 {
    using prototypes::safe_integer;

    template<typename Rep1, typename Rep2>
    auto operator*(safe_integer<Rep1> const& a, safe_integer<Rep2> const& b)
    {
        auto product = a.data()*b.data();

        // do some overflow checking

        return safe_integer<decltype(product)>{product};
    }

    static_assert(
            is_same<decltype(safe_integer<short>{2}*safe_integer<short>{3}), safe_integer<int>>::value,
            "error in CppNow 2017 slide");

    static_assert(
            is_same<decltype(safe_integer<short>{6}*safe_integer<int>{7}), safe_integer<int>>::value,
            "error in CppNow 2017 slide");
}

namespace composite {
    using prototypes::safe_integer;

    template<int Digits, typename Narrowest = int>
    using elastic_integer = cnl::elastic_integer<Digits, Narrowest>;

    template<int Digits, typename Narrowest = int>
    using safe_elastic_integer =
    safe_integer<elastic_integer<Digits, Narrowest>>;

    template<typename Rep1, typename Rep2>
    constexpr auto operator*(safe_integer<Rep1> const& a, safe_integer<Rep2> const& b)
    {
        auto product = a.data()*b.data();

        if (cnl::numeric_limits<Rep1>::digits+cnl::numeric_limits<Rep2>::digits
                >cnl::numeric_limits<decltype(product)>::digits) {
            // do some overflow checking
        }

        return safe_integer<decltype(product)>{product};
    }

    auto a = safe_elastic_integer<4>{14}*safe_elastic_integer<3>{6};
    static_assert(is_same<decltype(a), safe_elastic_integer<7>>::value, "error in CppNow 2017 slide");
}

#endif  // (__cplusplus>=201402L)
