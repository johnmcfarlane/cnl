
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_H)
#define CNL_IMPL_WIDE_INTEGER_H

#include "../constant.h"
#include "../integer.h"
#include "num_traits/from_value.h"
#include "num_traits/width.h"
#include "numbers/set_signedness.h"
#include "numbers/signedness.h"

#if defined(CNL_INT128_ENABLED)
#define WIDE_INTEGER_HAS_LIMB_TYPE_UINT64
#endif
#define WIDE_INTEGER_NAMESPACE cnl::_impl  // NOLINT(cppcoreguidelines-macro-usage)
#include "ckormanyos/uintwide_t.h"

#include <type_traits>

namespace cnl::_impl {
    template<typename T>
    inline constexpr auto is_uintwide_v = false;

    template<std::uint32_t Width, typename LimbType, typename AllocatorType, bool IsSigned>
    inline constexpr auto is_uintwide_v<
            math::wide_integer::uintwide_t<Width, LimbType, AllocatorType, IsSigned>> = true;

    template<typename T>
    concept any_uintwide = is_uintwide_v<T>;

    template<int Width, integer Narrowest>
    struct make_uintwide {
        using limb = numbers::set_signedness_t<Narrowest, false>;

        static constexpr auto is_signed{numbers::signedness_v<Narrowest>};

        static constexpr auto min_width{Width + is_signed};
        static constexpr auto limb_width{width<limb>};
        static constexpr auto min_limbs{(min_width + limb_width - 1) / limb_width};
        static constexpr auto rounded_up_width{min_limbs * limb_width};

        using type = math::wide_integer::uintwide_t<rounded_up_width, limb, void, is_signed>;
    };
}

namespace cnl {
    template<std::uint32_t Width, typename LimbType, typename AllocatorType, bool IsSigned>
    struct is_integer<
            _impl::math::wide_integer::uintwide_t<
                    Width,
                    LimbType,
                    AllocatorType,
                    IsSigned>>
        : std::true_type {
    };

    template<std::uint32_t Width, typename LimbType, typename AllocatorType>
    inline constexpr int
            digits_v<_impl::math::wide_integer::uintwide_t<Width, LimbType, AllocatorType, false>> = Width;

    template<std::uint32_t Width, typename LimbType, typename AllocatorType>
    inline constexpr int
            digits_v<_impl::math::wide_integer::uintwide_t<Width, LimbType, AllocatorType, true>> = Width - 1;

    template<std::uint32_t Width, typename LimbType, typename AllocatorType, bool IsSigned, typename Value>
    struct from_value<
            _impl::math::wide_integer::uintwide_t<Width, LimbType, AllocatorType, IsSigned>,
            Value> {
        [[nodiscard]] constexpr auto operator()(Value const& value) const
        {
            constexpr auto width = _impl::width<Value>;
            constexpr auto is_signed = numbers::signedness_v<Value>;
            using result_type = _impl::math::wide_integer::uintwide_t<width, LimbType, AllocatorType, is_signed>;
            return result_type{value};
        }
    };

    template<_impl::any_uintwide UIntWide, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator<<(UIntWide const& lhs, constant<Value>)
    {
        return lhs << Value;
    }

    template<_impl::any_uintwide UIntWide, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto operator>>(UIntWide const& lhs, constant<Value>)
    {
        return lhs >> Value;
    }
}

namespace cnl::numbers {
    template<std::uint32_t Width, typename LimbType, typename AllocatorType, bool IsSigned>
    struct signedness<cnl::_impl::math::wide_integer::uintwide_t<Width, LimbType, AllocatorType, IsSigned>>
        : std::bool_constant<IsSigned> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_H
