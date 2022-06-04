//includes on top
#include "RatioFinder.hpp"


//constructor
RatioFinder::RatioFinder(){}

//implementation of functions in autotune.m
float RatioFinder::getRatio(const float* buffer_channel_pointer){
    float* mag_buffer = const_cast<float*> (buffer_channel_pointer);
    
    //computing FFT (not sure about type conversion and compatibility)
    //std::vector<float> buffer_in_frequency  = FFT( (std::vector<float>)* buffer_channel_pointer);
    juce::dsp::FFT FFT(10);
    FFT.performFrequencyOnlyForwardTransform(mag_buffer);

    return near_f/peak_f;
}

//FFT
//std::vector<float> RatioFinder::FFT(std::vector<float> buffer_in_time){}

//find nearest note
void RatioFinder::nearestNote(){
    near_f = peak_f;
}

//index to frequency and viceversa converters
//int RatioFinder::freqToIndx(const float* f){}
//float RatioFinder::indxToFreq(const int* i){}
