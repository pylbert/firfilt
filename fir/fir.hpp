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

#pragma once

#define MAX_NUM_FILTER_TAPS 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <string>
#include <vector>

/**
 * Enumerated filter types
 */
enum filterType
{
    /** Low-pass filter */
    LPF,
    /** High-pass filter */
    HPF,
    /** Band-pass filter */
    BPF
};

/**
 * PURPOSE:
 * This object designs digital filters and filters digital data streams
 *
 * USAGE:
 * Invoke an object of type {Filter, Filter_LP, Filter_HP}.
 * The arguments to the constructors are as follows:
 *
 *         // For LPF or HPF, F0 = cutoff frequency
 *         Filter(filterType filt_t, int num_taps, double Fs, double F0);
 *
 *         // For BPF, F0 = low cutoff frequency, F1 = high cutoff frequency
 *         Filter(filterType filt_t, int num_taps, double Fs, double F0, double F1);
 *
 * filt_t: is LPF, HPF or BPF
 * num_taps: is the number of taps you want the filter to use
 * Fs: is the sampling frequency of the digital data being filtered
 * F0: is the "transition" frequency for LPF and HPF filters
 * F0, F1: are the upper and lower transition frequencies for BPF filters
 *
 * Once the filter is created, you can start filtering data.  Here
 * is an example for 51 tap lowpass filtering of an audio stream sampled at
 * 44.1Khz (the CD sampling rate), where the goal is to create a signal
 * of "telephone" bandwidth (4Khz):
 *
 * Filter my_filter(LPF, 51, 44.1, 4.0)
 *
 * while(data_to_be_filtered){
 *
 *     next_sample = // Get the next sample from the data stream somehow
 *
 *     filtered_sample = my_filter.do_sample( next_sample );
 *     .
 *     .
 *     .
 * }
 *
 * Several helper functions are provided:
 *     init(): The filter can be re-initialized with a call to this function
 *     get_taps(double *taps): returns the filter taps in the array "taps"
 *     write_taps_to_file(char *filename): writes the filter taps to a file
 *     write_freqres_to_file(char *filename): output frequency response to a file
 *
 * The Filter class can throw std::runtime_error from its constructor for
 * the following constraints:
 *      Fs <= 0
 *      F0 <= 0 or F0 >= Fs/2
 *      num_taps <= 0 or num_taps >= MAX_NUM_FILTER_TAPS
 *      Fs <= 0 (BPF case)
 *      F0 >= F1
 *      F0 <= 0 || F0 >= Fs/2
 *      F1 <= 0 || F1 >= Fs/2
 *      num_taps <= 0 or num_taps >= MAX_NUM_FILTER_TAPS (BPF case)
 *
 * The filters are designed using the "Fourier Series Method".  This
 * means that the coefficients of a Fourier Series approximation to the
 * frequency response of an ideal filter (LPF, HPF, BPF) are used as
 * the filter taps.  The resulting filters have some ripple in the passband
 * due to the Gibbs phenomenon; the filters are linear phase.
 */
class Filter
{
    public:
        /*
         * Construct a Filter object.
         * @param filt_t Filter type
         * @param num_taps Target number of filter taps
         * @param Fs Data sampling frequency
         * @param F0 Low cutoff frequency for LPF/BPF, high cutoff frequency for HPF
         * @param F1 High cutoff frequency for BPF, otherwise 0.0
         */
        Filter(filterType filt_t, int num_taps, double Fs, double F0, double F1 = 0.0);

        /**
         * Default destructor
         */
        virtual ~Filter() {}

        /**
         * Filter gain coefficient calculated as the sum of all filter taps
         * @return Filter gain
         */
        double gain();

        /**
         * Process one data sample
         * @return Filtered data sample
         */
        double do_sample(double data_sample);

        /**
         * Provide a method to get Filter taps
         * @return Vector of Filter taps
         */
        const std::vector<double>& get_taps();

        /**
         * Write taps to file
         * @filename Target filename
         */
        void write_taps_to_file(const char* filename );

        /**
         * Write frequency response coefficients to file
         * @filename Target filename
         */
        void write_freqres_to_file(const char* filename );

        /**
         * Provide a description of this Filter object
         * @return String representation of this Filter
         */
        std::string __str__();

    private:
        filterType m_filt_t;
        int m_num_taps;
        double m_Fs;
        double m_F0;

        /* m_F1 and M_phi are only needed for the bandpass filter case  */
        double m_F1;
        double m_phi;

        double m_lambda;
        std::vector<double> m_taps;
        std::vector<double> m_sr;
        void designLPF();
        void designHPF();
        void designBPF();
};

/**
 * Helper class for low-pass filter
 */
class Filter_LP : public Filter
{
    public:
        /*
         * Construct a low-pass Filter object.
         * @param num_taps Target number of filter taps
         * @param Fs Data sampling frequency
         * @param F0 Low cutoff frequency
         */
        Filter_LP(int num_taps, double Fs, double F0) : Filter(LPF, num_taps, Fs, F0) {}
};

/**
 * Helper class for high-pass filter
 */
class Filter_HP : public Filter
{
    public:
        /*
         * Construct a high-pass Filter object.
         * @param num_taps Target number of filter taps
         * @param Fs Data sampling frequency
         * @param F0 High cutoff frequency
         */
        Filter_HP(int num_taps, double Fs, double F0) : Filter(HPF, num_taps, Fs, F0) {}
};
