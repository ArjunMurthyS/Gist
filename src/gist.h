//=======================================================================
/** @file Gist.h
 *  @brief Includes all relevant parts of the 'Gist' audio analysis library
 *  @author Adam Stark
 *  @copyright Copyright (C) 2013  Adam Stark
 *
 * This file is part of the 'Gist' audio analysis library
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//=======================================================================

#ifndef __GISTHEADER__
#define __GISTHEADER__

//=======================================================================
// core
#include "core/CoreTimeDomainFeatures.h"
#include "core/CoreFrequencyDomainFeatures.h"

// onset detection functions
#include "onset-detection-functions/OnsetDetectionFunction.h"

// pitch detection
#include "pitch/Yin.h"

// MFCC
#include "mfcc/MFCC.h"

//=======================================================================
// fft
#ifdef USE_FFTW
#include "fftw3.h"
#endif

#ifdef USE_KISS_FFT
#include "kiss_fft.h"
#endif

#ifdef USE_ACCELERATE_FFT
#include "fft/AccelerateFFT.h"
#endif

//=======================================================================
/** Class for all performing all Gist audio analyses */
template <class T>
class Gist
{
public:
    
    //=======================================================================
    /** Constructor
     * @param frameSize_ the input audio frame size
     * @param sampleRate the input audio sample rate
     */
    Gist (int frameSize_, int sampleRate_);

    /** Destructor */
    ~Gist();

    /** Set the audio frame size.
     * @param frameSize_ the frame size to use
     */
    void setAudioFrameSize (int frameSize_);
    
    /** @Returns the audio frame size currently being used */
    int getAudioFrameSize();

    /** Process an audio frame
     * @param audioFrame a vector containing audio samples
     */
    void processAudioFrame (std::vector<T> audioFrame_);

    /** Process an audio frame
     * @param buffer a pointer to an array containing the audio samples
     * @param numSamples the number of samples in the audio frame
     */
    void processAudioFrame (T* buffer, unsigned long numSamples);

    /** Gist automatically calculates the magnitude spectrum when processAudioFrame() is called, this function returns it.
     @returns the current magnitude spectrum */
    std::vector<T> getMagnitudeSpectrum();

    //================= CORE TIME DOMAIN FEATURES =================

    /** @Returns the root mean square (RMS) of the currently stored audio frame */
    T rootMeanSquare();

    /** @Returns the peak energy of the currently stored audio frame */
    T peakEnergy();

    /** @Returns the zero crossing rate of the currently stored audio frame */
    T zeroCrossingRate();

    //=============== CORE FREQUENCY DOMAIN FEATURES ==============

    /** @Returns the spectral centroid from the magnitude spectrum */
    T spectralCentroid();

    /** @Returns the spectral crest */
    T spectralCrest();

    /** @Returns the spectral flatness of the magnitude spectrum */
    T spectralFlatness();
    
    /** @Returns the spectral rolloff of the magnitude spectrum */
    T spectralRolloff();
    
    /** @Returns the spectral kurtosis of the magnitude spectrum */
    T spectralKurtosis();

    //================= ONSET DETECTION FUNCTIONS =================

    /** @Returns the energy difference onset detection function sample for the magnitude spectrum frame */
    T energyDifference();

    /** @Returns the spectral difference onset detection function sample for the magnitude spectrum frame */
    T spectralDifference();

    /** @Returns the half wave rectified complex spectral difference onset detection function sample for the magnitude spectrum frame */
    T spectralDifferenceHWR();

    /** @Returns the complex spectral difference onset detection function sample for the magnitude spectrum frame */
    T complexSpectralDifference();

    /** @Returns the high frequency content onset detection function sample for the magnitude spectrum frame */
    T highFrequencyContent();

    //=========================== PITCH ============================

    /** @Returns a monophonic pitch estimate according to the Yin algorithm */
    T pitch();

    //=========================== MFCCs =============================

    /** @Returns the Mel Frequency Spectrum */
    std::vector<T> melFrequencySpectrum();

    /** @Returns the Mel Frequency Cepstral Coefficients as a vector */
    std::vector<T> melFrequencyCepstralCoefficients();

private:
    //=======================================================================

    /** Configure the FFT implementation given the audio frame size) */
    void configureFFT();

    /** Free all FFT-related data */
    void freeFFT();

    /** perform the FFT on the current audio frame */
    void performFFT();

    //=======================================================================

#ifdef USE_FFTW
    fftw_plan p;          /**< fftw plan */
    fftw_complex* fftIn;  /**< to hold complex fft values for input */
    fftw_complex* fftOut; /**< to hold complex fft values for output */
#endif

#ifdef USE_KISS_FFT
    kiss_fft_cfg cfg;     /**< Kiss FFT configuration */
    kiss_fft_cpx* fftIn;  /**< FFT input samples, in complex form */
    kiss_fft_cpx* fftOut; /**< FFT output samples, in complex form */
#endif
    
#ifdef USE_ACCELERATE_FFT
    AccelerateFFT<T> accelerateFFT;
#endif

    int frameSize; /**< The audio frame size */

    std::vector<T> audioFrame;        /**< The current audio frame */
    std::vector<T> fftReal;           /**< The real part of the FFT for the current audio frame */
    std::vector<T> fftImag;           /**< The imaginary part of the FFT for the current audio frame */
    std::vector<T> magnitudeSpectrum; /**< The magnitude spectrum of the current audio frame */

    bool fftConfigured;

    /** object to compute core time domain features */
    CoreTimeDomainFeatures<T> coreTimeDomainFeatures;

    /** object to compute core frequency domain features */
    CoreFrequencyDomainFeatures<T> coreFrequencyDomainFeatures;

    /** object to compute onset detection functions */
    OnsetDetectionFunction<T> onsetDetectionFunction;

    /** object to compute pitch estimates via the Yin algorithm */
    Yin<T> yin;

    /** object to compute MFCCs and mel-frequency specta */
    MFCC<T> mfcc;
};

#endif
