#ifndef RATIO_FINDER_H
#define RATIO_FINDER_H

#include <juce_dsp/juce_dsp.h>
#include <cmath>

/**
 * @brief Contains functions to calculate the stretching ratio
 */
class RatioFinder {
    private:
        //------------PRIVATE VARIABLES------------
        float maxFrequency = 1050;   
        float minFrequency = 60;
        float startFrequency = 440;
        std::vector<float> freqTable;

        //------------PRIVATE METHODS------------
        float findNearestNoteFrequency(float noteFrequency);

    public:
        //GETTERS AND SETTERS
        //I added setters in case we want to make the user choose parameters from the GUI
        void setStartFrequency(float freq);
        float getStartFrequency();
        void setMinFrequency(float freq);
        float getMinFrequency();
        void setMaxFrequency(float freq);
        float getMaxFrequency();

        //------------PUBLIC VARIABLES------------

        //------------PUBLIC METHODS------------
        void getRatio(std::vector<float> input, double sampleRate);
        void createFreqTable();
        
};

#endif //RATIO_FINDER_H