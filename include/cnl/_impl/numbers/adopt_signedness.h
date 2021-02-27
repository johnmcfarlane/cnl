
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBERS_ADOPT_SIGNEDNESS_H)
#define CNL_IMPL_NUMBERS_ADOPT_SIGNEDNESS_H

#include "set_signedness.h"
#include "signedness.h"

namespace cnl::_impl {
    template<typename Mimic, typename Source>
    using adopt_signedness_t = numbers::set_signedness_t<Mimic, numbers::signedness_v<Source>>;
}

#endif  // CNL_IMPL_NUMBERS_ADOPT_SIGNEDNESS_H
