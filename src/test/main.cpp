
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

////////////////////////////////////////////////////////////////////////////////
// boost::throw_exception

#if defined(CNL_BOOST_ENABLED) && defined(BOOST_NO_EXCEPTIONS)
namespace boost {
    void throw_exception(std::exception const&) {
        std::terminate();
    }
}
#endif
