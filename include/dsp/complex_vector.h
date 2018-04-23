//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_COMPLEX_VECTOR)
#define CNL_DSP_COMPLEX_VECTOR

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include <dsp/complex.h>
#include <dsp/virtual_float.h>
#include <dsp/dsp_math.h>
#include <dsp/trig.h>

namespace cnl
{
namespace dsp
{

/// Defines maximum size for the complex Vector
static const size_t c_complexVectorMaxsize = 
    std::numeric_limits<std::size_t>::max();

/// complex vector with separate vectors for real and imag parts for speed.
/// @tparam T Type parameter
template<typename T>
class complex_vector
{
public:
    /// Constructor
    explicit complex_vector(std::size_t size = 0,
                            complex<T> init = complex<T>{}) :
        m_real(), m_imag()
    {
        m_real.resize(size, init.real());
        m_imag.resize(size, init.imag());
    };
    /// Construction from std::vectors
    complex_vector(std::vector<T> const& real,
                   std::vector<T> const& imag) :
        m_real(real), m_imag(imag)
    {
        assert(m_real.size() == m_imag.size());
    };
    /// Construction from c arrays
    complex_vector(T const* const real, T const* const imag, std::size_t size)
    {
        m_real.resize(size);
        m_imag.resize(size);
        m_real.assign(real, real + size);
        m_imag.assign(imag, imag + size);
    };
    /// Copy constructor
    complex_vector(complex_vector const&) = default;
    /// Move constructor
    explicit complex_vector(complex_vector && rhs) noexcept :
        m_real(std::move(rhs.m_real)),
        m_imag(std::move(rhs.m_imag))
    {
    };
    /// Destructor
    ~complex_vector() = default;
    /// Assignment
    complex_vector& operator=(complex_vector const&) = default;
    /// Move assignment
    complex_vector& operator=(complex_vector && rhs) noexcept
    {
        m_real = std::move(rhs.m_real);
        m_imag = std::move(rhs.m_imag);
        return *this;
    };
    /// Addition
    complex_vector& operator+=(complex_vector const&);
    /// Subtraction
    complex_vector& operator-=(complex_vector const&);
    /// Return the VALUE from index. !!! DOES NOT RETURN REFERENCE
    complex<T> operator[](std::size_t index) const;
    /// Return the VALUE from index. !!! DOES NOT RETURN REFERENCE
    complex<T> at(std::size_t index);
    /// Sets a value to given index
    void set_at(std::size_t index, complex<T> const& value);
    /// Returns the size of vector
    std::size_t size() const;
    /// resizes the vector
    void resize(std::size_t size, complex<T> init = complex<T>(0, 0));
    /// reserves space for the vector
    void reserve(std::size_t size);
    /// Pushes value back to the vector
    void push_back(complex<T> const& value);
    /// conjugates the vector
    void conj();
    /// Returns reference to the real vector
    std::vector<T> const& real() const { return m_real; };
    /// Returns reference to the imaginary vector
    std::vector<T> const& imag() const { return m_imag; };
    /// Returns reference to the real vector
    std::vector<T>& real_data() { return m_real; };
    /// Returns reference to the imaginary vector
    std::vector<T>& imag_data() { return m_imag; };
    /// fills the array with given value
    void fill(complex<T> const& value);
private:
    /// real vector
    std::vector<T> m_real;
    /// imag vector
    std::vector<T> m_imag;
};

/// Addition
template<typename T>
complex_vector<T> operator+(complex_vector<T> const& lhs,
                            complex_vector<T> const& rhs);
/// Subtraction
template<typename T>
complex_vector<T> operator-(complex_vector<T> const& lhs,
                            complex_vector<T> const& rhs);
/// Returns conjugate of the vector
template<typename T>
complex_vector<T> conj(complex_vector<T> const& vector);
/// absolute value of the vector elements
template<typename T>
std::vector<T> abs(complex_vector<T> const& vector);
/// hadamard (element by element) product of the two input vectors as specified mathematically
/// @param conjugate being true means that the elements of the second vector conjugated
template<typename T>
complex_vector<T> hadamard(complex_vector<T> const& lhs,
                           complex_vector<T> const& rhs,
                           bool conjugate = false,
                           size_t begin = 0, size_t end = c_complexVectorMaxsize);

/// dot, or Schur, product of the two input vector.s
/// With default parameters the elements of second vector are conjugated as
/// specified mathematically for dot product
/// @param conjugate being true means that the elements of the second vector are NOT conjugated.
template<typename T, typename R>
complex<R> dot(complex_vector<T> const& lhs,
               complex_vector<T> const& rhs,
               bool conjugate = false, size_t begin = 0,
               size_t end = c_complexVectorMaxsize);

/// Correlation of the input vector with complex rotator exp(j*(k*step/fftsize)*2*pi)
/// The elements of complex rotator are conjugated as specified mathematically (correlation)
/// @param conjugate being true means that the elements of the rotator are NOT conjugated.
template<typename T, typename R>
R correlate_with_exp(complex_vector<T> const& vector,
                     unsigned int step,
                     unsigned int fftsize,
                     bool conjugate = false,
                     size_t begin = 0, size_t end = c_complexVectorMaxsize);

/// energy of the vector in the given range
template<typename T, typename R>
R energy(complex_vector<T> const& vector,
         size_t begin = 0, size_t end = c_complexVectorMaxsize);

///////////////////////////////
/////// IMPLEMENTATIONS ///////
///////////////////////////////

template<typename T>
complex<T> complex_vector<T>::operator[](std::size_t index) const
{
    return complex<T>(m_real[index], m_imag[index]);
}

template<typename T>
complex<T> complex_vector<T>::at(std::size_t index)
{
    return complex<T>(m_real.at(index), m_imag.at(index));
}

template<typename T>
void complex_vector<T>::set_at(std::size_t index, complex<T> const& value)
{
    m_real.at(index) = value.real();
    m_imag.at(index) = value.imag();
}

template<typename T>
void complex_vector<T>::push_back(complex<T> const& value)
{
    m_real.push_back(value.real());
    m_imag.push_back(value.imag());
}

template<typename T>
std::size_t complex_vector<T>::size() const
{
    assert(m_real.size() == m_imag.size());
    return m_real.size();
}

template<typename T>
void complex_vector<T>::resize(std::size_t size, complex<T> init)
{
    m_real.resize(size, init.real());
    m_imag.resize(size, init.imag());
}

template<typename T>
void complex_vector<T>::reserve(std::size_t size)
{
    m_real.reserve(size);
    m_imag.reserve(size);
}

template<typename T>
void complex_vector<T>::fill(complex<T> const& value)
{
    std::fill(m_real.begin(), m_real.end(), value.real());
    std::fill(m_imag.begin(), m_imag.end(), value.imag());
}

template<typename T>
complex_vector<T>& complex_vector<T>::operator+=(complex_vector<T> const& rhs)
{
    assert(size() == rhs.size());
    std::transform(m_real.begin(), m_real.end(), rhs.m_real.begin(),
                   m_real.begin(), cnl::dsp::math::plus<T>());
    std::transform(m_imag.begin(), m_imag.end(), rhs.m_imag.begin(),
                   m_imag.begin(), cnl::dsp::math::plus<T>());
    return *this;
}

template<typename T>
complex_vector<T>& complex_vector<T>::operator-=(complex_vector<T> const& rhs)
{
    assert(size() == rhs.size());
    std::transform(m_real.begin(), m_real.end(), rhs.m_real.begin(),
                   m_real.begin(), cnl::dsp::math::minus<T>());
    std::transform(m_imag.begin(), m_imag.end(), rhs.m_imag.begin(),
                   m_imag.begin(), cnl::dsp::math::minus<T>());
    return *this;
}

template<typename T>
complex_vector<T> operator+(complex_vector<T> const& lhs,
                           complex_vector<T> const& rhs)
{
    complex_vector<T> res(lhs);
    res += rhs;
    return res;
}

template<typename T>
complex_vector<T> operator-(complex_vector<T> const& lhs,
                           complex_vector<T> const& rhs)
{
    complex_vector<T> res(lhs);
    res -= rhs;
    return res;
}

template<typename T>
void complex_vector<T>::conj()
{
    std::for_each(m_imag.begin(), m_imag.end(),
                  [] (T & val)
                  {
                      val = -val;
                  });
}

template<typename T>
complex_vector<T> conj(complex_vector<T> const& vector)
{
    complex_vector<T> ret(vector);
    ret.conj();
    return ret;
}

template<typename T>
std::vector<T> abs(complex_vector<T> const& vector)
{
    std::vector<T> ret(vector.size());
    for (std::size_t index = 0; index < vector.size(); ++index)
    {
        ret[index] = abs(vector[index]);
    }
    return ret;
}

template<typename T>
complex_vector<T> hadamard(complex_vector<T> const& lhs,
                          complex_vector<T> const& rhs,
                          bool conjugate,
                          size_t begin, size_t end)
{
    size_t last = std::min(end, rhs.size());
    last = std::min(last, lhs.size());
    complex_vector<T> ret(last - begin);
    std::size_t at = 0;
    for (std::size_t index = begin; index < last; ++index, ++at)
    {
        // hadamard product is BY DEFINITION, a.*b;
        // when conjugation is on it means a.*conj(b)
        complex<T> prod = (conjugate) ? lhs[index] * conj(rhs[index])
                          : lhs[index] * rhs[index];
        ret.set_at(at, prod);
    }
    return ret;
}

template<typename T, typename R>
complex<R> dot(complex_vector<T> const& lhs, complex_vector<T> const& rhs,
               bool conjugate,
               size_t begin, size_t end)
{
    size_t last = std::min(end, rhs.size());
    last = std::min(last, lhs.size());
    complex<R> sum(0.,0.);
    for (std::size_t index = begin; index < last; ++index)
    {
        // When (!conjugate) is true, the ? is lhs*conj(rhs)
        // when conjugation is on it means a*conj(conj(b)) = a * b;
        sum += (!conjugate) ? lhs[index] * conj(rhs[index])
                            : lhs[index] * rhs[index];
    }
    return sum;
}

template<typename T, typename R>
R correlate_with_exp(complex_vector<T> const& vector,
                     unsigned int step,
                     unsigned int fftsize,
                     bool conjugate,
                     size_t begin,
                     size_t end)
{
    size_t last = std::min(end, vector.size());
    cnl::dsp::trig<T>& exp = cnl::dsp::trig<T>::instance();
    std::size_t stride = static_cast<std::size_t>(step * exp.getTwoPiIndex() / fftsize);
    R sum(0.);
    for (std::size_t index = begin; index < last; ++index)
    {
        // Correlation is BY DEFINITION, a*conj(b);
        // When (!conjugate) is true, the ? is lhs*conj(exp)
        // when conjugation is on it means a*conj(conj(b)) = a * b;
        sum += (!conjugate) ? vector[index] * conj(exp[index * stride])
                            : vector[index] * exp[index * stride];
    }
    return sum;
}

template<typename T, typename R>
R energy(complex_vector<T> const& vector,
         size_t begin, size_t end)
{
    size_t last = std::min(end, vector.size());
    R sum(0.);
    for (std::size_t index = begin; index < last; ++index)
    {
        sum += static_cast<R>(vector[index].real() * vector[index].real() +
                              vector[index].imag() * vector[index].imag());
    }
    return sum;
}


} // namespace dsp
} // namespace cnl
#endif //CNL_DSP_COMPLEX_VECTOR
