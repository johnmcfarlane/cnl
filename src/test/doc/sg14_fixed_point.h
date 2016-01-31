constexpr auto n = fixed_point<char, -3>{-2.75};
static_assert(n == -2.75, "fixed-point type was unable to store the value");