
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
        // sg14::_type_traits_impl::resize

        template<class FundamentalSignedInteger>
        struct fsi : public std::integral_constant<
                bool,
                std::is_signed<FundamentalSignedInteger>::value
                        && std::is_integral<FundamentalSignedInteger>::value
                        && std::is_fundamental<FundamentalSignedInteger>::value> {
        };

        template<class FundamentalUnsignedInteger>
        struct fui : public std::integral_constant<
                bool,
                std::is_unsigned<FundamentalUnsignedInteger>::value
                        && std::is_integral<FundamentalUnsignedInteger>::value
                        && std::is_fundamental<FundamentalUnsignedInteger>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_type_traits_impl::resize

        // supports sg14::resize specialization for built-in integer types
        template<class Archetype, int NumBytes, class _Enable = void>
        struct resize;

        // specializations
        template<class FundamentalUnsignedInteger>
        struct resize<
                FundamentalUnsignedInteger, 1,
                typename std::enable_if<fui<FundamentalUnsignedInteger>::value>::type> {
            using type = std::uint8_t;
        };
        template<class FundamentalSignedInteger>
        struct resize<
                FundamentalSignedInteger, 1,
                typename std::enable_if<fsi<FundamentalSignedInteger>::value>::type> {
            using type = std::int8_t;
        };

        template<class FundamentalUnsignedInteger>
        struct resize<
                FundamentalUnsignedInteger, 2,
                typename std::enable_if<fui<FundamentalUnsignedInteger>::value>::type> {
            using type = std::uint16_t;
        };
        template<class FundamentalSignedInteger>
        struct resize<
                FundamentalSignedInteger, 2,
                typename std::enable_if<fsi<FundamentalSignedInteger>::value>::type> {
            using type = std::int16_t;
        };

        template<class FundamentalUnsignedInteger>
        struct resize<
                FundamentalUnsignedInteger, 4,
                typename std::enable_if<fui<FundamentalUnsignedInteger>::value>::type> {
            using type = std::uint32_t;
        };
        template<class FundamentalSignedInteger>
        struct resize<
                FundamentalSignedInteger, 4,
                typename std::enable_if<fsi<FundamentalSignedInteger>::value>::type> {
            using type = std::int32_t;
        };

        template<class FundamentalUnsignedInteger>
        struct resize<
                FundamentalUnsignedInteger, 8,
                typename std::enable_if<fui<FundamentalUnsignedInteger>::value>::type> {
            using type = std::uint64_t;
        };
        template<class FundamentalSignedInteger>
        struct resize<
                FundamentalSignedInteger, 8,
                typename std::enable_if<fsi<FundamentalSignedInteger>::value>::type> {
            using type = std::int64_t;
        };

#if defined(_GLIBCXX_USE_INT128)
        template<class FundamentalUnsignedInteger>
        struct resize<
                FundamentalUnsignedInteger, 16,
                typename std::enable_if<fui<FundamentalUnsignedInteger>::value>::type> {
            using type = unsigned __int128;
        };
        template<class FundamentalSignedInteger>
        struct resize<
                FundamentalSignedInteger, 16,
                typename std::enable_if<fsi<FundamentalSignedInteger>::value>::type> {
            using type = __int128;
        };
#endif

        template<class FundamentalInteger, int NumBytes>
        struct resize<
                FundamentalInteger, NumBytes,
                typename std::enable_if<(((1 << NumBytes) & 0xfee8)!=0)>::type> {
            using type = typename resize<FundamentalInteger, NumBytes+1>::type;
        };
    }

    /// resizes a type;
    /// see @ref resize_t for details
    template<class Archetype, int NumBytes>
    struct resize;

    // specialization for built-in integer
    template<class FundamentalInteger, int NumBytes>
    struct resize
            : std::enable_if<
                    std::is_fundamental<FundamentalInteger>::value,
                    _type_traits_impl::resize<FundamentalInteger, NumBytes>>::type {
    };

    /// resizes a type
    ///
    /// \param Archetype the type to resize
    /// \param NumBytes the desired size
    ///
    /// \par Examples
    ///
    /// To resize an native-sized unsigned int to 2-bytes:
    /// \include sg14_resize1.h
    ///
    /// To resize a signed byte type to a built-in signed type of at least 5 bytes:
    /// \include sg14_resize2.h
    ///
    /// To resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes:
    /// \include sg14_resize3.h
    template<class Archetype, int NumBytes>
    using resize_t = typename resize<Archetype, NumBytes>::type;
}

#endif	// _SG14_TYPE_TRAITS
