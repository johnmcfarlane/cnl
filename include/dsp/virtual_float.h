//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_VIRTUAL_FLOAT)
#define CNL_DSP_VIRTUAL_FLOAT

#include "dsp_types.h"

namespace cnl
{
namespace dsp
{
/// Virtual_float implements floating point arithmetic with fixed-point.
/// Very useful when DSP does not have floating point numbers and
/// Dynamic range of the native fixed-point datatype is not enough
///
/// NOTE: With built-in default datatypes float and double, exponent is not used
/// in any of the arithmetic, thus calculation falls down to arithmetic on the
/// datatypes themselves.
template<typename T>
class virtual_float
{
public:
    ///  Constructor
    explicit virtual_float(T value = T{});
    template<typename FromT>
    virtual_float(FromT const& value);
    /// From another base type of virtual_float
    template<typename S>
    virtual_float(virtual_float<S> const& value);
    /// Copy constructor
    virtual_float(virtual_float const&) = default;
    /// Move constructor
    virtual_float(virtual_float && rhs) noexcept :
        m_mantissa(std::move(rhs.m_mantissa)),
        m_exponent(std::move(rhs.m_exponent))
    {
    };
    /// Assignment
    virtual_float& operator=(virtual_float const&) = default;
    /// Move assignment
    virtual_float& operator=(virtual_float && rhs) noexcept
    {
        m_mantissa = std::move(rhs.m_mantissa);
        m_exponent = std::move(rhs.m_exponent);
        return *this;
    };
    /// Addition
    virtual_float& operator+=(virtual_float const& rhs);
    /// Subtraction
    virtual_float& operator-=(virtual_float const& rhs);
    /// Multiplication
    virtual_float& operator*=(virtual_float const& rhs);
    /// Division
    virtual_float& operator/=(virtual_float const& rhs);
    /// Unary negation
    virtual_float& operator-();
    /// Conversion to float
    explicit operator float() const;
    /// Conversion to double
    explicit operator double() const;
    /// Conversion to q4_20
    explicit operator q4_20() const;
    /// Conversion to q8_40
    explicit operator q8_40() const;
    /// Destructor
    ~virtual_float() = default;
    /// Get the mantissa
    T mantissa() const { return m_mantissa; };
    /// Get the exponent
    int exponent() const { return m_exponent; };
    /// Square Root
    template<class S>
    friend virtual_float<S> sqrt(virtual_float<S> const& rhs);
private:
    /// Normalization
    void normalize();
    /// Normalization from different datatype of mantissa
    template<typename FromT>
    void normalize(FromT mantissa);
    /// mantissa
    T m_mantissa;
    /// exponent
    int m_exponent;
};

/// Addition
template<class T>
virtual_float<T> operator+(virtual_float<T> const& lhs,
                           virtual_float<T> const& rhs);
/// Substraction
template<class T>
virtual_float<T> operator-(virtual_float<T> const& lhs,
                           virtual_float<T> const& rhs);
/// Multiplication
template<class T>
virtual_float<T> operator*(virtual_float<T> const& lhs,
                           virtual_float<T> const& rhs);
/// Division
template<class T>
virtual_float<T> operator/(virtual_float<T> const& lhs,
                           virtual_float<T> const& rhs);
/// Greater than
template<class T>
bool operator>(virtual_float<T> const& lhs,
               virtual_float<T> const& rhs);
/// Less than
template<class T>
bool operator<(virtual_float<T> const& lhs,
               virtual_float<T> const& rhs);
/// Equal
template<class T>
bool operator==(virtual_float<T> const& lhs,
                virtual_float<T> const& rhs);
/// Not Equal
template<class T>
bool operator!=(virtual_float<T> const& lhs,
                virtual_float<T> const& rhs);
/// Greater or equal
template<class T>
bool operator>=(virtual_float<T> const& lhs,
                virtual_float<T> const& rhs);
/// Less or equal
template<class T>
bool operator<=(virtual_float<T> const& lhs,
                virtual_float<T> const& rhs);

///////////////////////////////
/////// IMPLEMENTATIONS ///////
///////////////////////////////

template<typename T>
virtual_float<T>::virtual_float(T value) :
    m_mantissa(value),
    m_exponent(0)
{
    normalize();
}

template<typename T>
template<typename FromT>
virtual_float<T>::virtual_float(FromT const& value)
{
    // TODO: Improve the accuracy, by getting the exponent
    // TODO: log2 should be in cnl::dsp::math for all types
    // Do this by log2 and multiplication of value to the range
    // and using the log2 value as exponent.
    m_mantissa = static_cast<T>(value);
    m_exponent = 0;
    normalize();
}

template<typename T>
template<typename S>
virtual_float<T>::virtual_float(virtual_float<S> const& value)
{
    m_mantissa = static_cast<T>(value.mantissa());
    m_exponent = value.exponent();
}

template<typename T>
virtual_float<T>::operator float() const
{
    return static_cast<float>(m_mantissa) *
           std::pow(2.0f, static_cast<float>(m_exponent));
}

template<typename T>
virtual_float<T>::operator double() const
{
    return static_cast<double>(m_mantissa) *
           std::pow(2.0, static_cast<double>(m_exponent));
}

template<typename T>
virtual_float<T>::operator q4_20() const
{
    double val = static_cast<double>(m_mantissa) *
                 std::pow(2.0, static_cast<double>(m_exponent));
    q4_20 ret = val;
    return ret;
}

template<typename T>
virtual_float<T>::operator q8_40() const
{
    double val = static_cast<double>(m_mantissa) *
                 std::pow(2.0, static_cast<double>(m_exponent));
    q8_40 ret = val;
    return ret;
}

template<>
inline virtual_float<q4_20>::operator q4_20() const
{
    q4_20 ret = (m_exponent >= 0) ? m_mantissa << m_exponent
                     : m_mantissa >> (-m_exponent);
    return ret;
}

template<>
inline virtual_float<q8_40>::operator q4_20() const
{
    q4_20 ret = (m_exponent >= 0) ? m_mantissa << m_exponent
                     : m_mantissa >> (-m_exponent);
    return ret;
}

template<>
inline virtual_float<q8_40>::operator q8_40() const
{
    q8_40 ret = (m_exponent >= 0) ? m_mantissa << m_exponent
                     : m_mantissa >> (-m_exponent);
    return ret;
}

template<typename T>
void virtual_float<T>::normalize()
{
}

template<>
inline void virtual_float<q4_20>::normalize()
{
    int norm = cnl::leading_bits(m_mantissa) - 2;
    m_mantissa = norm >= 0 ? m_mantissa << norm : m_mantissa >> (-norm);
    m_exponent -= norm;
}

template<>
inline void virtual_float<q8_40>::normalize()
{
    int norm = cnl::leading_bits(m_mantissa) - 6;
    m_mantissa = norm >= 0 ? m_mantissa << norm : m_mantissa >> (-norm);
    m_exponent -= norm;
}


template<typename T>
template<typename FromT>
void virtual_float<T>::normalize(FromT)
{
}

template<>
template<>
inline void virtual_float<q4_20>::normalize(q8_40 mantissa)
{
    int norm = cnl::leading_bits(mantissa) - 6;
    m_mantissa = (norm >= 0) ? mantissa << norm : mantissa >> (-norm);
    m_exponent -= norm;
}

template<>
template<>
inline void virtual_float<q8_40>::normalize(q8_40 mantissa)
{
    int norm = cnl::leading_bits(mantissa) - 6;
    m_mantissa = (norm >= 0) ? mantissa << norm : mantissa >> (-norm);
    m_exponent -= norm;
}

template<typename T>
virtual_float<T>& virtual_float<T>::operator+=(virtual_float<T> const& rhs)
{
    m_mantissa += rhs.m_mantissa;
    normalize();
    return *this;
}

template<>
inline virtual_float<q4_20>&
virtual_float<q4_20>::operator+=(virtual_float<q4_20> const& rhs)
{
    int shift = m_exponent - rhs.m_exponent;
    q8_40 m1 = (shift >= 0) ? static_cast<q8_40>(m_mantissa)
                    : static_cast<q8_40>(m_mantissa) >> (-shift);
    q8_40 m2 = (shift >= 0) ? static_cast<q8_40>(rhs.m_mantissa) >> shift
                    : static_cast<q8_40>(rhs.m_mantissa);
    q8_40 mantissa = m1 + m2;
    m_exponent = (shift >= 0) ? m_exponent : rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<>
inline virtual_float<q8_40>&
virtual_float<q8_40>::operator+=(virtual_float<q8_40> const& rhs)
{
    int shift = m_exponent - rhs.m_exponent;
    q8_40 m1 = (shift >= 0) ? m_mantissa
                    : m_mantissa >> (-shift);
    q8_40 m2 = (shift >= 0) ? rhs.m_mantissa >> shift
                    : rhs.m_mantissa;
    q8_40 mantissa = m1 + m2;
    m_exponent = (shift >= 0) ? m_exponent : rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<typename T>
virtual_float<T> operator+(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    virtual_float<T> ret(lhs);
    ret += rhs;
    return ret;
}

template<typename T>
virtual_float<T>& virtual_float<T>::operator-=(virtual_float<T> const& rhs)
{
    m_mantissa -= rhs.m_mantissa;
    normalize();
    return *this;
}

template<>
inline virtual_float<q4_20>&
virtual_float<q4_20>::operator-=(virtual_float<q4_20> const& rhs)
{
    int shift = m_exponent - rhs.m_exponent;
    q8_40 m1 = (shift >= 0) ? static_cast<q8_40>(m_mantissa)
                    : static_cast<q8_40>(m_mantissa) >> (-shift);
    q8_40 m2 = (shift >= 0) ? static_cast<q8_40>(rhs.m_mantissa) >> shift
                    : static_cast<q8_40>(rhs.m_mantissa);
    q8_40 mantissa = m1 - m2;
    m_exponent = (shift >= 0) ? m_exponent : rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<>
inline virtual_float<q8_40>&
virtual_float<q8_40>::operator-=(virtual_float<q8_40> const& rhs)
{
    int shift = m_exponent - rhs.m_exponent;
    q8_40 m1 = (shift >= 0) ? m_mantissa
                    : m_mantissa >> (-shift);
    q8_40 m2 = (shift >= 0) ? rhs.m_mantissa >> shift
                    : rhs.m_mantissa;
    q8_40 mantissa = m1 - m2;
    m_exponent = (shift >= 0) ? m_exponent : rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<typename T>
virtual_float<T> operator-(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    virtual_float<T> ret(lhs);
    ret -= rhs;
    return ret;
}

template<typename T>
virtual_float<T>& virtual_float<T>::operator-()
{
    m_mantissa = -m_mantissa;
    return *this;
}

template<typename T>
virtual_float<T>& virtual_float<T>::operator*=(virtual_float<T> const& rhs)
{
    m_mantissa *= rhs.m_mantissa;
    normalize();
    return *this;
}

template<>
inline virtual_float<q4_20>&
virtual_float<q4_20>::operator*=(virtual_float<q4_20> const& rhs)
{
    q8_40 mantissa = m_mantissa * rhs.m_mantissa;
    m_exponent = m_exponent + rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<>
inline virtual_float<q8_40>&
virtual_float<q8_40>::operator*=(virtual_float<q8_40> const& rhs)
{
    q4_20 lhs_mantissa = m_mantissa;
    q4_20 rhs_mantissa = rhs.m_mantissa;
    q8_40 mantissa = lhs_mantissa * rhs_mantissa;
    m_exponent = m_exponent + rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<typename T>
virtual_float<T> operator*(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    virtual_float<T> ret(lhs);
    ret *= rhs;
    return ret;
}

template<typename T>
virtual_float<T>& virtual_float<T>::operator/=(virtual_float<T> const& rhs)
{
    m_mantissa = cnl::dsp::math::divides<T,T>()(m_mantissa,rhs.m_mantissa);
    normalize();
    return *this;
}

template<>
inline virtual_float<q4_20>&
virtual_float<q4_20>::operator/=(virtual_float<q4_20> const& rhs)
{
    q8_40 mantissa = cnl::dsp::math::divides<q4_20,q4_20>()(m_mantissa,rhs.m_mantissa);
    m_exponent = m_exponent - rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<>
inline virtual_float<q8_40>&
virtual_float<q8_40>::operator/=(virtual_float<q8_40> const& rhs)
{
    q4_20 lhs_mantissa = m_mantissa;
    q4_20 rhs_mantissa = rhs.m_mantissa;
    q8_40 mantissa = cnl::dsp::math::divides<q4_20,q4_20>()(lhs_mantissa,rhs_mantissa);
    m_exponent = m_exponent - rhs.m_exponent;
    normalize(mantissa);
    return *this;
}

template<typename T>
virtual_float<T> operator/(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    virtual_float<T> ret(lhs);
    ret /= rhs;
    return ret;
}

template<typename T>
bool operator<(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    assert(lhs.exponent() == 0);
    assert(rhs.exponent() == 0);
    bool res = lhs.mantissa() < rhs.mantissa();
    return res;
}

template<>
inline bool operator<(virtual_float<q4_20> const& lhs,
                      virtual_float<q4_20> const& rhs)
{
    int shift = lhs.exponent() - rhs.exponent();
    bool res = (shift >= 0) ? lhs.mantissa() < (rhs.mantissa() >> shift)
               : (lhs.mantissa() >> (-shift)) < rhs.mantissa();
    return res;
}


template<>
inline bool operator<(virtual_float<q8_40> const& lhs,
                      virtual_float<q8_40> const& rhs)
{
    int shift = lhs.exponent() - rhs.exponent();
    bool res = (shift >= 0) ? lhs.mantissa() < (rhs.mantissa() >> shift)
               : (lhs.mantissa() >> (-shift)) < rhs.mantissa();
    return res;
}

template<typename T>
bool operator==(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    return (lhs.exponent() == rhs.exponent())
           && (lhs.mantissa() == rhs.mantissa());
}

template<typename T>
bool operator!=(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    return !(lhs == rhs);
}

template<typename T>
bool operator>(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    return !((lhs < rhs) || (lhs == rhs));
}

template<typename T>
bool operator>=(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    return !(lhs < rhs);
}

template<typename T>
bool operator<=(virtual_float<T> const& lhs, virtual_float<T> const& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

template<typename T>
virtual_float<T> sqrt(virtual_float<T> const& rhs)
{
    return std::sqrt(rhs.m_mantissa);
}

template<>
inline virtual_float<q4_20> sqrt(virtual_float<q4_20> const& rhs)
{

    q4_20 mantissa;
    if (rhs.m_exponent % 2 == 0)
    {
        mantissa = cnl::sqrt(rhs.m_mantissa);
    }
    else
    {
        mantissa = cnl::sqrt(rhs.m_mantissa<<1);
    }
    virtual_float<q4_20> ret;
    ret.m_exponent = rhs.m_exponent >> 1;
    ret.m_mantissa = mantissa;
    ret.normalize();
    return ret;
}

#if defined(CNL_INT128_ENABLED)
template<>
inline virtual_float<q8_40> sqrt(virtual_float<q8_40> const& rhs)
{

    q8_40 mantissa;
    if (rhs.m_exponent % 2 == 0)
    {
        mantissa = cnl::sqrt(rhs.m_mantissa);
    }
    else
    {
        mantissa = cnl::sqrt(rhs.m_mantissa<<1);
    }
    virtual_float<q8_40> ret;
    ret.m_exponent = rhs.m_exponent >> 1;
    ret.m_mantissa = static_cast<q8_40>(mantissa);
    ret.normalize();
    return ret;
}
#endif

} // namespace dsp
} // namespace cnl
#endif //CNL_DSP_VIRTUAL_FLOAT
