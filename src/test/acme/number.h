
//          Copyright John McFarlane 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_SRC_TEST_ACME_NUMBER_H)
#define CNL_SRC_TEST_ACME_NUMBER_H

#include <cnl/limits.h>
#include <cnl/num_traits.h>

#include <limits>
#include <type_traits>

namespace acme {
    template<typename Rep>
    class number;

    namespace _impl {
        template<typename T>
        struct is_number : std::false_type {
        };

        template<typename Rep>
        struct is_number<number<Rep>> : std::true_type {
        };
    }

    template<typename Rep>
    number<Rep> make_number(Rep const& src);

    template<typename LhsRep, typename Rhs>
    constexpr auto operator*(number<LhsRep> const& lhs, Rhs const& rhs);

    template<typename LhsRep, typename Rhs>
    constexpr auto operator>>(number<LhsRep> const& lhs, Rhs const& rhs);

    template<typename LhsRep, typename Rhs>
    constexpr auto operator<<(number<LhsRep> const& lhs, Rhs const& rhs);

    template<typename Lhs, typename RhsRep>
    constexpr auto operator>>(Lhs const& lhs, number<RhsRep> const& rhs);

    template<typename Rep>
    class number {
        static_assert(!_impl::is_number<Rep>::value, "Cannot nested number in number.");
    public:
        template<typename Src,
                cnl::_impl::enable_if_t<cnl::numeric_limits<Rep>::is_specialized, int> = 0>
        explicit constexpr number(Src const& src)
                : _rep(src) { }

        template<typename Dest,
                cnl::_impl::enable_if_t<cnl::numeric_limits<Rep>::is_specialized, int> = 0>
        explicit constexpr operator Dest()
        {
            return _rep;
        }

        template<typename Rhs>
        friend constexpr auto operator*(number<Rep> const& lhs, Rhs const& rhs)
        {
            return acme::make_number(lhs._rep >> rhs);
        }

        template<typename Rhs>
        friend constexpr auto operator>>(number<Rep> const& lhs, Rhs const& rhs)
        {
            return acme::make_number(lhs._rep >> rhs);
        }

        template<typename Rhs>
        friend constexpr auto operator<<(number<Rep> const& lhs, Rhs const& rhs)
        {
            return acme::make_number(lhs._rep << rhs);
        }

        template<typename Lhs>
        friend constexpr auto operator>>(Lhs const& lhs, number<Rep> const& rhs)
        {
            return acme::make_number(lhs >> rhs._rep);
        }

    private:
        Rep _rep;
    };

    template<typename Rep>
    number<Rep> make_number(Rep const& src)
    {
        return src;
    }
}

namespace std {
    template<typename Rep>
    class numeric_limits<acme::number<Rep>> : public numeric_limits<Rep> {
    };
}

namespace cnl {
    template<typename Rep>
    struct digits<acme::number<Rep>> : digits<Rep> {
    };

    template<typename Rep>
    struct is_signed<acme::number<Rep>> : is_signed<Rep> {
    };

    template<typename Rep, int Digits>
    struct set_digits<acme::number<Rep>, Digits> {
        using type = acme::number<set_digits_t<Rep, Digits>>;
    };
}

#include <cnl/elastic_integer.h>
#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

static_assert(cnl::numeric_limits<acme::number<int>>::is_integer);
static_assert(
        cnl::convert_operator<
                cnl::nearest_rounding_tag,
                cnl::_impl::native_tag,
                cnl::elastic_integer<24, int>,
                acme::number<
                        cnl::scaled_integer<
                                cnl::_impl::number<
                                        cnl::elastic_integer<24, int>,
                                        cnl::nearest_rounding_tag>,
                                cnl::power<-20, 2> > >, void>{}());
//static_assert(
//        cnl::convert<
//                cnl::nearest_rounding_tag,
//                cnl::_impl::native_tag,
//                cnl::elastic_integer<24, int>>(
//                acme::number<
//                        acme::number<
//                                cnl::scaled_integer<
//                                        cnl::_impl::number<
//                                                cnl::elastic_integer<24, int>,
//                                                cnl::nearest_rounding_tag>,
//                                        cnl::power<-20, 2> > > >{42});

#endif  // CNL_SRC_TEST_ACME_NUMBER_H
