#ifndef RATIO_FINDER_H
#define RATIO_FINDER_H

#include "../PluginProcessor.h"

class RatioFinder {
    private:
        //pricate variables
        
        //private methods

    public: 
        //constructor
        RatioFinder();

        //public variables

        //public methods
        void getRatio(juce::AudioBuffer<float> inputTime);
};

#endif //RATIO_FINDER_H