/*
 * FIR filter class, by Mike Perkins
 *
 * a simple C++ class for linear phase FIR filtering
 *
 * For background, see the post http://www.cardinalpeak.com/blog?p=1841
 *
 * Copyright (c) 2013, Cardinal Peak, LLC.  http://www.cardinalpeak.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3) Neither the name of Cardinal Peak nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * CARDINAL PEAK, LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "fir.hpp"
#include <fstream>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>


class toss
{
    public:
        toss() {}

        template <typename Type>
            toss & operator << (const Type & value)
            {
                _ss << value;
                return *this;
            }

        operator std::string () const { return _ss.str(); }

    private:
        std::stringstream _ss;
};

// Handles all filter cases LPF, HPF, BPF)
Filter::Filter(filterType filt_t, int num_taps, double Fs, double F0, double F1) :
    m_filt_t(filt_t), m_gain(0.0), m_Fs(Fs), m_F0(F0),
    m_F1(F1), m_phi(M_PI * F1 / (Fs/2)), m_lambda(M_PI * F0/(Fs/2)),
    m_taps(num_taps), m_sr(num_taps)
{
    if(Fs <= 0 ) throw std::runtime_error(toss() << "Fs must be > 0");
    if((F1 != 0.0) && (F0 >= F1 )) throw std::runtime_error(toss() << "F0 must be > F1");
    if(F0 <= 0) throw std::runtime_error(toss() << "F0 must be > 0");
    if(F0 >= Fs/2 ) throw std::runtime_error(toss() << "F0 must be < Fs");
    if((F1 != 0.0) && (F1 <= 0)) throw std::runtime_error(toss() << "F1 must be 0.0 (LPF/HPF), or > 0 for BPF");
    if((F1 != 0.0) && (F1 >= Fs/2)) throw std::runtime_error(toss() << "F1 must be < Fs/2");
    if(num_taps <= 0 || num_taps > MAX_NUM_FILTER_TAPS ) throw std::runtime_error(toss() << "0 < num_taps <= " << MAX_NUM_FILTER_TAPS);

    if( m_filt_t == BPF ) designBPF();
    else if( m_filt_t == LPF ) designLPF();
    else if( m_filt_t == HPF ) designHPF();
    else throw std::runtime_error(toss() << "Valid filter types are {LPF, HPF, BPF}" );

    /* Calculate gain from the calculated tap coefficients */
    m_gain = std::accumulate(get_taps().begin(), get_taps().end(), (double)0.0);

    return;
}

void Filter::designLPF()
{
    for(size_t n = 0; n < m_taps.size(); n++)
    {
        double mm = n - (m_taps.size() - 1.0) / 2.0;
        if( mm == 0.0 )
            m_taps[n] = m_lambda / M_PI;
        else
            m_taps[n] = sin( mm * m_lambda ) / (mm * M_PI);
    }

    return;
}

void Filter::designHPF()
{
    for(size_t n = 0; n < m_taps.size(); n++)
    {
        double mm = n - (m_taps.size() - 1.0) / 2.0;
        if( mm == 0.0 ) m_taps[n] = 1.0 - m_lambda / M_PI;
        else m_taps[n] = -sin( mm * m_lambda ) / (mm * M_PI);
    }

    return;
}

void Filter::designBPF()
{
    for(size_t n = 0; n < m_taps.size(); n++)
    {
        double mm = n - (m_taps.size() - 1.0) / 2.0;
        if( mm == 0.0 ) m_taps[n] = (m_phi - m_lambda) / M_PI;
        else m_taps[n] = ( sin( mm * m_phi ) -
                sin( mm * m_lambda ) ) / (mm * M_PI);
    }

    return;
}

const std::vector<double>& Filter::get_taps()
{
    return m_taps;
}

void Filter::write_taps_to_file(const char *filename )
{
    std::ofstream fd;
    fd.open(filename);
    fd << m_taps.size() << std::endl;
    for (std::vector<double>::const_iterator it = m_taps.begin();
            it != m_taps.end(); ++it)
        fd << std::fixed << std::setw(15) << std::setprecision(6)
            << *it << std::endl;
}

// Output the magnitude of the frequency response in dB
#define NP 1000
void Filter::write_freqres_to_file(const char *filename )
{
    double w;
    double dw;
    double y_r[NP];
    double y_i[NP];
    double y_mag[NP];
    double mag_max = -1;
    double tmp_d;

    dw = M_PI / (NP - 1.0);
    for(int i = 0; i < NP; i++)
    {
        w = i*dw;
        y_r[i] = 0;
        y_i[i] = 0;
        for(size_t k = 0; k < m_taps.size(); k++)
        {
            y_r[i] += m_taps[k] * cos(k * w);
            y_i[i] -= m_taps[k] * sin(k * w);
        }
    }

    for(int i = 0; i < NP; i++)
    {
        y_mag[i] = sqrt( y_r[i] * y_r[i] + y_i[i] * y_i[i] );
        if( y_mag[i] > mag_max )
            mag_max = y_mag[i];
    }

    if( mag_max <= 0.0 )
        throw std::runtime_error(toss() << "invalid mag_max: " << mag_max);

    std::ofstream fd;
    fd.open(filename);

    for(int i = 0; i < NP; i++)
    {
        w = i*dw;
        if( y_mag[i] == 0 )
            tmp_d = -100;
        else
        {
            tmp_d = 20 * log10( y_mag[i] / mag_max );
            if( tmp_d < -100 )
                tmp_d = -100;
        }
        fd << std::fixed << std::setprecision(10)
            << w * (m_Fs/2)/M_PI << " " << tmp_d << std::endl;
    }
}

double Filter::gain()
{
    return m_gain;
}

double Filter::do_sample(double data_sample)
{
    /* Shift on the new sample, pop off an old */
    m_sr.push_front(data_sample);
    m_sr.pop_back();

    double result = 0;
    for(size_t i = 0; i < m_taps.size(); i++)
        result += m_sr[i] * m_taps[i];

    return result * 1/gain();
}

std::string Filter::__str__()
{
    std::ostringstream oss;
    oss << (m_filt_t == LPF ? "LPF" : m_filt_t == HPF ? "HPF" : "BPF")
        << " Fs: " << m_Fs << (m_filt_t < BPF ? " Fc" : " Fl" ) << ": " << m_F0;
    if (m_filt_t == BPF) oss << " Fh: " << m_F1;
    oss << " gain: " << gain();
    oss << " ntaps: " << m_taps.size() << std::endl;
    oss << "\ttaps: ";
    std::copy(m_taps.begin(), m_taps.end()-1, std::ostream_iterator<double>(oss, ","));
    oss << m_taps.back() << std::endl;
    oss << "\tSamples: ";
    std::copy(m_sr.begin(), m_sr.end()-1, std::ostream_iterator<double>(oss, ","));
    oss << m_sr.back();

    return oss.str();
}
