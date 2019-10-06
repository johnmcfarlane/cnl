
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/bit.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/bit.h>

using cnl::_impl::identical;

namespace {
    namespace test_rotl {
        static_assert(identical(cnl::rotl(cnl::uint8(0xff), 2783), cnl::uint8{0xff}), "cnl::rotl<uint8_t>");
        static_assert(identical(cnl::rotl(cnl::uint8{0x80}, 0xffa), cnl::uint8{0x2}), "cnl::rotl<uint8_t>");
        static_assert(identical(cnl::rotl(cnl::uint8{0x14}, unsigned(-5)), cnl::uint8{0xa0}), "cnl::rotl<uint8_t>");

        static_assert(identical(cnl::rotl(6U, 6), 384U), "cnl::rotl<unsigned>");
        static_assert(identical(cnl::rotl(0x12345678U, 9), 0x68ACF024U), "cnl::rotl<unsigned>");

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

        static_assert(identical(cnl::rotr(6U, 6), 402653184U), "cnl::rotr<unsigned>");
        static_assert(identical(cnl::rotr(0x12345678U, 7), 0xF02468ACU), "cnl::rotr<unsigned>");

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

    namespace test_ispow2 {
        static_assert(identical(cnl::ispow2(cnl::uint8{0x00}), false), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0x01}), true), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0x7E}), false), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0x7F}), false), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0x80}), true), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0x81}), false), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0xFE}), false), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ispow2(cnl::uint8{0xFF}), false), "cnl::ispow2<uint8_t>");

        static_assert(identical(cnl::ispow2(cnl::uint16{0x0000}), false), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0x0001}), true), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0x7FFE}), false), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0x7FFF}), false), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0x8000}), true), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0x8001}), false), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0xFFFE}), false), "cnl::ispow2<uint16_t>");
        static_assert(identical(cnl::ispow2(cnl::uint16{0xFFFF}), false), "cnl::ispow2<uint16_t>");

        static_assert(identical(cnl::ispow2(cnl::uint32{0x00000000}), false), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0x00000001}), true), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0x7FFFFFFE}), false), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0x7FFFFFFF}), false), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0x80000000}), true), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0x80000001}), false), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0xFFFFFFFE}), false), "cnl::ispow2<uint32_t>");
        static_assert(identical(cnl::ispow2(cnl::uint32{0xFFFFFFFF}), false), "cnl::ispow2<uint32_t>");

        static_assert(identical(cnl::ispow2(cnl::uint64{0x0000000000000000}), false), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0x0000000000000001}), true), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0x7FFFFFFFFFFFFFFE}), false), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0x7FFFFFFFFFFFFFFF}), false), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0x8000000000000000}), true), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0x8000000000000001}), false), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0xFFFFFFFFFFFFFFFE}), false), "cnl::ispow2<uint64_t>");
        static_assert(identical(cnl::ispow2(cnl::uint64{0xFFFFFFFFFFFFFFFF}), false), "cnl::ispow2<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::ispow2(CNL_UINTMAX_C(0)), false), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(CNL_UINTMAX_C(1)), true), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), false), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), false), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), true), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), false), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(~CNL_UINTMAX_C(1)), false), "cnl::ispow2<uint128_t>");
        static_assert(identical(cnl::ispow2(~CNL_UINTMAX_C(0)), false), "cnl::ispow2<uint128_t>");
#endif
    }

    namespace test_ceil2 {
        static_assert(identical(cnl::ceil2(cnl::uint8{0x00}), cnl::uint8{0x00}), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x01}), cnl::uint8{0x01}), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x02}), cnl::uint8{0x02}), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x03}), cnl::uint8{0x04}), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x04}), cnl::uint8{0x04}), "cnl::ispow2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x7E}), cnl::uint8{0x80}), "cnl::ceil2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x7F}), cnl::uint8{0x80}), "cnl::ceil2<uint8_t>");
        static_assert(identical(cnl::ceil2(cnl::uint8{0x80}), cnl::uint8{0x80}), "cnl::ceil2<uint8_t>");

        static_assert(identical(cnl::ceil2(cnl::uint16{0x0000}), cnl::uint16{0}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x0001}), cnl::uint16{1}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x0002}), cnl::uint16{2}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x0003}), cnl::uint16{4}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x0004}), cnl::uint16{4}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x7FFE}), cnl::uint16{0x8000}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x7FFF}), cnl::uint16{0x8000}), "cnl::ceil2<uint16_t>");
        static_assert(identical(cnl::ceil2(cnl::uint16{0x8000}), cnl::uint16{0x8000}), "cnl::ceil2<uint16_t>");

        static_assert(identical(cnl::ceil2(cnl::uint32{0x00000000}), cnl::uint32{0}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x00000001}), cnl::uint32{1}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x00000002}), cnl::uint32{2}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x00000003}), cnl::uint32{4}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x00000004}), cnl::uint32{4}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x7FFFFFFE}), cnl::uint32{0x80000000}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x7FFFFFFF}), cnl::uint32{0x80000000}), "cnl::ceil2<uint32_t>");
        static_assert(identical(cnl::ceil2(cnl::uint32{0x80000000}), cnl::uint32{0x80000000}), "cnl::ceil2<uint32_t>");

        static_assert(identical(cnl::ceil2(cnl::uint64{0x0000000000000000}), cnl::uint64{0}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x0000000000000001}), cnl::uint64{1}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x0000000000000002}), cnl::uint64{2}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x0000000000000003}), cnl::uint64{4}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x0000000000000004}), cnl::uint64{4}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x7FFFFFFFFFFFFFFE}), cnl::uint64{0x8000000000000000}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x7FFFFFFFFFFFFFFF}), cnl::uint64{0x8000000000000000}), "cnl::ceil2<uint64_t>");
        static_assert(identical(cnl::ceil2(cnl::uint64{0x8000000000000000}), cnl::uint64{0x8000000000000000}), "cnl::ceil2<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0)), CNL_UINTMAX_C(0)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(1)), CNL_UINTMAX_C(1)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0x00000000000000000000000000000002)), CNL_UINTMAX_C(2)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0x00000000000000000000000000000003)), CNL_UINTMAX_C(4)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0x00000000000000000000000000000004)), CNL_UINTMAX_C(4)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::ceil2<uint128_t>");
        static_assert(identical(cnl::ceil2(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::ceil2<uint128_t>");
#endif
    }

    namespace test_floor2 {
        static_assert(identical(cnl::floor2(cnl::uint8{0x00}), cnl::uint8{0}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0x01}), cnl::uint8{1}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0x7E}), cnl::uint8{0x40}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0x7F}), cnl::uint8{0x40}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0x80}), cnl::uint8{0x80}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0x81}), cnl::uint8{0x80}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0xFE}), cnl::uint8{0x80}), "cnl::floor2<uint8_t>");
        static_assert(identical(cnl::floor2(cnl::uint8{0xFF}), cnl::uint8{0x80}), "cnl::floor2<uint8_t>");

        static_assert(identical(cnl::floor2(cnl::uint16{0x0000}), cnl::uint16{0}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0x0001}), cnl::uint16{1}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0x7FFE}), cnl::uint16{0x4000}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0x7FFF}), cnl::uint16{0x4000}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0x8000}), cnl::uint16{0x8000}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0x8001}), cnl::uint16{0x8000}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0xFFFE}), cnl::uint16{0x8000}), "cnl::floor2<uint16_t>");
        static_assert(identical(cnl::floor2(cnl::uint16{0xFFFF}), cnl::uint16{0x8000}), "cnl::floor2<uint16_t>");

        static_assert(identical(cnl::floor2(cnl::uint32{0x00000000}), cnl::uint32{0}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0x00000001}), cnl::uint32{1}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0x7FFFFFFE}), cnl::uint32{0x40000000}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0x7FFFFFFF}), cnl::uint32{0x40000000}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0x80000000}), cnl::uint32{0x80000000}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0x80000001}), cnl::uint32{0x80000000}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0xFFFFFFFE}), cnl::uint32{0x80000000}), "cnl::floor2<uint32_t>");
        static_assert(identical(cnl::floor2(cnl::uint32{0xFFFFFFFF}), cnl::uint32{0x80000000}), "cnl::floor2<uint32_t>");

        static_assert(identical(cnl::floor2(cnl::uint64{0x0000000000000000}), cnl::uint64{0}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0x0000000000000001}), cnl::uint64{1}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0x7FFFFFFFFFFFFFFE}), cnl::uint64{0x4000000000000000}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0x7FFFFFFFFFFFFFFF}), cnl::uint64{0x4000000000000000}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0x8000000000000000}), cnl::uint64{0x8000000000000000}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0x8000000000000001}), cnl::uint64{0x8000000000000000}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0xFFFFFFFFFFFFFFFE}), cnl::uint64{0x8000000000000000}), "cnl::floor2<uint64_t>");
        static_assert(identical(cnl::floor2(cnl::uint64{0xFFFFFFFFFFFFFFFF}), cnl::uint64{0x8000000000000000}), "cnl::floor2<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::floor2(CNL_UINTMAX_C(0)), CNL_UINTMAX_C(0)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(CNL_UINTMAX_C(1)), CNL_UINTMAX_C(1)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), CNL_UINTMAX_C(0x40000000000000000000000000000000)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), CNL_UINTMAX_C(0x40000000000000000000000000000000)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(~CNL_UINTMAX_C(1)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::floor2<uint128_t>");
        static_assert(identical(cnl::floor2(~CNL_UINTMAX_C(0)), ~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), "cnl::floor2<uint128_t>");
#endif
    }

    namespace test_log2p1 {
        static_assert(identical(cnl::log2p1(cnl::uint8{0x00}), 0), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0x01}), 1), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0x7E}), 7), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0x7F}), 7), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0x80}), 8), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0x81}), 8), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0xFE}), 8), "cnl::log2p1<uint8_t>");
        static_assert(identical(cnl::log2p1(cnl::uint8{0xFF}), 8), "cnl::log2p1<uint8_t>");

        static_assert(identical(cnl::log2p1(cnl::uint16{0x0000}), 0), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0x0001}), 1), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0x7FFE}), 15), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0x7FFF}), 15), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0x8000}), 16), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0x8001}), 16), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0xFFFE}), 16), "cnl::log2p1<uint16_t>");
        static_assert(identical(cnl::log2p1(cnl::uint16{0xFFFF}), 16), "cnl::log2p1<uint16_t>");

        static_assert(identical(cnl::log2p1(cnl::uint32{0x00000000}), 0), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0x00000001}), 1), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0x7FFFFFFE}), 31), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0x7FFFFFFF}), 31), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0x80000000}), 32), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0x80000001}), 32), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0xFFFFFFFE}), 32), "cnl::log2p1<uint32_t>");
        static_assert(identical(cnl::log2p1(cnl::uint32{0xFFFFFFFF}), 32), "cnl::log2p1<uint32_t>");

        static_assert(identical(cnl::log2p1(cnl::uint64{0x0000000000000000}), 0), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0x0000000000000001}), 1), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 63), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 63), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0x8000000000000000}), 64), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0x8000000000000001}), 64), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 64), "cnl::log2p1<uint64_t>");
        static_assert(identical(cnl::log2p1(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 64), "cnl::log2p1<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::log2p1(CNL_UINTMAX_C(0)), 0), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(CNL_UINTMAX_C(1)), 1), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 127), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 128), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 128), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(~CNL_UINTMAX_C(1)), 128), "cnl::log2p1<uint128_t>");
        static_assert(identical(cnl::log2p1(~CNL_UINTMAX_C(0)), 128), "cnl::log2p1<uint128_t>");
#endif
    }

    namespace test_countl_rsb {
        static_assert(identical(cnl::countl_rsb(cnl::int8{0x00}), 7), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8{0x01}), 6), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8{0x7E}), 0), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8{0x7F}), 0), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8(-0x80)), 0), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8(-0x7F)), 0), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8(-0x02)), 6), "cnl::countl_rsb<int8_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int8(-0x01)), 7), "cnl::countl_rsb<int8_t>");

        static_assert(identical(cnl::countl_rsb(cnl::int16{0x0000}), 15), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16{0x0001}), 14), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16{0x7FFE}), 0), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16{0x7FFF}), 0), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16(-0x8000)), 0), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16(-0x7FFF)), 0), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16(-0x0002)), 14), "cnl::countl_rsb<int16_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int16(-0x0001)), 15), "cnl::countl_rsb<int16_t>");

        static_assert(identical(cnl::countl_rsb(cnl::int32{0x00000000}), 31), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32{0x00000001}), 30), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32{0x7FFFFFFE}), 0), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32{0x7FFFFFFF}), 0), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32(0x80000000)), 0), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32(0x80000001)), 0), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32(0xFFFFFFFE)), 30), "cnl::countl_rsb<int32_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int32(0xFFFFFFFF)), 31), "cnl::countl_rsb<int32_t>");

        static_assert(identical(cnl::countl_rsb(cnl::int64{0x0000000000000000}), 63), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64{0x0000000000000001}), 62), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64{0x7FFFFFFFFFFFFFFE}), 0), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64{0x7FFFFFFFFFFFFFFF}), 0), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64(0x8000000000000000)), 0), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64(0x8000000000000001)), 0), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64(0xFFFFFFFFFFFFFFFE)), 62), "cnl::countl_rsb<int64_t>");
        static_assert(identical(cnl::countl_rsb(cnl::int64(0xFFFFFFFFFFFFFFFF)), 63), "cnl::countl_rsb<int64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countl_rsb(CNL_INTMAX_C(0x00000000000000000000000000000000)), 127), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(CNL_INTMAX_C(0x00000000000000000000000000000001)), 126), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(~CNL_INTMAX_C(0x00000000000000000000000000000001)), 126), "cnl::countl_rsb<int128_t>");
        static_assert(identical(cnl::countl_rsb(~CNL_INTMAX_C(0x00000000000000000000000000000000)), 127), "cnl::countl_rsb<int128_t>");
#endif
    }

    namespace test_countl_rb {
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x00}), 8), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x01}), 7), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x7E}), 1), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x7F}), 1), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x80}), 0), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0x81}), 0), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0xFE}), 0), "cnl::countl_rb<uint8_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint8{0xFF}), 0), "cnl::countl_rb<uint8_t>");

        static_assert(identical(cnl::countl_rb(cnl::uint16{0x0000}), 16), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0x0001}), 15), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0x7FFE}), 1), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0x7FFF}), 1), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0x8000}), 0), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0x8001}), 0), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0xFFFE}), 0), "cnl::countl_rb<uint16_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint16{0xFFFF}), 0), "cnl::countl_rb<uint16_t>");

        static_assert(identical(cnl::countl_rb(cnl::uint32{0x00000000}), 32), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0x00000001}), 31), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0x7FFFFFFE}), 1), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0x7FFFFFFF}), 1), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0x80000000}), 0), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0x80000001}), 0), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0xFFFFFFFE}), 0), "cnl::countl_rb<uint32_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint32{0xFFFFFFFF}), 0), "cnl::countl_rb<uint32_t>");

        static_assert(identical(cnl::countl_rb(cnl::uint64{0x0000000000000000}), 64), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0x0000000000000001}), 63), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 1), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 1), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0x8000000000000000}), 0), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0x8000000000000001}), 0), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 0), "cnl::countl_rb<uint64_t>");
        static_assert(identical(cnl::countl_rb(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 0), "cnl::countl_rb<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countl_rb(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 127), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 1), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 1), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 0), "cnl::countl_rb<uint128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countl_rb<uint128_t>");
#endif

        static_assert(identical(cnl::countl_rb(cnl::int8{0x00}), 7), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8{0x01}), 6), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8{0x7E}), 0), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8{0x7F}), 0), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8(-0x80)), 0), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8(-0x7F)), 0), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8(-0x02)), 6), "cnl::countl_rb<int8_t>");
        static_assert(identical(cnl::countl_rb(cnl::int8(-0x01)), 7), "cnl::countl_rb<int8_t>");

        static_assert(identical(cnl::countl_rb(cnl::int16{0x0000}), 15), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16{0x0001}), 14), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16{0x7FFE}), 0), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16{0x7FFF}), 0), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16(-0x8000)), 0), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16(-0x7FFF)), 0), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16(-0x0002)), 14), "cnl::countl_rb<int16_t>");
        static_assert(identical(cnl::countl_rb(cnl::int16(-0x0001)), 15), "cnl::countl_rb<int16_t>");

        static_assert(identical(cnl::countl_rb(cnl::int32{0x00000000}), 31), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32{0x00000001}), 30), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32{0x7FFFFFFE}), 0), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32{0x7FFFFFFF}), 0), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32(0x80000000)), 0), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32(0x80000001)), 0), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32(0xFFFFFFFE)), 30), "cnl::countl_rb<int32_t>");
        static_assert(identical(cnl::countl_rb(cnl::int32(0xFFFFFFFF)), 31), "cnl::countl_rb<int32_t>");

        static_assert(identical(cnl::countl_rb(cnl::int64{0x0000000000000000}), 63), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64{0x0000000000000001}), 62), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64{0x7FFFFFFFFFFFFFFE}), 0), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64{0x7FFFFFFFFFFFFFFF}), 0), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64(0x8000000000000000)), 0), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64(0x8000000000000001)), 0), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64(0xFFFFFFFFFFFFFFFE)), 62), "cnl::countl_rb<int64_t>");
        static_assert(identical(cnl::countl_rb(cnl::int64(0xFFFFFFFFFFFFFFFF)), 63), "cnl::countl_rb<int64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countl_rb(CNL_INTMAX_C(0x00000000000000000000000000000000)), 127), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(CNL_INTMAX_C(0x00000000000000000000000000000001)), 126), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 0), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 0), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_INTMAX_C(0x00000000000000000000000000000001)), 126), "cnl::countl_rb<int128_t>");
        static_assert(identical(cnl::countl_rb(~CNL_INTMAX_C(0x00000000000000000000000000000000)), 127), "cnl::countl_rb<int128_t>");
#endif
    }

    namespace test_countr_used {
        static_assert(identical(cnl::countr_used(cnl::uint8{0x00}), 0), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0x01}), 1), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0x7E}), 7), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0x7F}), 7), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0x80}), 8), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0x81}), 8), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0xFE}), 8), "cnl::countr_used<uint8_t>");
        static_assert(identical(cnl::countr_used(cnl::uint8{0xFF}), 8), "cnl::countr_used<uint8_t>");

        static_assert(identical(cnl::countr_used(cnl::uint16{0x0000}), 0), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0x0001}), 1), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0x7FFE}), 15), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0x7FFF}), 15), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0x8000}), 16), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0x8001}), 16), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0xFFFE}), 16), "cnl::countr_used<uint16_t>");
        static_assert(identical(cnl::countr_used(cnl::uint16{0xFFFF}), 16), "cnl::countr_used<uint16_t>");

        static_assert(identical(cnl::countr_used(cnl::uint32{0x00000000}), 0), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0x00000001}), 1), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0x7FFFFFFE}), 31), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0x7FFFFFFF}), 31), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0x80000000}), 32), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0x80000001}), 32), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0xFFFFFFFE}), 32), "cnl::countr_used<uint32_t>");
        static_assert(identical(cnl::countr_used(cnl::uint32{0xFFFFFFFF}), 32), "cnl::countr_used<uint32_t>");

        static_assert(identical(cnl::countr_used(cnl::uint64{0x0000000000000000}), 0), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0x0000000000000001}), 1), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0x7FFFFFFFFFFFFFFE}), 63), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0x7FFFFFFFFFFFFFFF}), 63), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0x8000000000000000}), 64), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0x8000000000000001}), 64), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0xFFFFFFFFFFFFFFFE}), 64), "cnl::countr_used<uint64_t>");
        static_assert(identical(cnl::countr_used(cnl::uint64{0xFFFFFFFFFFFFFFFF}), 64), "cnl::countr_used<uint64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countr_used(CNL_UINTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(CNL_UINTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 127), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 128), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(~CNL_UINTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 128), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(~CNL_UINTMAX_C(0x00000000000000000000000000000001)), 128), "cnl::countr_used<uint128_t>");
        static_assert(identical(cnl::countr_used(~CNL_UINTMAX_C(0x00000000000000000000000000000000)), 128), "cnl::countr_used<uint128_t>");
#endif

        static_assert(identical(cnl::countr_used(cnl::int8{0x00}), 0), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8{0x01}), 1), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8{0x7E}), 7), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8{0x7F}), 7), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8(-0x80)), 7), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8(-0x7F)), 7), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8(-0x02)), 1), "cnl::countr_used<int8_t>");
        static_assert(identical(cnl::countr_used(cnl::int8(-0x01)), 0), "cnl::countr_used<int8_t>");

        static_assert(identical(cnl::countr_used(cnl::int16{0x0000}), 0), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16{0x0001}), 1), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16{0x7FFE}), 15), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16{0x7FFF}), 15), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16(-0x8000)), 15), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16(-0x7FFF)), 15), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16(-0x0002)), 1), "cnl::countr_used<int16_t>");
        static_assert(identical(cnl::countr_used(cnl::int16(-0x0001)), 0), "cnl::countr_used<int16_t>");

        static_assert(identical(cnl::countr_used(cnl::int32{0x00000000}), 0), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32{0x00000001}), 1), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32{0x7FFFFFFE}), 31), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32{0x7FFFFFFF}), 31), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32(0x80000000)), 31), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32(0x80000001)), 31), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32(0xFFFFFFFE)), 1), "cnl::countr_used<int32_t>");
        static_assert(identical(cnl::countr_used(cnl::int32(0xFFFFFFFF)), 0), "cnl::countr_used<int32_t>");

        static_assert(identical(cnl::countr_used(cnl::int64{0x0000000000000000}), 0), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64{0x0000000000000001}), 1), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64{0x7FFFFFFFFFFFFFFE}), 63), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64{0x7FFFFFFFFFFFFFFF}), 63), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64(0x8000000000000000)), 63), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64(0x8000000000000001)), 63), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64(0xFFFFFFFFFFFFFFFE)), 1), "cnl::countr_used<int64_t>");
        static_assert(identical(cnl::countr_used(cnl::int64(0xFFFFFFFFFFFFFFFF)), 0), "cnl::countr_used<int64_t>");

#if defined(CNL_INT128_ENABLED)
        static_assert(identical(cnl::countr_used(CNL_INTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(CNL_INTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 127), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)), 127), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(~CNL_INTMAX_C(0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE)), 127), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(~CNL_INTMAX_C(0x00000000000000000000000000000001)), 1), "cnl::countr_used<int128_t>");
        static_assert(identical(cnl::countr_used(~CNL_INTMAX_C(0x00000000000000000000000000000000)), 0), "cnl::countr_used<int128_t>");
#endif
    }
}
