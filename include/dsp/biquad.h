//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_BIQUAD)
#define CNL_DSP_BIQUAD

#include <array>
#include <vector>

#include "circular_buffer.h"
#include "dsp_types.h"

namespace cnl
{
namespace dsp
{

///Direct form I second order IIR
template<class T>
class biquad
{
public:

    /// Constructor for the second order IIR
    /// @param coeff {b_0,b_1,b_2,a_1,a_2}
    explicit biquad(std::array<T, 5> const& coeff = { { T(1.0), T(0.0), T(0.0), T(0.0), T(0.0) } },
                    unsigned int channelCount = 1);

    ~biquad() = default;

    /// Re-initializes the the second order IIR and clears the state.
    /// @param coeff {b_0,b_1,b_2,a_1,a_2}
    void init(std::array<T, 5> const& coeff, unsigned int channelCount = 1);

    /// Clears the state of the filter
    void clear();

    /// Filters one channel and one time-step at the time
    /// Use std::for_each for traversing an array
    /// @param Input sample
    /// @param channel index in a multichannel setup
    /// @return Filtered sample
    T filter(T in, std::size_t channel = 0);

    /// Filters N channels and one time-step at the time
    /// This is very suitable for N channels interleaved into single vector
    /// Use std::for_each for traversing an array with interleaved channels in N size vector
    /// @param Input sample vector, N parallel channels in the vector
    /// @return Filtered output sample vector. N parallel channels in the vector
    std::vector<T> filter_interleaved(std::vector<T> const& in);

private:
    /// Coefficients of the Second Order IIR
    /// They are in order {b_0,b_1,b_2,a_1,a_2}.
    std::array<T, 5> m_coeff;
    unsigned int m_channelCount;

    /// Delay lines of the IIR.
    std::vector<cnl::dsp::circular_buffer<T> > m_stateX;
    std::vector<cnl::dsp::circular_buffer<T> > m_stateY;
};

template<class T>
biquad<T>::biquad(std::array<T, 5> const& coeff,
                  unsigned int channelCount)
{
    this->init(coeff, channelCount);
}

template<class T>
void biquad<T>::init(std::array<T, 5> const& coeff,
                     unsigned int channelCount)
{
    m_channelCount = channelCount;
    m_stateX.resize(m_channelCount);
    m_stateY.resize(m_channelCount);
    for (unsigned int i = 0; i < m_channelCount; ++i)
    {
        m_stateX[i].resize(2);
        m_stateY[i].resize(2);
    }
    m_coeff = coeff;
    clear();
}

template<class T>
void biquad<T>::clear()
{
    for (unsigned int i = 0; i < m_channelCount; ++i)
    {
        m_stateX[i].clear();
        m_stateY[i].clear();
    }
}

template<class T>
inline T biquad<T>::filter(T in, std::size_t channel)
{
    assert(channel < m_channelCount);
    T acc = m_stateX[channel][0] * m_coeff[1];
    acc += m_stateX[channel][1] * m_coeff[2];
    acc += in * m_coeff[0];
    acc -= m_stateY[channel][0] * m_coeff[3];
    acc -= m_stateY[channel][1] * m_coeff[4];
    m_stateX[channel].push_back(in);
    m_stateY[channel].push_back(acc);
    return acc;
}

template<>
inline q4_20 biquad<q4_20>::filter(q4_20 in, std::size_t channel)
{
    assert(channel < m_channelCount);
    q8_40 acc = m_stateX[channel][0] * m_coeff[1];
    acc += m_stateX[channel][1] * m_coeff[2];
    acc += in * m_coeff[0];
    acc -= m_stateY[channel][0] * m_coeff[3];
    acc -= m_stateY[channel][1] * m_coeff[4];
    q4_20 acc_fix = acc; //Rounding implicitly here
    m_stateX[channel].push_back(in);
    m_stateY[channel].push_back(acc_fix);
    return acc_fix;
}


template<class T>
inline std::vector<T> biquad<T>::filter_interleaved(std::vector<T> const& in)
{
    std::vector<T> out(in.size());
    assert(m_channelCount == in.size());
    //  Should be possible to vectorize across channels
    for (unsigned int ch = 0; ch < m_channelCount; ++ch)
    {
        T acc = m_stateX[ch][0] * m_coeff[1];
        acc += m_stateX[ch][1] * m_coeff[2];
        acc += in[ch] * m_coeff[0];
        acc -= m_stateY[ch][0] * m_coeff[3];
        acc -= m_stateY[ch][1] * m_coeff[4];
        m_stateX[ch].push_back(in[ch]);
        m_stateY[ch].push_back(acc);
        out[ch] = acc;
    }
    return out;
}

template<>
inline std::vector<q4_20> biquad<q4_20>::filter_interleaved(std::vector<q4_20> const& in)
{
    std::vector<q4_20> out(in.size());
    assert(m_channelCount == in.size());
    for (unsigned int ch = 0; ch < m_channelCount; ++ch)
    {
        q8_40 acc = m_stateX[ch][0] * m_coeff[1];
        acc += m_stateX[ch][1] * m_coeff[2];
        acc += in[ch] * m_coeff[0];
        acc -= m_stateY[ch][0] * m_coeff[3];
        acc -= m_stateY[ch][1] * m_coeff[4];
        q4_20 acc_fix = acc; //Rounding implicitly here
        m_stateX[ch].push_back(in[ch]);
        m_stateY[ch].push_back(acc_fix);
        out[ch] = acc_fix;
    }
    return out;
}

} //namespace dsp
} //namespace cnl
#endif //CNL_DSP_BIQUAD
