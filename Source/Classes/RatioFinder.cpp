//includes on top

#include "RatioFinder.hpp"

#include <cmath>

//------------FUNCTIONS------------

//GETTERS AND SETTERS
void RatioFinder::setStartFrequency(float freq){
    startFrequency = freq;
}
float RatioFinder::getStartFrequency(){
    return startFrequency;
}
void RatioFinder::setMinFrequency(float freq){
    minFrequency = freq;
}
float RatioFinder::getMinFrequency(){
    return minFrequency;
}
void RatioFinder::setMaxFrequency(float freq){
    maxFrequency = freq;
}
float RatioFinder::getMaxFrequency(){
    return maxFrequency;
}

//CONSTRUCTOR
/**
 * @brief Create a Frequency Table of the equal tempered scale between MIN e MAX FREQUENCY
 */
RatioFinder::RatioFinder(){
    //save the start frequency because otherwise I call getStartFrequency a lot of times
    float fZero = (*this).getStartFrequency();

    //calculate extremes to generate the frequency table
    int nMin = floor(12*std::log2((*this).getMinFrequency()/fZero));
    int nMax = ceil(12*std::log2((*this).getMaxFrequency()/fZero));

    //table initialization
    freqTable = std::vector<float>(static_cast<unsigned long> (abs(nMax - nMin) + 1));

    //fill the tables with the frequencies calculated from nMin to nMax
    int i = 0;
    for (auto n = nMin; n <= nMax; n+=1){
        freqTable[i++] = fZero * pow(2,n/12);
    }
}

// A recursive binary search function
float binarySearch(float arr[], int l, int r, float x)
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

    /**
     * Condizione nel quale la frequenza che cerco si trova in mezzo ad altre due.
     * Devo calcolare se è nel quarto di semitono più vicino a quella più alta o quella più bassa e restituire la nearest frequency
     */
    return -1;
}

float RatioFinder::findNearestNoteFrequency(float noteFrequency){
    return freqTable[static_cast<unsigned long>(
            binarySearch(
                freqTable.data(),
                0,
                (int) freqTable.size() - 1,
                noteFrequency)
                )
            ];
}

//OTHERS
/**
 * @brief Get the pitching ratio between the fundamental frequency and the nearest note's frequency
 * 
 * @param inputTime Time samples vector 
 * @param sampleRate Sample rate to read the vector
 */
float RatioFinder::getRatio(std::vector<float> inputTime, double sampleRate){
    //calculate the base 2 exponent of the inputTime size to initialize dsp::FFT object  
    int order = floor(log2((double) inputTime.size()));

    //initialize FFT object
    auto fft = juce::dsp::FFT(order);

    //executing fft
    std::vector<float> inputFrequency;
    inputFrequency.insert(inputFrequency.begin(), inputTime.begin(), inputTime.begin() + pow(2,order) - 1);
    fft.performFrequencyOnlyForwardTransform(inputFrequency.data());
    
    //find fundamental between min and max frequency set in .hpp. I can calculate the index of the frequencies with the sampleRate.
    float fundamentalFrequency = *std::max_element(
            inputFrequency.begin() + ceil(minFrequency/sampleRate),
            inputFrequency.end() + floor(maxFrequency/sampleRate)
            );

    //find the nearest note
    //float nearestNoteFrequency = findNearestNoteFrequency(fundamentalFrequency);

    //fft destructor 
    fft.~FFT();

    return fundamentalFrequency;//nearestNoteFrequency;
}


/**
 * @brief Create a Frequency Table of the equal tempered scale between MIN e MAX FREQUENCY
 */
void RatioFinder::createFreqTable(){
    //save the start frequency because otherwise i call getStartFrequency a lot of times
    float fZero = (*this).getStartFrequency(); 

    //calculate extremes to generate the frequency table
    int nMin = floor(12*log2((*this).getMinFrequency()/fZero));
    int nMax = ceil(12*log2((*this).getMaxFrequency()/fZero));

    //table initializaztion
    freqTable = std::vector<float>(abs(nMax-nMin) + 1);

    //fill the tables with the frequencies calculated from nMin to nMax
    int i = 0;
    for (int n = nMin; n <= nMax; n++){
        freqTable[n] = fZero * pow(2,n/12);
    }
}
