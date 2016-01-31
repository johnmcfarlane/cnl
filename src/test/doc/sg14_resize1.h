using new_type = resize_t<unsigned, 2>;
static_assert(std::is_same<new_type, uint16_t>::value, "failed to resize an native-sized unsigned int to 2-bytes");