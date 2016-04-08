//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `sg14::fixed_point` type

#if !defined(_SG14_TYPE_TRAITS)
#define _SG14_TYPE_TRAITS 1

#include <cinttypes>
#include <tuple>
#include <type_traits>

#include "bits/int128.h"

/// study group 14 of the C++ working group
namespace sg14 {
    namespace _type_traits_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // built-in families

#if defined(_GLIBCXX_USE_INT128)
        using signed_family = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t, __int128>;
        using unsigned_family = std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, unsigned __int128>;
#else
        using signed_family = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;
		using unsigned_family = std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;
#endif

#if defined(_MSC_VER)
        using float_family = std::tuple<float, double>;
#else
        using float_family = std::tuple<float, double, long double>;
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // first_fit

        template<std::size_t MinNumBytes, class Family, class = void>
        struct first_fit;

        template<std::size_t MinNumBytes, class FamilyMembersHead, class ... FamilyMembersTail>
        struct first_fit<
                MinNumBytes,
                std::tuple<FamilyMembersHead, FamilyMembersTail ...>,
                typename std::enable_if<sizeof(FamilyMembersHead)>=MinNumBytes>::type> {
            using type = FamilyMembersHead;
        };

        template<std::size_t MinNumBytes, class FamilyMembersHead, class ... FamilyMembersTail>
        struct first_fit<
                MinNumBytes,
                std::tuple<FamilyMembersHead, FamilyMembersTail ...>,
                typename std::enable_if<sizeof(FamilyMembersHead)<MinNumBytes>::type> {
            using _tail_base = first_fit<MinNumBytes, std::tuple<FamilyMembersTail ...>>;
            using _tail_type = typename _tail_base::type;
        public:
            using type = typename std::conditional<
                    sizeof(FamilyMembersHead)>=MinNumBytes,
                    FamilyMembersHead,
                    _tail_type>::type;
        };

        template<std::size_t MinNumBytes, class Family>
        using first_fit_t = typename first_fit<MinNumBytes, Family>::type;
    }

    /// resizes a type;
    /// can be specialized for any type for which resizing that type makes sense
    ///
    /// \sa resize_t
    template<class Archetype, std::size_t NumBytes>
    struct resize;

    // sg14::resize specialized for 8-bit built-in integers
    template<std::size_t NumBytes>
    struct resize<std::int8_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::signed_family> {
    };
    template<std::size_t NumBytes>
    struct resize<std::uint8_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::unsigned_family> {
    };

    // sg14::resize specialized for 16-bit built-in integers
    template<std::size_t NumBytes>
    struct resize<std::int16_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::signed_family> {
    };
    template<std::size_t NumBytes>
    struct resize<std::uint16_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::unsigned_family> {
    };

    // sg14::resize specialized for 32-bit built-in integers
    template<std::size_t NumBytes>
    struct resize<std::int32_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::signed_family> {
    };
    template<std::size_t NumBytes>
    struct resize<std::uint32_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::unsigned_family> {
    };

    // sg14::resize specialized for 64-bit built-in integers
    template<std::size_t NumBytes>
    struct resize<std::int64_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::signed_family> {
    };
    template<std::size_t NumBytes>
    struct resize<std::uint64_t, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::unsigned_family> {
    };

#if defined(_GLIBCXX_USE_INT128)
    // sg14::resize specialized for 128-bit built-in integers
    template<std::size_t NumBytes>
    struct resize<__int128, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::signed_family> {
    };
    template<std::size_t NumBytes>
    struct resize<unsigned __int128, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::unsigned_family> {
    };
#endif

    // sg14::resize specialized for float
    template<std::size_t NumBytes>
    struct resize<float, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::float_family> {
    };

    // sg14::resize specialized for double
    template<std::size_t NumBytes>
    struct resize<double, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::float_family> {
    };

    // sg14::resize specialized for long double
    template<std::size_t NumBytes>
    struct resize<long double, NumBytes> : _type_traits_impl::first_fit<NumBytes, _type_traits_impl::float_family> {
    };

    /// resizes a type
    ///
    /// \tparam Archetype the type to resize
    /// \tparam NumBytes the desired size
    ///
    /// \par Examples
    ///
    /// To resize an native-sized unsigned int to 2-bytes:
    /// \snippet snippets.cpp use resize 1
    ///
    /// To resize a signed byte type to a built-in signed type of at least 5 bytes:
    /// \snippet snippets.cpp use resize 2
    ///
    /// To resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes:
    /// \snippet snippets.cpp use resize 3
    template<class Archetype, std::size_t NumBytes>
    using resize_t = typename resize<Archetype, NumBytes>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // import selected <type_traits> definitions from std namespace

    // common_type
    template<class ...T>
    struct common_type;

    template<class ...T>
    struct common_type : std::common_type<T...> {
    };

    // is_integral
    template<class T>
    struct is_integral;

    template<class T>
    struct is_integral : std::is_integral<T> {
    };

    // is_signed
    template<class T>
    struct is_signed;

    template<class T>
    struct is_signed : std::is_signed<T> {
    };

    // is_unsigned
    template<class T>
    struct is_unsigned;

    template<class T>
    struct is_unsigned : std::is_unsigned<T> {
    };

    // make_signed
    template<class T>
    struct make_signed;

    template<class T>
    struct make_signed : std::make_signed<T> {
    };

    // make_unsigned
    template<class T>
    struct make_unsigned;

    template<class T>
    struct make_unsigned : std::make_unsigned<T> {
    };
}

#endif	// _SG14_TYPE_TRAITS
