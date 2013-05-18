// ==================================================================================
// Copyright (c) 2013 HiFi-LoFi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ==================================================================================

#include <vector>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "../AudioFFT.h"


#define TEST_CORRECTNESS
//#define TEST_PERFORMANCE
//#define TEST_PERFORMANCE_KISSFFT


template<typename TA, typename TB>
static bool CheckBuffers(size_t size, const TA a, const TB b, double tolerance = 0.001)
{
  size_t numDiff = 0;
  for (size_t i=0; i<size; ++i)
  {
    const double aa = static_cast<double>(a[i]);
    const double bb = static_cast<double>(b[i]);
    const double diff = ::fabs(aa-bb);
    if (diff > tolerance)
    {
      ++numDiff;
    }
  }
  return (numDiff == 0);
}


static void TestCorrectness(size_t inputSize, const double* refRe, const double* refIm)
{
  bool success = true;

  std::vector<float> input(inputSize);
  for (size_t i=0; i<inputSize; ++i)
  {
    input[i] = static_cast<float>(i+1);
  }
  
  audiofft::AudioFFT fft;
  fft.init(inputSize);
  
  const size_t complexSize = audiofft::AudioFFT::ComplexSize(inputSize);
  std::vector<float> re(complexSize);
  std::vector<float> im(complexSize);
  fft.fft(&input[0], &re[0], &im[0]);
  success &= (CheckBuffers(complexSize, re, refRe) == true);
  success &= (CheckBuffers(complexSize, im, refIm) == true);
  
  std::vector<float> backward(inputSize, -10000.0f);
  fft.ifft(&backward[0], &re[0], &im[0]);
  success &= (CheckBuffers(inputSize, backward, input) == true);

  printf("Correctness (input size %d) => %s\n", static_cast<int>(inputSize), success ? "[OK]" : "[FAILED]");
}


static void TestCorrectness()
{
  {
    const double Re2[] = { 3.0000000,-1.0000000 };
    const double Im2[] = { 0.00000000,0.00000000 };
    TestCorrectness(2, Re2, Im2);
  }
  {
    const double Re4[] = { 10.000000,-2.0000000,-2.0000000 };
    const double Im4[] = { 0.00000000,2.0000000,0.00000000 };
    TestCorrectness(4, Re4, Im4);
  }
  {
    const double Re8[] = { 36.000000,-4.0000000,-4.0000000,-4.0000000,-4.0000000 };
    const double Im8[] = { 0.00000000,9.6568546,4.0000000,1.6568543,0.00000000 };
    TestCorrectness(8, Re8, Im8);
  }
  {
    const double Re16[] = { 136.00000,-8.0000000,-8.0000000,-8.0000000,-8.0000000,-8.0000000,-8.0000000,-8.0000000,-8.0000000 };
    const double Im16[] = { 0.00000000,40.218716,19.313709,11.972846,8.0000000,5.3454289,3.3137085,1.5912989,0.00000000 };
    TestCorrectness(16, Re16, Im16);
  }
  {
    const double Re32[] = { 528.00000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,
                            -16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000,-16.000000 };
    const double Im32[] = { 0.00000000,162.45073,80.437431,52.744930,38.627419,29.933895,23.945692,19.496056,16.000000,13.130860,
                            10.690858,8.5521784,6.6274171,4.8535471,3.1825979,1.5758624,0.00000000 };
    TestCorrectness(32, Re32, Im32);
  }
  {
    const double Re64[] = { 2080.0000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,
                            -32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,
                            -32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000,
                            -32.000000,-32.000000,-32.000000,-32.000000,-32.000000,-32.000000 };
    const double Im64[] = { 0.00000000,651.37494,324.90146,215.72647,160.87486,127.75116,105.48986,89.434006,77.254837,67.658318,
                            59.867790,53.388775,47.891384,43.147007,38.992111,35.306561,32.000000,29.003109,26.261721,23.732817,
                            21.381716,19.180061,17.104357,15.134872,13.254834,11.449783,9.7070942,8.0155830,6.3651958,4.7467518,
                            3.1517248,1.5720592,0.00000000 };
    TestCorrectness(64, Re64, Im64);
  }
  {
    const double Re128[] = { 8256.0000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,-64.000000,
                             -64.000000,-64.000000 };
    const double Im128[] = { 0.00000000,2607.0710,1302.7499,867.62683,649.80292,518.89832,431.45294,368.84109,321.74973,285.00494,
                             255.50232,231.26628,210.97972,193.73076,178.86801,165.91376,154.50967,144.38168,135.31664,127.14616,
                             119.73558,112.97580,106.77755,101.06705,95.782768,90.873016,86.294014,82.008423,77.984222,74.193787,
                             70.613121,67.221306,64.000000,60.933064,58.006218,55.206779,52.523441,49.946091,47.465633,45.073887,
                             42.763432,40.527554,38.360123,36.255550,34.208714,32.214893,30.269745,28.369249,26.509668,24.687525,
                             22.899567,21.142744,19.414188,17.711185,16.031166,14.371680,12.730392,11.105054,9.4935036,7.8936472,
                             6.3034496,4.7209234,3.1441183,1.5711118,0.00000000 };
    TestCorrectness(128, Re128, Im128);
  }
  {
    const double Re256[] = { 32896.000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,-128.00000,
                             -128.00000,-128.00000,-128.00000 };
    const double Im256[] = { 0.00000000,10429.854,5214.1421,3475.2219,2605.4998,2083.4570,1735.2537,1486.3871,1299.6058,1154.2147,
                             1037.7966,942.44965,862.90588,795.51843,737.68219,687.48676,643.49945,604.62457,570.00989,538.98267,
                             511.00464,485.64011,462.53256,441.38748,421.95944,404.04230,387.46152,372.06857,357.73602,344.35406,
                             331.82751,320.07346,309.01935,298.60141,288.76337,279.45544,270.63327,262.25735,254.29233,246.70645,
                             239.47116,232.56065,225.95160,219.62283,213.55510,207.73085,202.13409,196.75014,191.56554,186.56796,
                             181.74603,177.08928,172.58803,168.23331,164.01685,159.93094,155.96844,152.12273,148.38757,144.75720,
                             141.22624,137.78961,134.44261,131.18079,128.00000,124.89634,121.86613,118.90591,116.01244,113.18262,
                             110.41356,107.70251,105.04688,102.44421,99.892181,97.388565,94.931267,92.518303,90.147774,87.817863,
                             85.526863,83.273132,81.055107,78.871284,76.720245,74.600624,72.511101,70.450439,68.417427,66.410912,
                             64.429787,62.472988,60.539490,58.628311,56.738499,54.869133,53.019337,51.188248,49.375050,47.578934,
                             45.799133,44.034893,42.285488,40.550213,38.828377,37.119312,35.422371,33.736916,32.062332,30.398010,
                             28.743361,27.097807,25.460783,23.831732,22.210108,20.595375,18.987007,17.384483,15.787294,14.194933,
                             12.606899,11.022701,9.4418468,7.8638530,6.2882366,4.7145190,3.1422236,1.5708752,0.00000000 };
    TestCorrectness(256, Re256, Im256);
  }
}


static void TestPerformance(const size_t inputSize)
{
  const size_t overallSize = size_t(512) * size_t(1024) * size_t(1024);
  const size_t iterations = overallSize / inputSize;

  printf("Performance (processing %d MB, input size %d)", static_cast<int>(overallSize / (1024 * 1024)), static_cast<int>(inputSize));

  std::vector<float> input(inputSize);
  for (size_t i=0; i<inputSize; ++i)
  {
    input[i] = static_cast<float>(i);
  }

  const size_t complexSize = audiofft::AudioFFT::ComplexSize(inputSize);
  std::vector<float> re(complexSize);
  std::vector<float> im(complexSize);

  std::vector<float> backward(inputSize);

  {
    audiofft::AudioFFT fft;
    fft.init(inputSize);
    for (size_t i=0; i<iterations; ++i)
    {
      fft.fft(&input[0], &re[0], &im[0]);
      fft.ifft(&backward[0], &re[0], &im[0]);
    }
  }

  printf(" => Completed!\n");
}


#ifdef TEST_PERFORMANCE_KISSFFT

#include "kiss_fftr.h"

static void TestPerformanceKissFFT(const size_t inputSize)
{
  const size_t overallSize = size_t(512) * size_t(1024) * size_t(1024);
  const size_t iterations = overallSize / inputSize;

  printf("Performance KissFFT (processing %d MB, input size %d)", static_cast<int>(overallSize / (1024 * 1024)), static_cast<int>(inputSize));
  
  std::vector<kiss_fft_scalar> input(inputSize);
  for (size_t i=0; i<inputSize; ++i)
  {
    input[i] = static_cast<kiss_fft_scalar>(i);
  }
  
  std::vector<kiss_fft_cpx> complex(inputSize/2 + 1);
  
  std::vector<kiss_fft_scalar> backward(inputSize);
  
  kiss_fftr_cfg configForward = kiss_fftr_alloc(static_cast<unsigned>(inputSize), 0, 0, 0);
  kiss_fftr_cfg configBackward = kiss_fftr_alloc(static_cast<unsigned>(inputSize), 1, 0, 0);

  for (size_t i=0; i<iterations; ++i)
  {
    kiss_fftr(configForward, &input[0], &complex[0]);
    kiss_fftri(configBackward, &complex[0], &backward[0]);
  }
  
  kiss_fftr_free(configForward);
  kiss_fftr_free(configBackward);
  
  printf(" => Completed!\n");
}

#endif // TEST_PERFORMANCE_KISSFFT


int main()
{ 
#ifdef TEST_CORRECTNESS
  TestCorrectness();
#endif
  
#ifdef TEST_PERFORMANCE
  TestPerformance(64);
  TestPerformance(128);
  TestPerformance(512);
  TestPerformance(1024);
  TestPerformance(4096);
#endif
  
#ifdef TEST_PERFORMANCE_KISSFFT
  TestPerformanceKissFFT(64);
  TestPerformanceKissFFT(128);
  TestPerformanceKissFFT(512);
  TestPerformanceKissFFT(1024);
  TestPerformanceKissFFT(4096);
#endif
  
  return 0;
}
