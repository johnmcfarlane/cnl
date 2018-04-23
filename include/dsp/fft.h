//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_FFT)
#define CNL_DSP_FFT

#include "dsp_types.h"
#include "dsp_math.h"

#include "complex.h"
#include "complex_vector.h"
#include "trig.h"

namespace cnl
{
namespace dsp
{
///Various fixedpoint FFT implementations
namespace fft
{
/// Calculates block floating point FFT in-place using Cooley-Tukey algorithm
/// @param vector Input data array which will be overwritten by output
template<typename T>
int fft(complex_vector<T>& vector);

/// Calculates block floating point IFFT in-place using Cooley-Tukey algorithm
/// @param vector Input data array which will be overwritten by output
template<typename T>
int ifft(complex_vector<T>& vector);

/// Calculates block floating point FFT using Cooley-Tukey algorithm
/// @param in real  valued input data array of size FFT_SIZE
/// @param out Output data array of size 1+FFT_SIZE/2
template<typename T>
int real_fft(std::vector<T> const& in, complex_vector<T>& out);

///////////////////////////////
/////// IMPLEMENTATIONS ///////
///////////////////////////////

// Reordering for in-place FFT
template<typename T>
static void reorder(complex_vector<T>& in)
{
    std::size_t N = in.size();
    std::size_t i, j, k;
    std::vector<T>& inreal = in.real_data();
    std::vector<T>& inimag = in.imag_data();
    T real;
    T imag;

    // Calculate bit reversed pattern, and interchange if greater.
    j = 1;
    for (i = 1; i <= N - 1; i++)
    {
        if (i < j)
        {
            real = inreal[j - 1];
            inreal[j - 1] = inreal[i - 1];
            inreal[i - 1] = real;
            imag = inimag[j - 1];
            inimag[j - 1] = inimag[i - 1];
            inimag[i - 1] = imag;
        }
        k = (N / 2);
        while (k < j)
        {
            j = (j - k);
            k = (k / 2);
        }
        j = (j + k);
    }
}

// Cooley-Tukey type in-place FFT
template<typename T>
static void ct_core(complex_vector<T>& inout,
                    bool inverseTransform)
{
    cnl::dsp::trig<T>& exp = cnl::dsp::trig<T>::instance();
    auto N = static_cast<unsigned int>(inout.size());
    auto S = static_cast<unsigned int>(std::log2(N));
    auto stride = exp.get_twopi_index() >> S;
    reorder(inout);

    for (unsigned int q = 1; q <= S; ++q)
    {
        unsigned int L = 1u << q;
        unsigned int L_s = 1u << (q - 1);
        unsigned int r = N >> q;

        for (unsigned int j = 0; j < L_s; ++j)
        {
            complex<T> tw = (inverseTransform) ? exp[j * r * stride]
                            : conj(exp[j * r * stride]);
            for (unsigned int k = 0; k < r; ++k)
            {
                complex<T> tau = tw * inout[k * L + j + L_s];
                complex<T> x1 = inout[k * L + j] - tau;
                complex<T> x2 = inout[k * L + j] + tau;
                inout.set_at(k * L + j + L_s, x1);
                inout.set_at(k * L + j, x2);
            }
        }
    }
}

template<typename T>
static T find_max(complex_vector<T>& in)
{
    auto N = static_cast<unsigned int>(in.size());
    std::vector<T> const& inreal = in.real();
    std::vector<T> const& inimag = in.imag();
    T max{0.};

    for (unsigned int i = 0; i < N; ++i)
    {
        max = std::max(max, abs(inreal[i]));
        max = std::max(max, abs(inimag[i]));
    }
    return max;
}

template<typename T>
static void normalize(complex_vector<T>& in, int norm)
{
    std::vector<T>& inreal = in.real_data();
    std::vector<T>& inimag = in.imag_data();
    if (norm < 0)
    {
        for (std::size_t i = 0; i < in.size(); ++i)
        {
            inreal[i] = inreal[i] >> -norm;
            inimag[i] = inimag[i] >> -norm;
        }
    }
    else
    {
        for (std::size_t i = 0; i < in.size(); ++i)
        {
            inreal[i] = inreal[i] << norm;
            inimag[i] = inimag[i] << norm;
        }
    }
}

template<typename T>
static int bf_core(complex_vector<T>& inout,
                   bool inverseTransform)
{
    cnl::dsp::trig<T>& exp = cnl::dsp::trig<T>::instance();
    auto N = static_cast<unsigned int>(inout.size());
    auto S = static_cast<unsigned int>(std::log2(N));
    auto stride = exp.get_twopi_index() >> S;

    int exponent = 0;
    reorder(inout);
    T max = find_max(inout);

    for (unsigned int q = 1; q <= S; ++q)
    {
        unsigned int L = 1u << q;
        unsigned int L_s = 1u << (q - 1);
        unsigned int r = N >> q;
        int adj = (q > 2) ? 3 : 2;
        int norm = cnl::dsp::math::leading_bits(max) - adj;
        normalize(inout, norm);
        exponent -= norm;
        max = 0.;

        for (unsigned int j = 0; j < L_s; ++j)
        {
            complex<T> tw = (inverseTransform) ? exp[j * r * stride]
                                               : conj(exp[j * r * stride]);
            for (unsigned int k = 0; k < r; ++k)
            {
                complex<T> tau = tw * inout[k * L + j + L_s];
                complex<T> x1 = inout[k * L + j] - tau;
                complex<T> x2 = inout[k * L + j] + tau;
                inout.set_at(k * L + j + L_s, x1);
                inout.set_at(k * L + j, x2);
                max = std::max(max, abs(x1.real()));
                max = std::max(max, abs(x1.imag()));
                max = std::max(max, abs(x2.real()));
                max = std::max(max, abs(x2.imag()));
            }
        }
    }
    return exponent;
}

/// Adapted from:
/// Richard G. Lyons "Understanding Digital Signal Processing" pp. 694-699.
/// for inplace modification of the input array.
template<class T>
static void real_fft_postprocess(complex_vector<T>& out)
{
    unsigned int N = static_cast<unsigned int>(out.size());
    unsigned int S = static_cast<unsigned int>(std::log2(N));
    cnl::dsp::trig<T>& exp = cnl::dsp::trig<T>::instance();
    unsigned int stride = exp.get_twopi_index() >> (S + 1);

    // out[N] equals to out[0], so, let's add it
    out.push_back(out[0]);

    // Process the first and last index
    unsigned int k = 0;
    unsigned int r = N;
    complex<T> xPlus(cnl::dsp::math::arithmetic_right_shift(out[k].real() + out[r].real(), 1),
                     cnl::dsp::math::arithmetic_right_shift(out[k].imag() + out[r].imag(), 1));
    complex<T> xMinus(cnl::dsp::math::arithmetic_right_shift(out[k].real() - out[r].real(), 1),
                      cnl::dsp::math::arithmetic_right_shift(out[k].imag() - out[r].imag(), 1));
    complex<T> x1(xPlus.real(), xMinus.imag());
    complex<T> x2(xPlus.imag(), xMinus.real());

    complex<T> xa = x1 + conj(exp[k * stride] * x2);
    complex<T> xb(out[k].real() - out[k].imag(), T(0));
    out.set_at(k, xa);
    out.set_at(r, xb);

    if (N > 1) {
        // Process the remaining in place to both directions simultaneously
        for (k = 1; k < N / 2; k++) {
            r = N - k;
            xPlus = complex<T>(cnl::dsp::math::arithmetic_right_shift(out[k].real() + out[r].real(), 1),
                               cnl::dsp::math::arithmetic_right_shift(out[k].imag() + out[r].imag(), 1));
            xMinus = complex<T>(cnl::dsp::math::arithmetic_right_shift(out[k].real() - out[r].real(), 1),
                                cnl::dsp::math::arithmetic_right_shift(out[k].imag() - out[r].imag(), 1));
            x1 = complex<T>(xPlus.real(), xMinus.imag());
            x2 = complex<T>(xPlus.imag(), xMinus.real());
            xa = x1 + conj(exp[k * stride] * x2);
            xb = conj(x1) + x2 * conj(exp[r * stride]);
            out.set_at(k, xa);
            out.set_at(r, xb);
        }

        // And don't forget the midpoint
        k = N / 2;
        r = N - k;
        xPlus = complex<T>(cnl::dsp::math::arithmetic_right_shift(out[k].real() + out[r].real(), 1),
                           cnl::dsp::math::arithmetic_right_shift(out[k].imag() + out[r].imag(), 1));
        xMinus = complex<T>(cnl::dsp::math::arithmetic_right_shift(out[k].real() - out[r].real(), 1),
                            cnl::dsp::math::arithmetic_right_shift(out[k].imag() - out[r].imag(), 1));
        x1 = complex<T>(xPlus.real(), xMinus.imag());
        x2 = complex<T>(xPlus.imag(), xMinus.real());
        xa = x1 + conj(exp[k * stride] * x2);
        out.set_at(k, xa);
    }
}


template<class T>
int fft(complex_vector<T>& vector)
{
    ct_core(vector, false);
    return 0;
}

template<>
inline int fft(complex_vector<q4_20>& vector)
{
    return bf_core(vector, false);
}

#if defined(CNL_INT128_ENABLED)
template<>
inline int fft(complex_vector<q8_40>& vector)
{
    return bf_core(vector, false);
}
#endif

template<class T>
int ifft(complex_vector<T>& vector)
{
    ct_core(vector, true);
    return 0;
}

template<>
inline int ifft(complex_vector<q4_20>& vector)
{
    return bf_core(vector, true);
}

#if defined(CNL_INT128_ENABLED)
template<>
inline int ifft(complex_vector<q8_40>& vector)
{
    return bf_core(vector, true);
}
#endif

template<class T>
int real_fft(std::vector<T> const& in, complex_vector<T>& out)
{
    out.reserve(1 + in.size() / 2);
    out.resize(in.size() / 2);
    std::size_t index = 0;
    for (auto input = in.begin(); input != in.end(); input += 2, index++)
    {
        out.set_at(index, complex<T>(*input, *(input + 1)));
    }
    ct_core(out, false);
    real_fft_postprocess(out);
    return 0;
}

template<>
inline int real_fft(std::vector<q4_20> const& in, complex_vector<q4_20>& out)
{
    out.reserve(1 + in.size() / 2);
    out.resize(in.size() / 2);
    std::size_t index = 0;
    for (auto input = in.begin(); input != in.end(); input += 2, index++)
    {
        out.set_at(index, complex<q4_20>(*input, *(input + 1)));
    }
    int exponent = bf_core(out, false);
    real_fft_postprocess(out);
    return exponent;
}

#if defined(CNL_INT128_ENABLED)
template<>
inline int real_fft(std::vector<q8_40> const& in, complex_vector<q8_40>& out)
{
    out.reserve(1 + in.size() / 2);
    out.resize(in.size() / 2);
    std::size_t index = 0;
    for (auto input = in.begin(); input != in.end(); input += 2, index++)
    {
        out.set_at(index, complex<q8_40>(*input, *(input + 1)));
    }
    int exponent = bf_core(out, false);
    real_fft_postprocess(out);
    return exponent;
}
#endif

} // namespace fft
} // namespace dsp
} // namespace cnl
#endif //CNL_DSP_FFT
