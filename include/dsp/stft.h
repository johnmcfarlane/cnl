//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_STFT)
#define CNL_DSP_STFT

#include <algorithm>
#include <cassert>
#include <vector>

#include "dsp_types.h"
#include "dsp_math.h"

#include "complex_vector.h"
#include "fft.h"

namespace cnl
{
namespace dsp
{

template<class T>
class stft
{
public:
    stft(unsigned int fftSize = 2048, unsigned int channels = 1);

    ~stft();

    void init(unsigned int fftSize, unsigned int channels);

    void transform(std::vector<std::vector<T> > const& input,
                   std::vector<complex_vector<T> >& output,
                   std::vector<int>& exponents);

    void clear();

    void set_window(std::vector<T>& win);
    void set_boxcar_window();
    void set_hann_window();
    void get_window(std::vector<T>& window);

private:
    std::vector<std::vector<T> > m_prevSigBuffer;
    std::vector<T> m_window;
    std::vector<T> m_tmp;
    unsigned int m_fftSize;
    unsigned int m_channels;
};

///////////////////////////////
/////// IMPLEMENTATIONS ///////
///////////////////////////////

template<class T>
stft<T>::stft(unsigned int fftSize, unsigned int channels)
{
    init(fftSize, channels);
    set_hann_window();
}

template<class T>
void stft<T>::init(unsigned int fftSize, unsigned int channels)
{
    m_fftSize = fftSize;
    m_channels = channels;
    m_prevSigBuffer.resize(m_channels);
    for (std::vector<T>& v : m_prevSigBuffer)
    {
        v.resize(m_fftSize / 2, T(0));
    }
    m_window.resize(m_fftSize);
    m_tmp.resize(m_fftSize, T(0));
    clear();
}

template<class T>
void stft<T>::clear()
{
    for (std::vector<T>& v : m_prevSigBuffer)
    {
        std::fill(v.begin(), v.end(), T(0));
    }
}

template<class T>
stft<T>::~stft()
{
}

template<class T>
void stft<T>::set_window(std::vector<T>& win)
{
    assert(win.size() == m_fftSize);
    m_window = win;
}

template<class T>
void stft<T>::set_boxcar_window()
{
    std::fill(m_window.begin(), m_window.end(), T(1));
}

template<class T>
void stft<T>::set_hann_window()
{
    cnl::dsp::trig<T>& trig = cnl::dsp::trig<T>::instance();
    unsigned int twoPI = trig.get_twopi_index();
    unsigned int stride = (twoPI >> 1) / m_fftSize;
    assert(stride > 0);
    for (unsigned int i = 0; i < m_fftSize; ++i)
    {
        // Calculates sin(pi * i / fftSize)
        m_window[i] = trig.sin_at(i * stride);
    }
}

template<class T>
void stft<T>::get_window(std::vector<T>& win)
{
    win = m_window;
}

template<class T>
void stft<T>::transform(std::vector<std::vector<T> > const& input,
                        std::vector<complex_vector<T> >& output,
                        std::vector<int>& exponents)
{
    assert(m_channels == static_cast<unsigned int>(input.size()));
    assert(m_channels == static_cast<unsigned int>(output.size()));
    // Perhaps this could be very nicely written with range-for loop?
    for (unsigned int ch = 0; ch < m_channels; ch++)
    {
        // Element wise multiply of previous signal vector with
        // beginning half of window function, result to m_tmp.
        std::transform(m_prevSigBuffer[ch].begin(),
                       m_prevSigBuffer[ch].end(),
                       m_window.begin(),
                       m_tmp.begin(),
                       cnl::dsp::math::multiplies<T, T>());
        // Element wise multiply of new input signal vector with
        // latter half of window function, result to latter half
        // of m_tmp.
        std::transform(input[ch].begin(),
                       input[ch].end(),
                       m_window.begin() +
                       m_window.size() / 2,
                       m_tmp.begin() + m_window.size() / 2,
                       cnl::dsp::math::multiplies<T, T>());
        // Save the new input signal vector to previous for the next call
        std::copy(input[ch].begin(),
                  input[ch].end(),
                  m_prevSigBuffer[ch].begin());
        // Transform the windowed vector to complex numbers
        exponents[ch] = cnl::dsp::fft::real_fft(m_tmp, output[ch]);
    }
}
} //namespace dsp
} //namespace cnl
#endif //CNL_DSP_STFT