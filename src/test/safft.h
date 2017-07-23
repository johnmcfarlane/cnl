
//          Copyright Heikki Berg 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FIXED_POINT_SAFFT_H
#define FIXED_POINT_SAFFT_H

#include <type_traits>
#include <cmath>
#include <sg14/fixed_point>
#include "safft.h"

//#define CALCULATE_TWIDDLES_IN_NATIVE_DATATYPE
//#define CALCULATE_WITH_COMPLEX_DATATYPE
//#define USE_MULTIPLY_OPERATOR

namespace Algorithms {

/**
* Stockham Autosort FFT
* Reference: "Computational Frameworks for the Fast Fourier Transform",
* Charles Van Loan, SIAM, 1992.
* Algorithm 1.7.2. pp. 56-57.
* Memory access pattern suitable for vector machines compared to
* traditional Cooley-Tukey.
*/
    template<class T>
    class SaFFT {
    public:
        /**
         * @param maxSize Maximum size for FFT
         */
        SaFFT(unsigned int maxSize);

        ~SaFFT();

        /**
         * Calculates FFT using caller provided work buffers
         * @param vec1 Input data array and will be used as first work array
         * @param vec2 Second work array
         * @return returns 0 if result is in vec1 and 1 if in vec2
         */
        unsigned int fft(std::vector<std::complex<T>> &vec1,
                         std::vector<std::complex<T>> &vec2);

        /**
         * Calculates IFFT using caller provided work buffers
         * @param vec1 Input data array and will be used as first work array
         * @param vec2 Second work array
         * @return returns 0 if result is in vec1 and 1 if in vec2
         */
        unsigned int ifft(std::vector<std::complex<T>> &vec1,
                          std::vector<std::complex<T>> &vec2);

    protected:
    private:
        /**
         * Precalculated twiddle factors
         */
        std::vector<std::complex<T>> m_TwiddleTable;
    };

    template<class T>
    SaFFT<T>::SaFFT(unsigned int maxSize) {
#ifdef CALCULATE_TWIDDLES_IN_NATIVE_DATATYPE
        //TODO: Unsatisfactory performance with fixed_point datatype
        T PI = static_cast<T>(M_PI);
        unsigned int half_n = maxSize >> 1;
        m_TwiddleTable.resize(half_n);
        for (unsigned int i = 0; i < half_n; ++i) {
            T angle = static_cast<T> (i) / static_cast<T> (half_n);
            angle *= PI;
            m_TwiddleTable[i] = std::complex<T>(cos(angle), sin(angle));
        }
#else
        double PI = M_PI;
        unsigned int half_n = maxSize >> 1;
        m_TwiddleTable.resize(half_n);
        for (unsigned int i = 0; i < half_n; ++i) {
            double angle = (double)i / double(half_n);
            angle *= PI;
            m_TwiddleTable[i] = std::complex<T>((T)cos(angle),(T)sin(angle));
        }
#endif
    }

    template<class T>
    SaFFT<T>::~SaFFT() {
    }

    template<class Rep, int Exponent>
    unsigned int fft_core(std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &vec1,
                          std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &vec2,
                          std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &twiddles,
                          int direction_flag) {
        using fixed_point = cnl::fixed_point<Rep, Exponent>;
        using complex = std::complex<fixed_point>;
        unsigned int N = (unsigned int) vec1.size();
        unsigned int S = (unsigned int) (std::log10((double) N) /
                                         std::log10((double) 2));
        unsigned int stride = ((unsigned int) twiddles.size()) * 2 / N;

        unsigned int r, L_s, r_s;
        complex w, tau;

        complex *xp, *yp, *tp;
        yp = &vec1[0]; //Initial input buffer
        xp = &vec2[0]; //Initial output buffer

        for (unsigned int q = 1; q <= S; ++q) {
            L_s = (unsigned int) 1 << (q - 1);
            r = N >> q;
            r_s = N >> (q - 1);

            for (unsigned int j = 0; j < L_s; ++j) {
                w = (direction_flag < 0)
                    ? twiddles[j * r * stride]
                    : std::conj(twiddles[j * r * stride]);
                for (unsigned int k = 0; k < r; ++k) {
#ifdef CALCULATE_WITH_COMPLEX_DATATYPE
                    tau = w * yp[j * r_s + k + r]; //FAILS
                    xp[j * r + k] = yp[j * r_s + k] + tau; //FAILS
                    xp[(j + L_s) * r + k] = yp[j * r_s + k] - tau; //FAILS
#else //CALCULATE_WITH_COMPLEX_DATATYPE
#ifdef USE_MULTIPLY_OPERATOR
                    tau = complex(
                            w.real()*yp[j * r_s + k + r].real() -
                            w.imag()*yp[j * r_s + k + r].imag(),
                            w.imag()*yp[j * r_s + k + r].real() +
                            w.real()*yp[j * r_s + k + r].imag());
#else //USE_MULTIPLY_OPERATOR
                    tau = complex(
                            multiply(w.real(), yp[j * r_s + k + r].real()) -
                            multiply(w.imag(), yp[j * r_s + k + r].imag()),
                            multiply(w.imag(), yp[j * r_s + k + r].real()) +
                            multiply(w.real(), yp[j * r_s + k + r].imag()));
#endif //USE_MULTIPLY_OPERATOR
                    xp[j * r + k] = complex(
                            yp[j * r_s + k].real() + tau.real(),
                            yp[j * r_s + k].imag() + tau.imag());
                    xp[(j + L_s) * r + k] = complex(
                            yp[j * r_s + k].real() - tau.real(),
                            yp[j * r_s + k].imag() - tau.imag());
#endif //CALCULATE_WITH_COMPLEX_DATATYPE
                }
            }
            tp = xp;
            xp = yp;
            yp = tp;
        }
        // Return in which buffer the output is
        unsigned int ret = 0;
        if (S % 2 == 1) ret = 1;
        return ret;
    }

    template<class T>
    unsigned int fft_core(std::vector<std::complex<T>> &vec1,
                          std::vector<std::complex<T>> &vec2,
                          std::vector<std::complex<T>> &twiddles,
                          int direction_flag) {
        unsigned int N = (unsigned int) vec1.size();
        unsigned int S = (unsigned int) (std::log10((double) N) /
                                         std::log10((double) 2));
        unsigned int stride = ((unsigned int) twiddles.size()) * 2 / N;

        unsigned int r, L_s, r_s;
        std::complex<T> w, tau;

        std::complex<T> *xp, *yp, *tp;
        yp = &vec1[0]; //Initial input buffer
        xp = &vec2[0]; //Initial output buffer

        for (unsigned int q = 1; q <= S; ++q) {
            L_s = (unsigned int) 1 << (q - 1);
            r = N >> q;
            r_s = N >> (q - 1);

            for (unsigned int j = 0; j < L_s; ++j) {
                w = (direction_flag < 0)
                    ? twiddles[j * r * stride]
                    : std::conj(twiddles[j * r * stride]);
                for (unsigned int k = 0; k < r; ++k) {
                    tau = w * yp[j * r_s + k + r];
                    xp[j * r + k] = yp[j * r_s + k] + tau;
                    xp[(j + L_s) * r + k] = yp[j * r_s + k] - tau;
                }
            }
            tp = xp;
            xp = yp;
            yp = tp;
        }
        // Return in which buffer the output is
        unsigned int ret = 0;
        if (S % 2 == 1) ret = 1;
        return ret;
    }

    template<class T>
    unsigned int SaFFT<T>::fft(std::vector<std::complex<T>> &vec1,
                               std::vector<std::complex<T>> &vec2) {
        //TODO: check that the requested FFT size < supported by twiddle table
        vec2.resize(vec1.size());
        unsigned int ret = fft_core(vec1, vec2, m_TwiddleTable, 1);
        return ret;
    }


    template<class T>
    unsigned int SaFFT<T>::ifft(std::vector<std::complex<T>> &vec1,
                                std::vector<std::complex<T>> &vec2) {
        //TODO: check that the requested FFT size < supported by twiddle table
        vec2.resize(vec1.size());
        unsigned int ret = fft_core(vec1, vec2, m_TwiddleTable, -1);
        return ret;
    }
}
#endif //FIXED_POINT_SAFFT_H


