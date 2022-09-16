//includes on top

#include "RatioFinder.hpp"

#include <cmath>

//------------FUNCTIONS------------

//GETTERS AND SETTERS
void RatioFinder::setStartFrequency(float freq){
    startFrequency = freq;
}
float RatioFinder::getStartFrequency() const {
    return startFrequency;
}
void RatioFinder::setMinFrequency(float freq){
    minFrequency = freq;
}
float RatioFinder::getMinFrequency() const {
    return minFrequency;
}
void RatioFinder::setMaxFrequency(float freq){
    maxFrequency = freq;
}
float RatioFinder::getMaxFrequency() const {
    return maxFrequency;
}
std::vector<float> RatioFinder::getFreqTable() {
    return freqTable;
}

//CONSTRUCTOR
/**
 * @brief Create a Frequency Table of the equal tempered scale between MIN e MAX FREQUENCY
 */

RatioFinder::RatioFinder(){
    float fZero = (*this).getStartFrequency();

    //calculate extremes to generate the frequency table
    int nMin = (int) floorf(12*std::log2((*this).getMinFrequency()/fZero));
    int nMax = (int) ceilf(12*std::log2((*this).getMaxFrequency()/fZero));

    //table initialization
    freqTable = std::vector<float>((size_t) abs(nMax - nMin) + 1);

    //fill the tables with the frequencies calculated from nMin to nMax
    size_t i = 0;
    for (auto n = nMin; n <= nMax; n+=1){
        freqTable[i++] = fZero * powf(2,(float) n/12);
    }
    std::cout << "RatioFinder ready" << std::endl;
}

// A recursive binary search function
int binarySearch(float arr[], int l, int r, float x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the frequency is present at the middle itself
        if (arr[mid] == x)
            return mid;
 
        // If frequency is smaller than mid, then it can only be present in left subarray
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);
 
        // Else the element can only be present in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    if(arr[l] - x > arr[r] - x)
        return r;
    else
        return l;
}

float RatioFinder::findNearestNoteFrequency(float noteFrequency){
    //binarySearch ritorna l'indice del risultato, io voglio tornare la frequenza
    return freqTable[static_cast<unsigned long>(binarySearch(
            freqTable.data(),
            0,
            (int) freqTable.size() - 1,
            noteFrequency))
            ];
}

/**
 * @brief Get the pitching ratio between the fundamental frequency and the nearest note's frequency
 *
 * @param inputTime Time samples vector
 * @param sampleRate Sample rate to read the vector
 */
float RatioFinder::getRatio(std::vector<float> freq, double sampleRate){
    //calculate the base 2 exponent of the inputTime size to initialize dsp::FFT object
    int order = (int) ceilf(log2f( (float) freq.size()));

    //initialize FFT object
    auto fft = juce::dsp::FFT(order);
    //std::cout << "Fft created with order " << order << " with input size " << freq.size() << std::endl;

    //executing fft
    freq.resize(pow(2, order + 1));
    //std::cout << "Resized at " << freq.size() << std::endl;

    fft.performFrequencyOnlyForwardTransform(freq.data());

    //std::cout << "Fft performed" << std::endl;

    //min and max frequency set in .hpp
    int indexMinFrequency = (int) ceil( minFrequency * (float) freq.size()/sampleRate);
    //std::cout << "Min freq " << indexMinFrequency << std::endl;
    int indexMaxFrequency = (int) floor(maxFrequency * (float) freq.size()/sampleRate);
    //std::cout << "Max freq " << indexMaxFrequency << std::endl;

    //find fundamental
    int fundamentalFrequencyIndex = 0;
    float fundamentalFrequencyMagnitude = 0;
    for ( int i = indexMinFrequency; i < indexMaxFrequency; i++ ){
        if(freq[i] > fundamentalFrequencyMagnitude ) {
            fundamentalFrequencyMagnitude = freq[i];
            fundamentalFrequencyIndex = i;
        }
    }
    if (fundamentalFrequencyIndex == 0)
        return 1;
    float fundamentalFrequency = roundf(( (float) fundamentalFrequencyIndex - 1) * (float) sampleRate / (float) freq.size());
    //std::cout << "Fun mag " << fundamentalFrequencyMagnitude << " Fun index " << fundamentalFrequencyIndex << " Fun freq " << fundamentalFrequency << std::endl;

    //find the nearest note
    float nearestNoteFrequency = findNearestNoteFrequency(fundamentalFrequency);
    //std::cout << "Near freq " << nearestNoteFrequency << std::endl;

    if(fundamentalFrequency > 0 && nearestNoteFrequency != 0)
        return nearestNoteFrequency/fundamentalFrequency;
    
    return 1;
}