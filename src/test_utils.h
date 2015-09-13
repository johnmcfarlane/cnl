#ifndef _SG14_TEST_UTILS_H
#define _SG14_TEST_UTILS_H

#include <cassert>
#include <iostream>

#define ASSERT_EQUAL(A, B) \
	if ((A) != (B)) { \
		cout << "Failed: \"" << #A " == " #B << "\", i.e. " << (A) << " != " << (B) << endl; \
		assert(false); \
	}

#define ASSERT_TRUE(A) \
	if (!(A)) { \
		cout << "Failed: (" << #A << ") where (" #A "==" << (A) << ')' << endl; \
		assert(false); \
	}

#endif // _SG14_TEST_UTILS_H
