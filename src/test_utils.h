#ifndef _SG14_TEST_UTILS_H
#define _SG14_TEST_UTILS_H

#include <cassert>
#include <iostream>

template <typename LHS, typename RHS>
bool assert_equal(const LHS & lhs, const RHS & rhs, char const * lhs_name, char const * rhs_name) noexcept
{
	if (lhs == rhs)
	{
		return true;
	}

	std::cout << "Failed: \"" << lhs_name << " == " << rhs_name << "\", i.e. " << lhs << " != " << rhs << std::endl;
	return false;
};

template <typename LHS, typename RHS>
bool assert_true(const LHS & expression, char const * expression_name) noexcept
{
	if (expression)
	{
		return true;
	}

	std::cout << "Failed: (" << expression_name << ") where (" << expression_name << "==" << expression << ')' << std::endl; \
	return false;
};

#define ASSERT_EQUAL(A, B) if (!assert_equal(A, B, #A, #B)) assert(false);
#define ASSERT_TRUE(A) ig (!assert_true(A, #A)) assert(false);

#endif // _SG14_TEST_UTILS_H
