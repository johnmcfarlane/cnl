using new_type = resize_t<signed char, 5>;
static_assert(std::is_same<new_type, int64_t>::value, "failed to resize a signed byte type to a built-in signed type of at least 5 bytes");