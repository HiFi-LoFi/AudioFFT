AudioFFT
========

AudioFFT provides real-to-complex/complex-to-real FFT routines.

## Features ##

- Real-complex FFT and complex-real inverse FFT for power-of-2-sized real data.

- Uniform interface to different FFT implementations (currently Ooura, FFTW3, Apple Accelerate and Intel IPP).

- Complex data is handled in "split-complex" format, i.e. there are separate
  arrays for the real and imaginary parts which can be useful for SIMD optimizations
  (split-complex arrays have to be of length (size/2+1) representing bins from DC
  to Nyquist frequency).

- Output is "ready to use" (all scaling etc. is already handled internally).

- No allocations/deallocations after the initialization which makes it usable
  for real-time audio applications (that's what I wrote it for and using it).


## How to use it in your project ##

- Add the .h and .cpp file to your project - that's all.


- To get extra speed, you can link FFTW3 to your project and define
  AUDIOFFT_FFTW3 (however, please check whether your project suits the
  according license).

- To get the best speed on Apple platforms, you can link the Apple
  Accelerate framework to your project and define AUDIOFFT_APPLE_ACCELERATE.

- On any supported platform you can use Intel IPP's FFT's by linking to IPP and defining AUDIOFFT_INTEL_IPP.

## Remarks ##

- AudioFFT is not intended to be the fastest FFT, but to be a fast-enough
  FFT suitable for most audio applications.
  
- AudioFFT uses the quite liberal MIT license.


## Example usage ##

    #include "AudioFFT.h"
    
    void Example()
    {
      const size_t fftSize = 1024; // Needs to be power of 2!
      
      std::vector<float> input(fftSize, 0.0f);
      std::vector<float> re(fftaudio::AudioFFT::ComplexSize(fftSize)); 
      std::vector<float> im(fftaudio::AudioFFT::ComplexSize(fftSize)); 
      std::vector<float> output(fftSize);
      
      audiofft::AudioFFT fft;
      fft.init(1024);
      fft.fft(input.data(), re.data(), im.data());
      fft.ifft(output.data(), re.data(), im.data());
    }


## Benchmarks ##

The following tables show time measurements for forward/backward "FFTing" 512MB
of real data using the FFT input size as listed in the tables.

AudioFFT was run using internally Ooura, FFTW3 and vDSP (Apple Accelerate).

For comparison and reference, the same setup also was used with the great
KissFFT, which is a quite popular FFT implementation, and which is also able
to handle non-power-of-2 sizes. :-)


#### CPU: Intel Core i5 (2,4 GHz) ####
- Mac OS X Lion 10.7.5
- Compiler: Apple LLVM 3.0 (/Os (fastest, smallest), SSE enabled) 

| Size |  Ooura  |  FFTW3  |  Apple  | KissFFT |
| ----:| -------:| -------:| -------:| -------:|
|   64 |  8.805s |  6.914s |  3.420s | 12.496s |
|  128 | 10.047s |  6.473s |  2.992s | 11.457s |
|  512 | 11.895s |  6.473s |  3.025s | 13.737s |
| 1024 | 12.956s |  6.932s |  3.139s | 17.050s |
| 4096 | 14.840s |  7.517s |  3.661s | 19.379s |


#### CPU: Intel Xeon (2.93 GHz) ####
- Windows 7
- Compiler: VC10 (/O2 /arch:SSE2 /fp:precise)

| Size |  Ooura  |  FFTW3  | KissFFT |
| ----:| -------:| -------:| -------:|
|   64 |  7.267s |  4.625s | 20.819s |
|  128 |  7.583s |  5.494s | 20.822s |
|  512 |  8.608s |  5.346s | 24.812s |
| 1024 |  9.546s |  5.604s | 28.936s |
| 4096 | 11.026s |  6.265s | 33.160s |
