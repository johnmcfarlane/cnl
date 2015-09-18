#ifndef _SG14_TEST_UTILS_H
#define _SG14_TEST_UTILS_H

#include <cassert>
#include <iostream>

template <typename LHS, typename RHS>
void assert_equal(const LHS & lhs, const RHS & rhs, char const * lhs_name, char const * rhs_name)
{
	if (lhs == rhs)
	{
		return;
	}

	std::cout << "Failed: \"" << lhs_name << " == " << rhs_name << "\", i.e. " << lhs << " != " << rhs << std::endl;
	assert(false);
};

template <typename LHS, typename RHS>
void assert_true(const LHS & expression, char const * expression_name)
{
	if (expression)
	{
		return;
	}

	std::cout << "Failed: (" << expression_name << ") where (" << expression_name << "==" << expression << ')' << std::endl; \
	assert(false);
};

#define ASSERT_EQUAL(A, B) assert_equal(A, B, #A, #B)
#define ASSERT_TRUE(A) assert_true(A, #A)

#endif // _SG14_TEST_UTILS_H
