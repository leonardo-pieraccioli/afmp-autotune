#ifndef PITCH_SHIFTER_H
#define PITCH_SHIFTER_H

#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>
#include <juce_dsp/juce_dsp.h>


class PitchShifter {
    private:
        //private variables
        unsigned int numberFrames;
        std::vector<float> phaseCumulative;
        std::vector<float> phaseFrame;
        std::vector<float> magFrame;
        std::vector<std::complex<float>> currentFrame;
        std::vector<float> deltaPhi;
        std::vector<float> trueFreq;
        float previousPhase;
        //private methods
        std::vector<float> hann(std::vector<float> &inputVector, unsigned int n_points);
        void toComplex(const std::vector<float>& toConvert, std::vector<std::complex<float>> & converted);
    public: 
        //constructor
        PitchShifter();

        //public variables

        //public methods
        std::vector<std::vector<float>> execute(std::vector <std::vector<float>> outputy, const unsigned int& winSize, const unsigned int& hop, const float& ratio);
};

#endif //PITCH_SHIFTER_H