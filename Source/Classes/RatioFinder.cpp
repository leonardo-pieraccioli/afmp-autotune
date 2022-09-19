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
    std::cout << "Freq table" << std::endl;
    for (auto n = nMin; n <= nMax; n+=1){
        freqTable[i++] = fZero * powf(2,(float) n/12);
        std::cout << "\tFreq " << i << " " << freqTable[i - 1] << std::endl;
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

    std::cout << "\tInterval found\n\t\tArr[r]: " << arr[r] << "\n\t\tArr[l]: " << arr[l] << "\n\t\tFreq x: " << x << "\n\t\t" << x / arr[r] << " >? " << powf(2, (float) 1/ (float) 24)<< std::endl;
    if(x / arr[r] > powf(2, (float) 1/ (float) 24)) //al posto di 1/24 arr[r]/(arr[l]*2)
        return l;
    return r;
    /*
    if(arr[l] - x > arr[r] - x)
        return r;
    return l;*/
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

void toComplex(const std::vector<float>& toConvert, std::vector<std::complex<float>>& converted)
{
    int size = toConvert.size();
    converted.clear();
    for (int i = 0; i < size; ++i) {
        converted[i].real(toConvert[i]);
        converted[i].imag(0);
    }
}

/**
 * @brief Get the pitching ratio between the fundamental frequency and the nearest note's frequency
 *
 * @param inputTime Time samples vector
 * @param sampleRate Sample rate to read the vector
 */
float RatioFinder::getRatio(std::vector<float> input, double sampleRate){
    //calculate the base 2 exponent of the inputTime size to initialize dsp::FFT object
    //std::cout << "\tFreq size: " << freq.size() << std::endl;
    int order = (int) ceilf(log2f( (float) input.size()));

    //initialize FFT object
    auto fft = juce::dsp::FFT(order);
    std::cout << "\tFft created with order " << order << "\n\twith input size " << input.size() << std::endl;
    std::cout << "\tFft size: " << fft.getSize() << std::endl;
    //executing fft
    input.resize(fft.getSize());
    std::cout << "\tResized at " << input.size() << std::endl;

    // CALCOLARE FFT CON PERFORM E POI CALCOLARE IL MODULO
    auto inputFFT = std::vector<std::complex<float>>(input.size());
    auto outputFFT = std::vector<std::complex<float>>(input.size());
    toComplex(input, inputFFT);
    std::cout << "\tConversion to complex completed\n";

    //Seg fault
    fft.perform(inputFFT.data(), outputFFT.data(), false);
    std::cout << "\tFft size after perform " << fft.getSize() << " \n\tSize after perform " << outputFFT.size() << std::endl;

    //min and max frequency set in .hpp
    int indexMinFrequency = (int) ceil( minFrequency * (float) fft.getSize()/(sampleRate));
    std::cout << "\tMin freq index to check " << indexMinFrequency;
    int indexMaxFrequency = (int) floor(maxFrequency * (float) fft.getSize()/(sampleRate));
    std::cout << "\n\tMax frequency index to check " << indexMaxFrequency << std::endl;

    //find fundamental
    int fundamentalFrequencyIndex = 0;
    float fundamentalFrequencyMagnitude = 0;
    for ( int i = indexMinFrequency; i < indexMaxFrequency; i++ ){
        if(std::abs(outputFFT[i]) > fundamentalFrequencyMagnitude ) {
            fundamentalFrequencyMagnitude = std::abs(outputFFT[i]);
            fundamentalFrequencyIndex = i;
        }
    }

    if (fundamentalFrequencyIndex == 0) {
        std::cout << "\tFund frequency equal to 0" << std::endl;
        return 1;
    }
    float fundamentalFrequency = roundf(( (float) fundamentalFrequencyIndex - 1) * (float) sampleRate / ((float) fft.getSize()));
    std::cout << "\tFund mag " << fundamentalFrequencyMagnitude << "\n\tFund index " << fundamentalFrequencyIndex << "\n\tFund freq " << fundamentalFrequency << std::endl;

    //find the nearest note
    float nearestNoteFrequency = findNearestNoteFrequency(fundamentalFrequency);
    std::cout << "\tNearest frequency " << nearestNoteFrequency << std::endl;

    if(fundamentalFrequency > 0 && nearestNoteFrequency > 0) {
        std::cout << "\tRatio: " << nearestNoteFrequency / fundamentalFrequency << std::endl;
        return nearestNoteFrequency / fundamentalFrequency;
    }
    
    return 1;
}

