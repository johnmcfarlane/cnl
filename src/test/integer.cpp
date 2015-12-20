#include <integer.h>

using namespace sg14;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::common_type

static_assert(is_same<
        _integer_impl::common_type<
                saturated_integer<int8_t>,
                saturated_integer<int8_t>>::type,
        saturated_integer<int8_t>>::value, "sg14::_integer_impl::common_type test failure");

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::positive_digits

static_assert(sg14::_integer_impl::positive_digits<int8_t>::value == 7, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<uint8_t>::value == 8, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<int16_t>::value == 15, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<uint16_t>::value == 16, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<int32_t>::value == 31, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<uint32_t>::value == 32, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<int64_t>::value == 63, "sg14::_integer_impl::positive_digits test failed");
static_assert(sg14::_integer_impl::positive_digits<uint64_t>::value == 64, "sg14::_integer_impl::positive_digits test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::negative_digits

static_assert(sg14::_integer_impl::negative_digits<int8_t>::value == 7, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<uint8_t>::value == 0, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<int16_t>::value == 15, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<uint16_t>::value == 0, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<int32_t>::value == 31, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<uint32_t>::value == 0, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<int64_t>::value == 63, "sg14::_integer_impl::negative_digits test failed");
static_assert(sg14::_integer_impl::negative_digits<uint64_t>::value == 0, "sg14::_integer_impl::negative_digits test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::is_positive_overflow

static_assert(! _integer_impl::is_positive_overflow<uint8_t, int8_t>(-1), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, int8_t>(127), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, uint16_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, uint16_t>(255), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(_integer_impl::is_positive_overflow<uint8_t, uint16_t>(256), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, int16_t>(-1), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, int16_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, int16_t>(255), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(_integer_impl::is_positive_overflow<uint8_t, int16_t>(256), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint8_t, int16_t>(-1), "sg14::_integer_impl::is_positive_overflow test failed");

static_assert(! _integer_impl::is_positive_overflow<int8_t, uint8_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, uint8_t>(127), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(_integer_impl::is_positive_overflow<int8_t, uint8_t>(128), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, uint16_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, uint16_t>(127), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(_integer_impl::is_positive_overflow<int8_t, uint16_t>(128), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, int16_t>(-1), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, int16_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<int8_t, int16_t>(127), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(_integer_impl::is_positive_overflow<int8_t, int16_t>(128), "sg14::_integer_impl::is_positive_overflow test failed");

static_assert(! _integer_impl::is_positive_overflow<uint16_t, uint8_t>(0), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint16_t, uint8_t>(255), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint16_t, int8_t>(-128), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint16_t, int8_t>(127), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint16_t, int16_t>(-32768), "sg14::_integer_impl::is_positive_overflow test failed");
static_assert(! _integer_impl::is_positive_overflow<uint16_t, int16_t>(32767), "sg14::_integer_impl::is_positive_overflow test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_integer_impl::is_negative_overflow

static_assert(_integer_impl::is_negative_overflow<uint8_t, int8_t>(-1), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, int8_t>(127), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, uint16_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, uint16_t>(255), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, uint16_t>(256), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(_integer_impl::is_negative_overflow<uint8_t, int16_t>(-1), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, int16_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, int16_t>(255), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint8_t, int16_t>(256), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(_integer_impl::is_negative_overflow<uint8_t, int16_t>(-1), "sg14::_integer_impl::is_negative_overflow test failed");

static_assert(! _integer_impl::is_negative_overflow<int8_t, uint8_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, uint8_t>(127), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, uint8_t>(128), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, uint16_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, uint16_t>(127), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, uint16_t>(128), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(_integer_impl::is_negative_overflow<int8_t, int16_t>(-129), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, int16_t>(-128), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<int8_t, int16_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");

static_assert(! _integer_impl::is_negative_overflow<uint16_t, uint8_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint16_t, uint8_t>(255), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(_integer_impl::is_negative_overflow<uint16_t, int8_t>(-1), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint16_t, int8_t>(0), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint16_t, int8_t>(127), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(_integer_impl::is_negative_overflow<uint16_t, int16_t>(-32768), "sg14::_integer_impl::is_negative_overflow test failed");
static_assert(! _integer_impl::is_negative_overflow<uint16_t, int16_t>(32767), "sg14::_integer_impl::is_negative_overflow test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::saturated_integer

// equality

static_assert(32767 == saturated_integer<int16_t>(5000000000L), "sg14::saturated_integer equality test failed");
static_assert(saturated_integer<uint8_t>(-1) == 0, "sg14::saturated_integer equality test failed");
static_assert(saturated_integer<int64_t>(5000000000L) != saturated_integer<uint32_t>(5000000000L), "sg14::saturated_integer equality test failed");

// assignment

static_assert(saturated_integer<uint8_t>(-1) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(-.1) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1) == 1, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254) == 254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.99) == 254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255) == 255, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(256) == 255, "sg14::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-129) == -128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128) == -128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127) == -127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126) == 126, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127) == 127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(128) == 127, "sg14::saturated_integer test failed");

static_assert(saturated_integer<uint8_t>(-1e38f) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(0.) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1.f) == 1, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(254.) == 254, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(255.f) == 255, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(1e50) == 255, "sg14::saturated_integer test failed");

static_assert(saturated_integer<int8_t>(-1e50) == -128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-128.f) == -128, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-127.) == -127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(126.f) == 126, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(127.) == 127, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(1e38f) == 127, "sg14::saturated_integer test failed");

// arithmetic

static_assert(saturated_integer<int8_t>(100) + saturated_integer<int8_t>(100) == numeric_limits<int8_t>::max(), "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint8_t>(100) - saturated_integer<int16_t>(100000) == 100 - numeric_limits<int16_t>::max(), "sg14::saturated_integer test failed");
static_assert(saturated_integer<int8_t>(-5) * saturated_integer<int32_t>(-14) == 70, "sg14::saturated_integer test failed");
static_assert(saturated_integer<uint32_t>(1) / saturated_integer<int16_t>(-10) == 0, "sg14::saturated_integer test failed");
static_assert(saturated_integer<int16_t>(77) / saturated_integer<int8_t>(-11) == -7, "sg14::saturated_integer test failed");
