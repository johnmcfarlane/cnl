#if defined(RUN_TESTS)
#if defined(RUN_BENCHMARKS)
#error cannot build tests and benchmarks together; one or other
#endif	// RUN_BENCHMARKS

#include <cstdio>

void fixed_point_test();
void fixed_point_utils_test();
void proposal_test();

int main(int, char *[])
{
	fixed_point_test();
	fixed_point_utils_test();
	proposal_test();

	return 0;
}

#else	// RUN_TESTS
#if ! defined(RUN_BENCHMARKS)
#error Please either pass RUN_TESTS or RUN_BENCHMARKS to cmake.
#endif
#endif	// RUN_TESTS
