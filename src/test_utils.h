#ifndef _SG14_TEST_UTILS_H
#define _SG14_TEST_UTILS_H

#include <cassert>
#include <iomanip>
#include <iostream>

#define FAIL_STREAM std::cerr

inline void test_break()
{
	exit(1);
}

template <typename LHS, typename RHS>
void assert_equal(const LHS & lhs, const RHS & rhs, char const * lhs_name, char const * rhs_name, char const * file, int line)
{
	if (lhs == rhs)
	{
		// test passed
		return;
	}
	
	FAIL_STREAM << std::setprecision(100)
		<< file << ':' << line
		<< ": Failed: \"" << lhs_name << " == " << rhs_name 
		<< "\", i.e. " << lhs << " != " << rhs << std::endl;
	
	test_break();
};

template <typename Expression>
void assert_true(const Expression & expression, char const * expression_name, char const * file, int line)
{
	if (expression)
	{
		// test passed
		return;
	}
	
	FAIL_STREAM
		<< file << ':' << line
		<< ": Failed: (" << expression_name << ") where (" << expression_name << "==" << expression << ')' << std::endl;
	
	test_break();
}

#if defined(_SG14_EXCEPTIONS_ENABLED)
template <typename Exception, typename Expression>
void assert_throws(const Expression & expression, char const * expression_name, char const * file, int line)
{
	try {
		expression();
		FAIL_STREAM
			<< file << ':' << line
			<< ": Failed to throw: \"" << expression_name << '"' << std::endl;
		
		test_break();
	}
	catch (Exception) {
		// test passed
	}
	catch (...) {
		FAIL_STREAM << "Threw wrong exception: (" << expression_name << ')' << std::endl;
	}
}
#endif

#define ASSERT_EQUAL(A, B) do { assert_equal(A, B, #A, #B, __FILE__, __LINE__); } while (false)
#define ASSERT_TRUE(EXPR) do { assert_true(EXPR, #EXPR, __FILE__, __LINE__)); } while (false)
#define ASSERT_THROWS(EXPR, EXCEPTION) do { assert_throws<EXCEPTION>([&](){EXPR;}, #EXPR, __FILE__, __LINE__); } while (false)

#endif // _SG14_TEST_UTILS_H
