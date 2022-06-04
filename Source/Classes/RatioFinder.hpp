#ifndef RATIO_FINDER_H
#define RATIO_FINDER_H

#include <vector>
#include <juce_dsp/juce_dsp.h>


class RatioFinder {
    private:
        //pricate variables
        float peak_f;
        float near_f;
        //private methods
        //std::vector<float> FFT(std::vector<float> buffer_in_time);
        
        void nearestNote();

        int freqToIndx(const float* f);
        float indxToFreq(const int* i);

    public: 
        //constructor
        RatioFinder();

        //public variables

        //public methods
        float getRatio(const float* buffer_channel_pointer);
};

#endif //RATIO_FINDER_H