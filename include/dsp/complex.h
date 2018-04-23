//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_COMPLEX)
#define CNL_DSP_COMPLEX

//#include <cmath>

#include "dsp_types.h"
#include "dsp_math.h"

namespace cnl
{
namespace dsp
{

/// Complex numbers
template<typename T>
class complex
{
public:
    /// Constructor taking a real and imaginary parts
    explicit complex(T real = T{}, T imag = T{}) :
        m_real(real),
        m_imag(imag)
    {
    };
    /// Copy constuctor
    complex(complex const&) = default;
    /// Move constructor
    complex(complex&& value) noexcept :
        m_real(std::move(value.m_real)),
        m_imag(std::move(value.m_imag))
    {
    };
    /// Copy constructor from different template parameter
    template<typename FromT>
    complex(complex<FromT> const& value);
    /// Assignment operator
    complex& operator=(complex const&) = default;
    /// Move assignment
    complex& operator=(complex && rhs) noexcept
    {
        m_real = std::move(rhs.m_real);
        m_imag = std::move(rhs.m_imag);
        return *this;
    };
    /// Assignment from different template parameter
    template<typename FromT>
    complex& operator=(complex<FromT> const& rhs);
    /// Addition
    complex& operator+=(complex const& rhs);
    /// Subtraction
    complex& operator-=(complex const& rhs);
    /// Multiplication
    complex& operator*=(complex const& rhs);
    /// Division
    complex& operator/=(complex const& rhs);
    /// Absolute value
    T abs() const;
    /// Conjugation
    void conj();
    /// Destructor
    ~complex() = default;
    /// Get the real part
    T real() const { return m_real; };
    /// Get the imaginary part
    T imag() const { return m_imag; };
private:
    /// Real part
    T m_real;
    /// Imaginary part
    T m_imag;
};

/// Conjugation
template<typename T>
complex<T> conj(complex<T> const& value);
/// Absolute value
template<typename T>
T abs(complex<T> const& value);
/// Addition
template<typename T>
complex<T> operator+(complex<T> const& lhs, complex<T> const& rhs);
/// Subtraction
template<typename T>
complex<T> operator-(complex<T> const& lhs, complex<T> const& rhs);
/// Multiplication
template<typename T>
complex<T> operator*(complex<T> const& lhs, complex<T> const& rhs);
/// Division
template<typename T>
complex<T> operator/(complex<T> const& lhs, complex<T> const& rhs);

///////////////////////////////
/////// IMPLEMENTATIONS ///////
///////////////////////////////

template<typename T>
template<typename FromT>
complex<T>::complex(complex<FromT> const& value) :
    m_real(static_cast<T>(value.real())),
    m_imag(static_cast<T>(value.imag()))
{
}

template<typename T>
template<typename FromT>
complex<T>& complex<T>::operator=(complex<FromT> const& rhs)
{
    m_real = static_cast<T>(rhs.real());
    m_imag = static_cast<T>(rhs.imag());
    return *this;
}

template<typename T>
complex<T>& complex<T>::operator+=(complex<T> const& rhs)
{
    m_real += rhs.m_real;
    m_imag += rhs.m_imag;
    return *this;
}

template<typename T>
complex<T>& complex<T>::operator-=(complex<T> const& rhs)
{
    m_real -= rhs.m_real;
    m_imag -= rhs.m_imag;
    return *this;
}

template<typename T>
complex<T>& complex<T>::operator*=(complex<T> const& rhs)
{
    T real = m_real * rhs.m_real - m_imag * rhs.m_imag;
    T imag = m_imag * rhs.m_real + m_real * rhs.m_imag;
    m_real = real;
    m_imag = imag;
    return *this;
}

template<typename T>
complex<T>& complex<T>::operator/=(complex<T> const& rhs)
{
    T real = m_real * rhs.m_real + m_imag * rhs.m_imag;
    T imag = m_imag * rhs.m_real - m_real * rhs.m_imag;
    T div = rhs.m_real * rhs.m_real + rhs.m_imag * rhs.m_imag;
    m_real = cnl::dsp::math::divides<T,T>()(real, div);
    m_imag = cnl::dsp::math::divides<T,T>()(imag, div);
    return *this;
}

template<typename T>
complex<T> operator+(complex<T> const& lhs, complex<T> const& rhs)
{
    complex<T> res(lhs);
    res += rhs;
    return res;
}

template<typename T>
complex<T> operator-(complex<T> const& lhs, complex<T> const& rhs)
{
    complex<T> res(lhs);
    res -= rhs;
    return res;
}

template<typename T>
complex<T> operator*(complex<T> const& lhs, complex<T> const& rhs)
{
    complex<T> res(lhs);
    res *= rhs;
    return res;
}

template<typename T>
complex<T> operator/(complex<T> const& lhs, complex<T> const& rhs)
{
    complex<T> res(lhs);
    res /= rhs;
    return res;
}

template<typename T>
T complex<T>::abs() const
{
    T val = m_real * m_real + m_imag * m_imag;
    T res = cnl::dsp::math::sqrt(val);
    return res;
}

template<typename T>
T abs(complex<T> const& value)
{
    return value.abs();
}

template<typename T>
void complex<T>::conj()
{
    m_imag = -m_imag;
}

template<typename T>
complex<T> conj(complex<T> const& value)
{
    complex<T> ret(value);
    ret.conj();
    return ret;
}

} // namespace dsp
} // namespace cnl
#endif //CNL_DSP_COMPLEX