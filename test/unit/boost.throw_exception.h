
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TEST_BOOST_THROW_EXCEPTION_H)
#define CNL_TEST_BOOST_THROW_EXCEPTION_H

////////////////////////////////////////////////////////////////////////////////
// boost::throw_exception
//
// http://www.boost.org/doc/libs/release/libs/exception/doc/throw_exception.html

namespace boost {
#if defined(BOOST_NO_EXCEPTIONS)
    inline void throw_exception(std::exception const&)
    { // NOLINT(readability-redundant-declaration)
        std::terminate();
    }
#endif
}

#endif // CNL_TEST_BOOST_THROW_EXCEPTION_H
