
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_CONVERT_H)
#define CNL_IMPL_ROUNDING_CONVERT_H

#include "../../limits.h"
#include "../native_tag.h"
#include "../type_traits/enable_if.h"
#include "rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Result, typename Input>
    struct convert<_impl::native_rounding_tag, Result, Input>
            : convert<_impl::native_tag, Result, Input> {
    };

    template<typename Result, typename Input>
    struct convert<
            _impl::nearest_rounding_tag, Result, Input,
            _impl::enable_if_t<
                    cnl::numeric_limits<Result>::is_integer && std::is_floating_point<Input>::value>> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from+((from>=0) ? .5 : -.5));
        }
    };

    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   std::is_floating_point<Result>::value && std::is_floating_point<Input>::value >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };

    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_integer >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };

    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   std::is_floating_point<Result>::value && cnl::numeric_limits<Input>::is_integer >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };
}

#endif  // CNL_IMPL_ROUNDING_CONVERT_H
