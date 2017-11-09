
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions supporting generic treatment of numeric types

#if !defined(CNL_ENCOMPASSES)
#define CNL_ENCOMPASSES 1

#include <cnl/num_traits.h>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::encompasses

        template<class T, class Enable = void>
        struct unsigned_or_float;

        template<class T>
        struct unsigned_or_float<T, enable_if_t<numeric_limits<T>::is_iec559>> {
            using type = T;
        };

        template<class T>
        struct unsigned_or_float<T, enable_if_t<!numeric_limits<T>::is_iec559>> : make_unsigned<T> {
        };

        template<class T>
        using unsigned_or_float_t = typename unsigned_or_float<T>::type;

        template<class Encompasser, class Encompassed, class Enable = void>
        struct encompasses_lower;

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<numeric_limits<Encompasser>::is_signed
                        && numeric_limits<Encompassed>::is_signed>> {
        static constexpr bool value = numeric_limits<Encompasser>::lowest()
                <=numeric_limits<Encompassed>::lowest();
        };

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!numeric_limits<Encompassed>::is_signed>> : std::true_type {
        };

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!numeric_limits<Encompasser>::is_signed
                        && numeric_limits<Encompassed>::is_signed>> : std::false_type {
        };

        // true if Encompassed can be cast to Encompasser without chance of overflow
        template<class Encompasser, class Encompassed>
        struct encompasses {
            static constexpr bool _lower = encompasses_lower<Encompasser, Encompassed>::value;
            static constexpr bool _upper =
                    static_cast<unsigned_or_float_t<Encompasser>>(numeric_limits<Encompasser>::max())
                            >=static_cast<unsigned_or_float_t<Encompassed>>(numeric_limits<Encompassed>::max());

            static constexpr bool value = _lower && _upper;
        };
    }
}
#endif  // CNL_ENCOMPASSES
