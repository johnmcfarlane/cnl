
//          Copyright Heikki Berg 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FIXED_POINT_SAFFT_H
#define FIXED_POINT_SAFFT_H

#include <type_traits>
#include <cmath>
#include <cnl/fixed_point.h>

//#define CALCULATE_TWIDDLES_IN_NATIVE_DATATYPE
//#define CALCULATE_WITH_COMPLEX_DATATYPE
//#define USE_MULTIPLY_OPERATOR

namespace Algorithms {

/**
* Various fixedpoint FFT implementations
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
         * Stockham Autosort FFT
         * Reference: "Computational Frameworks for the Fast Fourier Transform",
         * Charles Van Loan, SIAM, 1992.
         * Algorithm 1.7.2. pp. 56-57.
         * Calculates FFT using caller provided work buffers
         * @param vec1 Input data array and will be used as first work array
         * @param vec2 Second work array
         * @return returns 0 if result is in vec1 and 1 if in vec2
         */
        unsigned int sa_fft(std::vector<std::complex<T>> &vec1,
                         std::vector<std::complex<T>> &vec2);

        /**
         * Stockham Autosort IFFT
         * Calculates IFFT using caller provided work buffers
         * @param vec1 Input data array and will be used as first work array
         * @param vec2 Second work array
         * @return returns 0 if result is in vec1 and 1 if in vec2
         */
        unsigned int sa_ifft(std::vector<std::complex<T>> &vec1,
                          std::vector<std::complex<T>> &vec2);

        /**
        * Calculates FFT in-place using Cooley-Tukey algorithm
         * @param vec1 Input data array which will be overwritten by output
        */
        void ct_fft(std::vector<std::complex<T>> &vec1);

        /**
        * Calculates IFFT in-place using Cooley-Tukey algorithm
         * @param vec1 Input data array which will be overwritten by output
        */
        void ct_ifft(std::vector<std::complex<T>> &vec1);

        /**
        * Calculates block floating point FFT in-place using Cooley-Tukey algorithm
         * @param vec1 Input data array which will be overwritten by output
        */
        int bf_fft(std::vector<std::complex<T>> &vec1);

        /**
        * Calculates block floating point IFFT in-place using Cooley-Tukey algorithm
         * @param vec1 Input data array which will be overwritten by output
        */
        int bf_ifft(std::vector<std::complex<T>> &vec1);

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
    static unsigned int sa_fft_core(
            std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &vec1,
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
    static unsigned int sa_fft_core(
            std::vector<std::complex<T>> &vec1,
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

    //Reordering for in-place FFT
    template<class T>
    static void reorder(std::vector<std::complex<T>> &in) {
        unsigned int N = (unsigned int) in.size();
        unsigned int i, j, k;
        std::complex<T> y;

        // Calculate bit reversed pattern, and interchange if greater.
        j = 1;
        for ( i = 1; i<=N-1;i++) {
            if (i<j) {
                y = in[j - 1];
                in[j-1] = in[i-1];
                in[i-1] = y;
            }
            k = (N / 2);
            while (k<j) {
                j = (j - k);
                k = (k / 2);
            }
            j = (j + k);
        }
        return;
    }

    // Cooley-Tukey type in-place FFT
    template<class Rep, int Exponent>
    static void ct_core(std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &vec1,
                 std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &twiddles,
                 int direction_flag) {
        unsigned int N = (unsigned int) vec1.size();
        unsigned int S = (unsigned int) (std::log10((double) N) /
                                         std::log10((double) 2));
        unsigned int stride = ((unsigned int) twiddles.size()) * 2 / N;
        using fixed_point = cnl::fixed_point<Rep, Exponent>;
        using complex = std::complex<fixed_point>;

        unsigned int L, r, L_s;
        complex w, tau;

        reorder(vec1);

        for (unsigned int q = 1; q <= S; ++q) {
            L = (unsigned int) 1 << q;
            L_s = (unsigned int) 1 << (q - 1);
            r = N >> q;

            for (unsigned int j = 0; j < L_s; ++j) {
                w = (direction_flag < 0)
                    ? twiddles[j * r * stride]
                    : std::conj(twiddles[j * r * stride]);
                for (unsigned int k = 0; k < r; ++k) {
#ifdef CALCULATE_WITH_COMPLEX_DATATYPE
                    tau = w * vec1[k * L + j + L_s]; //FAILS
                    vec1[k * L + j + L_s] = vec1[k * L + j] - tau; //FAILS
                    vec1[k * L + j] = vec1[k * L + j] + tau; //FAILS
#else //CALCULATE_WITH_COMPLEX_DATATYPE
                    tau = complex(
                            w.real()*vec1[k * L + j + L_s].real() -
                            w.imag()*vec1[k * L + j + L_s].imag(),
                            w.imag()*vec1[k * L + j + L_s].real() +
                            w.real()*vec1[k * L + j + L_s].imag());
                    vec1[k * L + j + L_s] = complex(
                            vec1[k * L + j].real() - tau.real(),
                            vec1[k * L + j].imag() - tau.imag());
                    vec1[k * L + j] = complex(
                            vec1[k * L + j].real() + tau.real(),
                            vec1[k * L + j].imag() + tau.imag());
#endif //CALCULATE_WITH_COMPLEX_DATATYPE
                }
            }
        }

        return;
    }

    // Cooley-Tukey type in-place FFT
    template<class T>
    static void ct_core(std::vector<std::complex<T>> &vec1,
                 std::vector<std::complex<T>> &twiddles,
                 int direction_flag) {
        unsigned int N = (unsigned int) vec1.size();
        unsigned int S = (unsigned int) (std::log10((double) N) /
                                         std::log10((double) 2));
        unsigned int stride = ((unsigned int) twiddles.size()) * 2 / N;

        unsigned int L, r, L_s, r_s;
        std::complex<T> w, tau;

        reorder(vec1);

        for (unsigned int q = 1; q <= S; ++q) {
            L = (unsigned int) 1 << q;
            L_s = (unsigned int) 1 << (q - 1);
            r = N >> q;
            r_s = N >> (q - 1);

            for (unsigned int j = 0; j < L_s; ++j) {
                w = (direction_flag < 0)
                    ? twiddles[j * r * stride]
                    : std::conj(twiddles[j * r * stride]);
                for (unsigned int k = 0; k < r; ++k) {
                    tau = w * vec1[k * L + j + L_s];
                    vec1[k * L + j + L_s] = vec1[k * L + j] - tau;
                    vec1[k * L + j] = vec1[k * L + j] + tau;
                }
            }
        }

        return;
    }

    template<class T>
    static T find_max(std::vector<std::complex<T>> &in) {
        unsigned int N = (unsigned int) in.size();
        T max = 0;

        for (unsigned int i = 0; i<N;++i){
            if (abs(in[i].real()) > max) {
                max = abs(in[i].real());
            }
            if (abs(in[i].imag()) > max) {
                max = abs(in[i].imag());
            }
        }
        return max;
    }

    template<class T>
    static int normalize(std::vector<std::complex<T>> &in, int adjustment) {
        T max = find_max(in);
        int norm = leading_bits(max) - (cnl::digits<decltype(max)>::value + max.exponent) - adjustment;
        if (norm < 0)  {
            for (unsigned int i = 0;i<(unsigned int)in.size(); ++i) {
                in[i] = std::complex<T> (in[i].real() >> -norm,
                                         in[i].imag() >> -norm);
            }
        } else {
            for (unsigned int i = 0; i < (unsigned int) in.size(); ++i) {
                in[i] = std::complex<T>(in[i].real() << norm,
                                        in[i].imag() << norm);
            }
        }
        return norm;
    }

    // Cooley-Tukey type in-place FFT, but with block floating point arithmetic
    // maximizing the SNR in each stage for limited word length accumulator as
    // described in "A Block Floating Point Implementation for an N-Point FFT
    // on the TMS320C55x DSP"
    template<class Rep, int Exponent>
    static int bf_core(std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &vec1,
                    std::vector<std::complex<cnl::fixed_point<Rep, Exponent>>> &twiddles,
                    int direction_flag) {
        unsigned int N = (unsigned int) vec1.size();
        unsigned int S = (unsigned int) (std::log10((double) N) /
                                         std::log10((double) 2));
        unsigned int stride = ((unsigned int) twiddles.size()) * 2 / N;
        using fixed_point = cnl::fixed_point<Rep, Exponent>;
        using complex = std::complex<fixed_point>;

        unsigned int L, r, L_s;
        complex w, tau;
        int total_scale = 0;

        reorder(vec1);

        for (unsigned int q = 1; q <= S; ++q) {
            L = (unsigned int) 1 << q;
            L_s = (unsigned int) 1 << (q - 1);
            r = N >> q;
            int adj = (q > 2) ? 2 : 1;
            total_scale += normalize(vec1, adj);

            for (unsigned int j = 0; j < L_s; ++j) {
                w = (direction_flag < 0)
                    ? twiddles[j * r * stride]
                    : std::conj(twiddles[j * r * stride]);
                for (unsigned int k = 0; k < r; ++k) {
#ifdef CALCULATE_WITH_COMPLEX_DATATYPE
                    tau = w * vec1[k * L + j + L_s]; //FAILS
                    vec1[k * L + j + L_s] = vec1[k * L + j] - tau; //FAILS
                    vec1[k * L + j] = vec1[k * L + j] + tau; //FAILS
#else //CALCULATE_WITH_COMPLEX_DATATYPE
                    tau = complex(
                            w.real()*vec1[k * L + j + L_s].real() -
                            w.imag()*vec1[k * L + j + L_s].imag(),
                            w.imag()*vec1[k * L + j + L_s].real() +
                            w.real()*vec1[k * L + j + L_s].imag());
                    vec1[k * L + j + L_s] = complex(
                            vec1[k * L + j].real() - tau.real(),
                            vec1[k * L + j].imag() - tau.imag());
                    vec1[k * L + j] = complex(
                            vec1[k * L + j].real() + tau.real(),
                            vec1[k * L + j].imag() + tau.imag());
#endif //CALCULATE_WITH_COMPLEX_DATATYPE
                }
            }
        }
        return total_scale;
    }


    template<class T>
    unsigned int SaFFT<T>::sa_fft(std::vector<std::complex<T>> &vec1,
                               std::vector<std::complex<T>> &vec2) {
        //TODO: check that the requested FFT size < supported by twiddle table
        vec2.resize(vec1.size());
        unsigned int ret = sa_fft_core(vec1, vec2, m_TwiddleTable, 1);
        return ret;
    }


    template<class T>
    unsigned int SaFFT<T>::sa_ifft(std::vector<std::complex<T>> &vec1,
                                std::vector<std::complex<T>> &vec2) {
        //TODO: check that the requested FFT size < supported by twiddle table
        vec2.resize(vec1.size());
        unsigned int ret = sa_fft_core(vec1, vec2, m_TwiddleTable, -1);
        return ret;
    }

    template<class T>
    void SaFFT<T>::ct_fft(std::vector<std::complex<T>> &vec1) {
        //TODO: check that the requested FFT size < supported by twiddle table
        ct_core(vec1, m_TwiddleTable, 1);
    }


    template<class T>
    void SaFFT<T>::ct_ifft(std::vector<std::complex<T>> &vec1) {
        //TODO: check that the requested FFT size < supported by twiddle table
        ct_core(vec1, m_TwiddleTable, -1);
    }

    template<class T>
    int SaFFT<T>::bf_fft(std::vector<std::complex<T>> &vec1) {
        //TODO: check that the requested FFT size < supported by twiddle table
        return bf_core(vec1, m_TwiddleTable, 1);
    }


    template<class T>
    int SaFFT<T>::bf_ifft(std::vector<std::complex<T>> &vec1) {
        //TODO: check that the requested FFT size < supported by twiddle table
        return bf_core(vec1, m_TwiddleTable, -1);
    }

}
#endif //FIXED_POINT_SAFFT_H


