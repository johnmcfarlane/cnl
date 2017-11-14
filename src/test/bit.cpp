
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/bit.h>

#include <cnl/bit.h>

using cnl::_impl::identical;

namespace {
    namespace test_rotl {
        static_assert(identical(cnl::rotl(cnl::uint8(0xff), 2783), cnl::uint8{0xff}), "cnl::rotl<uint8_t>");
        static_assert(identical(cnl::rotl(cnl::uint8{0x80}, 0xffa), cnl::uint8{0x2}), "cnl::rotl<uint8_t>");
        static_assert(identical(cnl::rotl(cnl::uint8{0x14}, unsigned(-5)), cnl::uint8{0xa0}), "cnl::rotl<uint8_t>");

        static_assert(identical(cnl::rotl(6u, 6), 384u), "cnl::rotl<unsigned>");
        static_assert(identical(cnl::rotl(0x12345678u, 9), 0x68ACF024u), "cnl::rotl<unsigned>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::rotl(CNL_UINTMAX_C(0x12345678), 9), CNL_UINTMAX_C(0x2468ACF000)),
                "cnl::rotl<unsigned>");
        static_assert(
                identical(cnl::rotl(CNL_UINTMAX_C(0x12345678), 105), CNL_UINTMAX_C(0x68ACF000000000000000000000000024)),
                "cnl::rotl<unsigned>");
#endif
    }

    namespace test_rotr {
        static_assert(identical(cnl::rotr(cnl::uint8{0xff}, 2783), cnl::uint8{0xff}), "cnl::rotr<uint8_t>");
        static_assert(identical(cnl::rotr(cnl::uint8{0x80}, 0xffa), cnl::uint8{0x20}), "cnl::rotr<uint8_t>");
        static_assert(identical(cnl::rotr(cnl::uint8{0x14}, unsigned(-5)), cnl::uint8{0x82}), "cnl::rotr<uint8_t>");

        static_assert(identical(cnl::rotr(6u, 6), 402653184u), "cnl::rotr<unsigned>");
        static_assert(identical(cnl::rotr(0x12345678u, 7), 0xF02468ACu), "cnl::rotr<unsigned>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::rotr(CNL_UINTMAX_C(0x12345678), -9), CNL_UINTMAX_C(0x2468ACF000)),
                "cnl::rotr<unsigned>");
        static_assert(identical(cnl::rotr(CNL_UINTMAX_C(0x12345678), -105),
                CNL_UINTMAX_C(0x68ACF000000000000000000000000024)), "cnl::rotr<unsigned>");
#endif
    }

    namespace test_countl_zero {
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x00}), 8), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x01}), 7), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x7E}), 1), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x7F}), 1), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x80}), 0), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0x81}), 0), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0xFE}), 0), "cnl::countl_zero<uint8_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint8{0xFF}), 0), "cnl::countl_zero<uint8_t>");

        static_assert(identical(cnl::countl_zero(cnl::uint16{0x0000}), 16), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0x0001}), 15), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0x7FFE}), 1), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0x7FFF}), 1), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0x8000}), 0), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0x8001}), 0), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0xFFFE}), 0), "cnl::countl_zero<uint16_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint16{0xFFFF}), 0), "cnl::countl_zero<uint16_t>");

        static_assert(identical(cnl::countl_zero(cnl::uint32{0x00000000}), 32), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0x00000001}), 31), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0x7FFFFFFE}), 1), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0x7FFFFFFF}), 1), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0x80000000}), 0), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0x80000001}), 0), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0xFFFFFFFE}), 0), "cnl::countl_zero<uint32_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint32{0xFFFFFFFF}), 0), "cnl::countl_zero<uint32_t>");

        static_assert(identical(cnl::countl_zero(cnl::uint64{0x0000000000000000}), 64), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0x0000000000000001}), 63), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 1), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 1), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0x8000000000000000}), 0), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0x8000000000000001}), 0), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 0), "cnl::countl_zero<uint64_t>");
        static_assert(identical(cnl::countl_zero(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 0), "cnl::countl_zero<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countl_zero(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 127), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 1), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 1), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 0), "cnl::countl_zero<uint128_t>");
        static_assert(identical(cnl::countl_zero(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countl_zero<uint128_t>");
#endif
    }

    namespace test_countl_one {
        static_assert(identical(cnl::countl_one(cnl::uint8{0x00}), 0), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0x01}), 0), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0x7E}), 0), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0x7F}), 0), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0x80}), 1), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0x81}), 1), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0xFE}), 7), "cnl::countl_one<uint8_t>");
        static_assert(identical(cnl::countl_one(cnl::uint8{0xFF}), 8), "cnl::countl_one<uint8_t>");

        static_assert(identical(cnl::countl_one(cnl::uint16{0x0000}), 0), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0x0001}), 0), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0x7FFE}), 0), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0x7FFF}), 0), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0x8000}), 1), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0x8001}), 1), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0xFFFE}), 15), "cnl::countl_one<uint16_t>");
        static_assert(identical(cnl::countl_one(cnl::uint16{0xFFFF}), 16), "cnl::countl_one<uint16_t>");

        static_assert(identical(cnl::countl_one(cnl::uint32{0x00000000}), 0), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0x00000001}), 0), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0x7FFFFFFE}), 0), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0x7FFFFFFF}), 0), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0x80000000}), 1), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0x80000001}), 1), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0xFFFFFFFE}), 31), "cnl::countl_one<uint32_t>");
        static_assert(identical(cnl::countl_one(cnl::uint32{0xFFFFFFFF}), 32), "cnl::countl_one<uint32_t>");

        static_assert(identical(cnl::countl_one(cnl::uint64{0x0000000000000000}), 0), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0x0000000000000001}), 0), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 0), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 0), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0x8000000000000000}), 1), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0x8000000000000001}), 1), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 63), "cnl::countl_one<uint64_t>");
        static_assert(identical(cnl::countl_one(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 64), "cnl::countl_one<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countl_one(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 0), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 1), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 1), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 127), "cnl::countl_one<uint128_t>");
        static_assert(identical(cnl::countl_one(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countl_one<uint128_t>");
#endif
    }

    namespace test_countr_zero {
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x00}), 8), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x01}), 0), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x7E}), 1), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x7F}), 0), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x80}), 7), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0x81}), 0), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0xFE}), 1), "cnl::countr_zero<uint8_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint8{0xFF}), 0), "cnl::countr_zero<uint8_t>");

        static_assert(identical(cnl::countr_zero(cnl::uint16{0x0000}), 16), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0x0001}), 0), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0x7FFE}), 1), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0x7FFF}), 0), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0x8000}), 15), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0x8001}), 0), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0xFFFE}), 1), "cnl::countr_zero<uint16_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint16{0xFFFF}), 0), "cnl::countr_zero<uint16_t>");

        static_assert(identical(cnl::countr_zero(cnl::uint32{0x00000000}), 32), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0x00000001}), 0), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0x7FFFFFFE}), 1), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0x7FFFFFFF}), 0), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0x80000000}), 31), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0x80000001}), 0), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0xFFFFFFFE}), 1), "cnl::countr_zero<uint32_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint32{0xFFFFFFFF}), 0), "cnl::countr_zero<uint32_t>");

        static_assert(identical(cnl::countr_zero(cnl::uint64{0x0000000000000000}), 64), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0x0000000000000001}), 0), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 1), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 0), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0x8000000000000000}), 63), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0x8000000000000001}), 0), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 1), "cnl::countr_zero<uint64_t>");
        static_assert(identical(cnl::countr_zero(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 0), "cnl::countr_zero<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countr_zero(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 0), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 1), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::countr_zero<uint128_t>");
        static_assert(identical(cnl::countr_zero(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countr_zero<uint128_t>");
#endif
    }

    namespace test_countr_one {
        static_assert(identical(cnl::countr_one(cnl::uint8{0x00}), 0), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0x01}), 1), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0x7E}), 0), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0x7F}), 7), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0x80}), 0), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0x81}), 1), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0xFE}), 0), "cnl::countr_one<uint8_t>");
        static_assert(identical(cnl::countr_one(cnl::uint8{0xFF}), 8), "cnl::countr_one<uint8_t>");

        static_assert(identical(cnl::countr_one(cnl::uint16{0x0000}), 0), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0x0001}), 1), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0x7FFE}), 0), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0x7FFF}), 15), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0x8000}), 0), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0x8001}), 1), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0xFFFE}), 0), "cnl::countr_one<uint16_t>");
        static_assert(identical(cnl::countr_one(cnl::uint16{0xFFFF}), 16), "cnl::countr_one<uint16_t>");

        static_assert(identical(cnl::countr_one(cnl::uint32{0x00000000}), 0), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0x00000001}), 1), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0x7FFFFFFE}), 0), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0x7FFFFFFF}), 31), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0x80000000}), 0), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0x80000001}), 1), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0xFFFFFFFE}), 0), "cnl::countr_one<uint32_t>");
        static_assert(identical(cnl::countr_one(cnl::uint32{0xFFFFFFFF}), 32), "cnl::countr_one<uint32_t>");

        static_assert(identical(cnl::countr_one(cnl::uint64{0x0000000000000000}), 0), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0x0000000000000001}), 1), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 0), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 63), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0x8000000000000000}), 0), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0x8000000000000001}), 1), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 0), "cnl::countr_one<uint64_t>");
        static_assert(identical(cnl::countr_one(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 64), "cnl::countr_one<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countr_one(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 1), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 0), "cnl::countr_one<uint128_t>");
        static_assert(identical(cnl::countr_one(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countr_one<uint128_t>");
#endif
    }

    namespace test_popcount {
        static_assert(identical(cnl::popcount(cnl::uint8{0x00}), 0), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0x01}), 1), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0x7E}), 6), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0x7F}), 7), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0x80}), 1), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0x81}), 2), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0xFE}), 7), "cnl::popcount<uint8_t>");
        static_assert(identical(cnl::popcount(cnl::uint8{0xFF}), 8), "cnl::popcount<uint8_t>");

        static_assert(identical(cnl::popcount(cnl::uint16{0x0000}), 0), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0x0001}), 1), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0x7FFE}), 14), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0x7FFF}), 15), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0x8000}), 1), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0x8001}), 2), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0xFFFE}), 15), "cnl::popcount<uint16_t>");
        static_assert(identical(cnl::popcount(cnl::uint16{0xFFFF}), 16), "cnl::popcount<uint16_t>");

        static_assert(identical(cnl::popcount(cnl::uint32{0x00000000}), 0), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0x00000001}), 1), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0x7FFFFFFE}), 30), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0x7FFFFFFF}), 31), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0x80000000}), 1), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0x80000001}), 2), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0xFFFFFFFE}), 31), "cnl::popcount<uint32_t>");
        static_assert(identical(cnl::popcount(cnl::uint32{0xFFFFFFFF}), 32), "cnl::popcount<uint32_t>");

        static_assert(identical(cnl::popcount(cnl::uint64{0x0000000000000000}), 0), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0x0000000000000001}), 1), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 62), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 63), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0x8000000000000000}), 1), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0x8000000000000001}), 2), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 63), "cnl::popcount<uint64_t>");
        static_assert(identical(cnl::popcount(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 64), "cnl::popcount<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::popcount(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 126), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 1), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 2), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 127), "cnl::popcount<uint128_t>");
        static_assert(identical(cnl::popcount(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::popcount<uint128_t>");
#endif
    }
}
