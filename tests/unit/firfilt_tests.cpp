#include "gtest/gtest.h"
#include "fir.hpp"
#include <exception>
#include <string>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux
#include <unistd.h>
#include <linux/limits.h>
#endif

class fir_ft : public ::testing::Test {};

TEST_F(fir_ft, test_doa_LPF)
{
    Filter_LP filter(11, 1000, 10);
}

TEST_F(fir_ft, test_doa_HPF)
{
    Filter_HP filter(11, 1000, 10);
}

TEST_F(fir_ft, test_doa_BPF)
{
    Filter_BP filter(11, 1000, 10, 100);
}

TEST_F(fir_ft, test_doa_filter_destructors)
{
    Filter* filter = new Filter(BPF, 11, 1000, 10, 100);
    delete filter;

    Filter_BP* bp = new Filter_BP(11, 1000, 10, 100);
}

TEST_F(fir_ft, test_throws)
{
    /* Fs must be > 0 */
    ASSERT_THROW(Filter_BP filter(11, 0, 10, 100), std::runtime_error);

    /* F0 must be > F1 */
    ASSERT_THROW(Filter_BP filter(11, 1000, 10, 10), std::runtime_error);

    /* F0 must be > 0 */
    ASSERT_THROW(Filter_BP filter(11, 1000, 0, 100), std::runtime_error);

    /* F0 must be > Fs */
    ASSERT_THROW(Filter_BP filter(11, 1000, 1000, 100), std::runtime_error);

    /* Invalid filter type (use a c-style cast since this will be invalid) */
    ASSERT_THROW(Filter filter((filterType)(-1), 11, 1000, 10, 100), std::runtime_error);
}

bool AreEqual(const std::string& filename0, const std::string& filename1)
{
    std::ifstream f1(filename0.c_str(),  std::ios::ate);
    std::ifstream f2(filename1.c_str(),  std::ios::ate);

    if (f1.fail() || f2.fail())
        return false;

    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(f2.rdbuf()));
}


std::string bp()
{
    std::string bp;
    char sep = '/';

#ifdef _WIN32
    char result[ MAX_PATH ];
    sep = '\\';
    bp = std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
}
#else
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    bp = std::string( result, (count > 0) ? count : 0 );
#endif

    size_t i = bp.rfind(sep, bp.length());
    if (i != std::string::npos)
        return(bp.substr(0, i+1));

    return "";
}

TEST_F(fir_ft, test_write_taps_to_file)
{
    Filter filter(LPF, 11, 1000, 10);
    filter.write_taps_to_file((bp() + "taps.txt").c_str());
    ASSERT_TRUE(AreEqual(bp() + "taps.txt", bp() + "taps.txt.golden"));
}

TEST_F(fir_ft, test_write_freqres_to_file)
{
    Filter filter(LPF, 11, 1000, 10);
    filter.write_freqres_to_file((bp() + "freqresp.txt").c_str());
    ASSERT_TRUE(AreEqual(bp() + "freqresp.txt", bp() + "freqresp.txt.golden"));
}

TEST_F(fir_ft, test_filter_str_method)
{
    Filter filter(LPF, 11, 1000, 10);
    std::ofstream ofs ((bp() + "lpf.txt").c_str());
    ofs << filter.__str__();
    ofs.close();

    ASSERT_TRUE(AreEqual(bp() + "lpf.txt", bp() + "lpf.txt.golden"));
}

TEST_F(fir_ft, test_filter_sin_wave)
{
    Filter filter(LPF, 51, 1000, 7);

    std::ifstream in((bp() + "Fs1000_1Hz_signal_20Hz_noise.txt").c_str());
    std::ofstream out ((bp() + "filtered_Fs1000_1Hz_signal_20Hz_noise.txt").c_str());
    ASSERT_FALSE(in.eof());

    out << std::fixed << std::setprecision(8);

    /* Filter the input sin wave with noise, write output to file */
    while(!in.eof())
    {
        double value;
        in >> value;
        out << filter.do_sample(value) << "\n";
    }

    out.close();

    /* Check the filtered data */
    ASSERT_TRUE(AreEqual(bp() + "filtered_Fs1000_1Hz_signal_20Hz_noise.txt",
                bp() + "filtered_Fs1000_1Hz_signal_20Hz_noise.txt.golden"));
}
