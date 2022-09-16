#ifndef RATIO_FINDER_H
#define RATIO_FINDER_H

#include <juce_dsp/juce_dsp.h>
#include <cmath>
#include <vector>

/**
 * @brief Contains functions to calculate the stretching ratio
 */
class RatioFinder {
    private:
        //------------PRIVATE VARIABLES----------
        float maxFrequency = 10000;
        float minFrequency = 100;
        float startFrequency = 440;
        std::vector<float> freqTable;

        //------------PRIVATE METHODS------------
        float findNearestNoteFrequency(float noteFrequency);

    public:
        RatioFinder();
        //GETTERS AND SETTERS
        //I added setters in case we want to make the user choose parameters from the GUI
        void setStartFrequency(float freq);
        float getStartFrequency() const;
        void setMinFrequency(float freq);
        float getMinFrequency() const;
        void setMaxFrequency(float freq);
        float getMaxFrequency() const;
        std::vector<float> getFreqTable();

        //------------PUBLIC VARIABLES------------

        //------------PUBLIC METHODS--------------
        float getRatio(std::vector<float> input, double sampleRate);
        void createFreqTable();
};

#endif //RATIO_FINDER_H