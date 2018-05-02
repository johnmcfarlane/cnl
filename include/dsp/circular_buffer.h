//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#if !defined(CNL_DSP_CIRCULAR_BUFFER)
#define CNL_DSP_CIRCULAR_BUFFER

#include <algorithm>
#include <vector>

namespace cnl
{
namespace dsp
{

template<typename T>
class circular_buffer
{
public:
    /// Ctor
    explicit circular_buffer(std::size_t size = 1) :
        m_buffer(size, T{}),
        m_last(0)
    {
    };

    ~circular_buffer() = default;

    /// Resizes the buffer
    void resize(std::size_t size);

    /// Clears the contents of the circular buffer
    void clear();

    /// Replaces the oldest value in the circular buffer and steps forward
    void push_back(T const& data);

    /// Returns reference to the value, 0 index being latest, 1 second latest
    T& operator[](std::size_t index);
private:
    std::vector<T> m_buffer;
    /// Points to the latest inserted data sample
    std::size_t m_last;
};


template<class T>
void circular_buffer<T>::resize(std::size_t size)
{
    m_buffer.resize(size);
    std::fill(m_buffer.begin(), m_buffer.end(), T{});
    m_last = 0;
}

template<class T>
void circular_buffer<T>::clear()
{
    std::fill(m_buffer.begin(), m_buffer.end(), T{});
}

template<class T>
T& circular_buffer<T>::operator[](std::size_t index)
{
    return m_buffer[(m_last + m_buffer.size() - index) % m_buffer.size()];
}

template<class T>
void circular_buffer<T>::push_back(T const& data)
{
    m_last = (m_last + 1) % m_buffer.size();
    m_buffer[m_last] = data;
}

} //namespace dsp
} //namespace cnl
#endif //CNL_DSP_CIRCULAR_BUFFER