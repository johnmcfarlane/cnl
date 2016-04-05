//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `sg14::fixed_point` type

#if !defined(_SG14_TYPE_TRAITS)
#define _SG14_TYPE_TRAITS 1

#include <cinttypes>
#include <type_traits>

/// study group 14 of the C++ working group
namespace sg14 {
    namespace _type_traits_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_type_traits_impl::resize_family

        template<class ... Types>
        struct resize_family;

        template<>
        struct resize_family<> {
            template<class T>
            using is_member = std::false_type;

            template<int, class Smallest = void>
            using resize = Smallest;
        };

        template<class TypesHead, class ... TypesTail>
        struct resize_family<TypesHead, TypesTail...> {
            template<class T>
            using is_member = typename std::conditional<
                    std::is_same<T, TypesHead>::value,
                    std::true_type,
                    typename resize_family<TypesTail...>::template is_member<T>
            >::type;

            static constexpr std::size_t head_size = sizeof(TypesHead);

            template<int NumBytes, class Smallest = TypesHead>
            using resize = typename resize_family<TypesTail...>::template resize<
                    NumBytes,
                    typename std::conditional<
                            head_size>=NumBytes && head_size<sizeof(Smallest),
                            TypesHead,
                            Smallest>::type>;
        };

        using signed_resize_family = resize_family<
#if defined(_GLIBCXX_USE_INT128)
                __int128,
#endif
                std::int64_t,
                std::int32_t,
                std::int16_t,
                std::int8_t>;

        using unsigned_resize_family = resize_family<
#if defined(_GLIBCXX_USE_INT128)
                unsigned __int128,
#endif
                std::uint64_t,
                std::uint32_t,
                std::uint16_t,
                std::uint8_t>;

        using floating_resize_family = resize_family<
                long double,
                double,
                float>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_type_traits_impl::resize_family_base

        template<
                class Archetype,
                std::size_t NumBytes,
                class Family>
        struct resize_family_base {
            using type = typename Family::template resize<NumBytes>;
        };

        template<class T, class Family>
        constexpr bool is_member()
        {
            return Family::template is_member<T>::value;
        }
    }

    /// resizes a type;
    /// can be specialized for any type for which resizing that type makes sense
    ///
    /// \sa resize_t
    template<class Archetype, std::size_t NumBytes, class = void>
    struct resize;

    // resize<signed-integer, NumBytes>

    template<
            class Archetype,
            typename std::enable_if<
                    _type_traits_impl::is_member<Archetype, _type_traits_impl::signed_resize_family>(), std::size_t>::type NumBytes>
    struct resize<Archetype, NumBytes>
            : _type_traits_impl::resize_family_base<Archetype, NumBytes, _type_traits_impl::signed_resize_family> {
    };

    // resize<unsigned-integer, NumBytes>
    template<
            class Archetype,
            typename std::enable_if<
                    _type_traits_impl::is_member<Archetype, _type_traits_impl::unsigned_resize_family>(), std::size_t>::type NumBytes>
    struct resize<Archetype, NumBytes>
            : _type_traits_impl::resize_family_base<Archetype, NumBytes, _type_traits_impl::unsigned_resize_family> {
    };

    // resize<floating-point, NumBytes>
    template<
            class Archetype,
            typename std::enable_if<
                    _type_traits_impl::is_member<Archetype, _type_traits_impl::floating_resize_family>(), std::size_t>::type NumBytes>
    struct resize<Archetype, NumBytes>
            : _type_traits_impl::resize_family_base<Archetype, NumBytes, _type_traits_impl::floating_resize_family> {
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
}

#endif	// _SG14_TYPE_TRAITS
