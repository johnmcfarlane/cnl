//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_BIQUAD_CASCADE)
#define CNL_DSP_BIQUAD_CASCADE

#include <algorithm>
#include <vector>

#include "biquad.h"

namespace cnl
{
namespace dsp
{

/// Cascaded Second Order IIR sections
template<class T>
class biquad_cascade
{
public:
    /// Ctor
    /// @param coeff Matrix of coefficients. Each section in a row.
    explicit biquad_cascade(std::vector<std::array<T, 5> > const& coeff = { { { T(1.0), T(0.0), T(0.0), T(0.0), T(0.0) } } },
                           unsigned int channelCount = 1);

    ~biquad_cascade() = default;

    /// @param coeff Matrix of coefficients. Each section in a row.
    void init(std::vector<std::array<T, 5> > const& coeff,
              unsigned int channelCount = 1);

    /// Clears the state
    void clear();

    /// Filters one channel and one time-step at the time
    /// Use this interface through std::for_each looping through a vector
    /// @param in Input sample
    /// @param channel index in a multichannel setup
    /// @return Filtered sample
    T filter(T in, std::size_t channel = 0);

    /// Filters N channels and one time-step at the time
    /// This is very suitable for N channels interleaved into single vector
    /// Use std::for_each for traversing an array with interleaved channels in N size vector
    /// @param in Input sample vector for time instant t, N parallel channels in the vector
    /// @return Filtered output sample vector. N parallel channels in the vector
    std::vector<T> filter_interleaved(std::vector<T> const& in);

    /// Filters N channels, each channel in its own vector
    /// Channels are filtered _IN_PLACE_
    /// Use this interface if channels are NOT interleaved
    /// @param in Each channel in its own vector filtered in-place
    void filter_channels(std::vector<std::vector<T> >& in);

private:
    std::vector<cnl::dsp::biquad<T> > m_biquad;
};

template<class T>
biquad_cascade<T>::biquad_cascade(std::vector<std::array<T, 5> > const& coeff,
                                unsigned int channelCount)
{
    this->init(coeff, channelCount);
}

template<class T>
void biquad_cascade<T>::init(std::vector<std::array<T, 5> > const& coeff,
                             unsigned int channelCount)
{
    m_biquad.resize(coeff.size());
    auto v = coeff.begin();
    for (auto it = m_biquad.begin(); it != m_biquad.end(); ++it, ++v)
    {
        it->init(*v, channelCount);
    }
}

template<class T>
void biquad_cascade<T>::clear()
{
    for (auto& biquad : m_biquad)
    {
        biquad.clear();
    }
}

template<class T>
T biquad_cascade<T>::filter(T in, std::size_t channel)
{
    T accu = in;
    for (auto& biquad : m_biquad)
    {
        accu = biquad.filter(accu, channel);
    }
    return accu;
}

template<class T>
std::vector<T> biquad_cascade<T>::filter_interleaved(std::vector<T> const& in)
{
    std::vector<T> accu = in;
    for (auto& biquad : m_biquad)
    {
        accu = biquad.filter_interleaved(accu);
    }
    return accu;
}

template<class T>
void biquad_cascade<T>::filter_channels(std::vector<std::vector<T> >& in)
{
    std::size_t channels = in.size();
    //Leverage data locality, filter one channel at the time with each biquad
    for (std::size_t ch = 0; ch < channels; ch++)
    {
        std::for_each(in[ch].begin(),
                      in[ch].end(),
                      [&](T & val)
                      {
                          val = filter(val, ch); //this->filter(val,ch)
                      });
    }
}

} // namespace dsp
} // namespace cnl
#endif //CNL_DSP_BIQUAD_CASCADE
