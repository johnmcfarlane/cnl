//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_TRIG)
#define CNL_DSP_TRIG

#include "complex.h"
#include "dsp_types.h"
#include "dsp_math.h"

namespace cnl
{
namespace dsp
{

/// Table lookup based trigonometric functions implemented as thread safe
/// singleton. "Meyers Singleton" to find more information.
template<class T>
class trig
{
public:
    /// Returns thread safe singleton.
    /// @return the Singleton
    static trig& instance();

    trig(trig const& instance) = delete;
    trig(trig const && instance) = delete;
    trig& operator=(trig const&) = delete;
    trig && operator=(trig const &&) = delete;
    ~trig() = default;

    /// Sine function for any positive turn value
    /// @param turn value denoting the angle. 1.0 turns equals to 2*PI.
    /// @return sin(2*PI*turn)
    T sin_turn(T turn);

    /// Cosine function for any positive turn value
    /// @param turn value denoting the angle. 1.0 turns equals to 2*PI.
    /// @return cos(2*PI*turn)
    T cos_turn(T turn);

    /// Returns both cosine and sine for any positive turn value
    /// @param turn value denoting the angle. 1.0 turns equals to 2*PI.
    /// @return cos(2*PI*turn) + j*sin(2*PI*turn)
    complex<T> exp_turn(T turn);

    /// Returns sin for any non-negative index
    /// @param index non-negative index
    /// @return returns the sine from the index i.e. sin(index*2*pi/twoPiIndex)
    T sin_at(std::size_t index);

    /// Returns cosine for any non-negative index
    /// @param index non-negative index
    /// @return returns the cosine from the index cos(index*2*pi/twoPiIndex)
    T cos_at(std::size_t index);

    /// Returns exp(j*(index/twoPiIndex)*2*pi)
    /// @param index non-negative index
    /// @return cos(index*2*PI/twoPiIndex) + j*sin(index*2*PI/twoPiIndex)
    complex<T> operator[](std::size_t index);

    /// Returns the index of 2*PI. Use this to calculate the stride for indexing.
    /// @return index of 2*PI
    std::size_t get_twopi_index();

    /// Returns reference to internal pre-calculated cos data. Use at your own risk.
    /// @return const reference to CosQuarter vector
    const std::vector<T>& data() const { return m_cosQuarter; }

protected:
private:
    trig();
    std::vector<T> m_cosQuarter;
};

std::size_t constexpr QUARTER_ANGLE = 2048;
std::size_t constexpr INDEX_MASK = (QUARTER_ANGLE - 1);
std::size_t constexpr QMASK = (QUARTER_ANGLE | (QUARTER_ANGLE << 1));
std::size_t constexpr WRAP = (QUARTER_ANGLE << 2);

/// Static variables with block scope will be created exactly once.
/// This characteristic is the base of the so called Meyers Singleton.
template<class T>
trig<T>& trig<T>::instance()
{
    static trig<T> m_instance;
    return m_instance;
}

template<class T>
trig<T>::trig()
{
    m_cosQuarter.resize(QUARTER_ANGLE + 1);
    m_cosQuarter[0] = T{1};
    m_cosQuarter[QUARTER_ANGLE] = T{0};
    for (std::size_t i = 1; i < QUARTER_ANGLE; ++i)
    {
        double angle = cnl::dsp::math::c_pi_2 * double(i) / double(QUARTER_ANGLE);
        m_cosQuarter[i] = static_cast<T>(std::cos(angle));
    }
}

template<class T>
std::size_t trig<T>::get_twopi_index()
{
    return 4 * QUARTER_ANGLE;
}

template<class T>
T trig<T>::sin_turn(T turn)
{
    unsigned int index = static_cast<unsigned int>(cnl::dsp::math::floor(turn * (4 * QUARTER_ANGLE)));
    T fra = turn * (4 * QUARTER_ANGLE) - index;
    T lo = sin_at(static_cast<std::size_t>(index));
    T hi = sin_at(static_cast<std::size_t>(index) + 1);
    T dif = hi - lo;
    T retval = lo + dif * fra;
    return retval;
}

template<class T>
T trig<T>::cos_turn(T turn)
{
    unsigned int index = static_cast<unsigned int>(cnl::dsp::math::floor(turn * (4 * QUARTER_ANGLE)));
    T fra = turn * (4 * QUARTER_ANGLE) - index;
    T lo = cos_at(static_cast<std::size_t>(index));
    T hi = cos_at(static_cast<std::size_t>(index) + 1);
    T dif = hi - lo;
    T retval = lo + dif * fra;
    return retval;
}

template<class T>
complex<T> trig<T>::exp_turn(T turn)
{
    unsigned int index = static_cast<unsigned int>(cnl::dsp::math::floor(turn * (4 * QUARTER_ANGLE)));
    T fra = turn * (4 * QUARTER_ANGLE) - index;
    complex<T> lo = operator[](index);
    complex<T> hi = operator[](index+1);
    T cos_lo, sin_lo, cos_hi, sin_hi;
    complex<T> ret = lo + (hi-lo)*complex<T>(fra);
    return ret;
}

template<class T>
T trig<T>::sin_at(std::size_t index)
{
    T r_sin;
    switch (index & QMASK)
    {
    case 0:
        r_sin = m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    case QUARTER_ANGLE:
        r_sin = m_cosQuarter[index & INDEX_MASK];
        break;
    case (QUARTER_ANGLE << 1):
        r_sin = -m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    default: // QUARTER_ANGLE | (QUARTER_ANGLE<<1)
        r_sin = -m_cosQuarter[index & INDEX_MASK];
        break;
    }
    return r_sin;
}

template<class T>
T trig<T>::cos_at(std::size_t index)
{
    T r_cos;
    switch (index & QMASK)
    {
    case 0:
        r_cos = m_cosQuarter[index & INDEX_MASK];
        break;
    case QUARTER_ANGLE:
        r_cos = -m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    case (QUARTER_ANGLE << 1):
        r_cos = -m_cosQuarter[index & INDEX_MASK];
        break;
    default: // QUARTER_ANGLE | (QUARTER_ANGLE<<1)
        r_cos = m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    }
    return r_cos;
}

template<class T>
complex<T> trig<T>::operator[](std::size_t index)
{
    T r_sin;
    T r_cos;
    switch (index & QMASK)
    {
    case 0:
        r_cos = m_cosQuarter[index & INDEX_MASK];
        r_sin = m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    case QUARTER_ANGLE:
        r_cos = -m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        r_sin = m_cosQuarter[index & INDEX_MASK];
        break;
    case (QUARTER_ANGLE << 1):
        r_cos = -m_cosQuarter[index & INDEX_MASK];
        r_sin = -m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        break;
    default: // QUARTER_ANGLE | (QUARTER_ANGLE<<1)
        r_cos = m_cosQuarter[QUARTER_ANGLE - (index & INDEX_MASK)];
        r_sin = -m_cosQuarter[index & INDEX_MASK];
        break;
    }
    return complex<T>(r_cos, r_sin);
}

} // namespace dsp
} // namespace cnl
#endif // CNL_DSP_TRIG