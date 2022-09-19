//includes on top
#include "PitchShifter.hpp"

using namespace std;

//constructor
PitchShifter::PitchShifter(){
    
}

//implementation of functions in PitchShifter.h
std::vector<std::vector<float>> PitchShifter::execute(std::vector <std::vector<float>> outputy,const unsigned int & winSize, const unsigned int& hop,const float & ratio){
    cout << "\tPitch Shifter... ";
    float hopOut = round(hop * ratio); //computing the hop scaled by the ratio
    auto wn = std::vector <float>(winSize);
    wn = hann(wn, winSize); //creating the Hanning window of winSize points
    //cout << "Hann done" << endl;

    auto fft = juce::dsp::FFT (floor(log2(winSize))); //TODO attento all'ordine
    //cout << "FFT created" << endl;

    //Vectors resizes
    if(winSize!=phaseCumulative.size()){
        phaseCumulative.resize(winSize);
        phaseFrame.resize(winSize);
        magFrame.resize(winSize);
        previousPhase.resize(winSize);
        deltaPhi.resize(winSize);
        trueFreq.resize(winSize);
    }

    auto currentFrameFFT = std::vector<std::complex<float>>();
    
    for (int i = 0; i < outputy.size(); ++i) {
        //cout << "Frame " << i << endl;

        //Analysis
        toComplex(outputy[i], currentFrame);
        //cout << "Complexata" << endl;

        for (int j = 0; j < currentFrame.size(); ++j){
            currentFrame[j] *= wn[j]; //hanning window
        }
        //cout << "Window done" << endl;

        currentFrameFFT.resize(currentFrame.size());
        fft.perform(currentFrame.data(), currentFrameFFT.data(), false);
        //cout << "FFT 1 done" << endl;

        for (int j = 0; j < currentFrameFFT.size(); ++j){
            //Gets the magnitude and the phase of the frame in elaboration
            magFrame[j] = std::abs(currentFrameFFT[j]);
            phaseFrame[j] = std::arg(currentFrameFFT[j]);

        //Processing
            deltaPhi[j] = (phaseFrame[j] - previousPhase[j]) - (hop * 2 * M_PI * j / winSize); //lines 102 and 106 of Matlab pitchShift.m script
            deltaPhi[j] = fmod((deltaPhi[j] + M_PI), (2 * M_PI)) - M_PI; //fmod gives out the floating-point remainder
            previousPhase[j] = phaseFrame[j];
            trueFreq[j] = (2 * M_PI * j / winSize) + (deltaPhi[j]/hop);
            phaseCumulative[j] = phaseCumulative[j] + hopOut * trueFreq[j];
        
        //Synthesis      
            std::complex<float> tempComplex(0, phaseCumulative[j]);
            tempComplex = magFrame[j] * std::exp(tempComplex);

            currentFrameFFT[j]=tempComplex;
        }

        //IFFT
        fft.perform(currentFrameFFT.data(), currentFrame.data(), true);
        //cout << "FFT 2 done" << endl;
        for (int j = 0; j < currentFrame.size(); ++j) {
            outputy[i][j] = real(currentFrame[j]);
            outputy[i][j] *= wn[j];
        }
    }

    cout << "\t...done" << endl;
    return outputy;
}

//Hanning function generator
std::vector<float> PitchShifter::hann(std::vector<float> &inputVector, unsigned int n_points){
    for (int i = 0; i < n_points; ++i)
        inputVector[i] = 0.5 * (1 - cos(2 * M_PI*i/n_points));
    return inputVector;
}

//This function takes a float vector and converts it into a complex<float> vector
void PitchShifter::toComplex(const std::vector<float>& toConvert, std::vector<std::complex<float>>& converted)
{
    int size = toConvert.size();
    std::complex<float> temp;
    converted.clear();
    for (int i = 0; i < size; ++i) {
        temp = toConvert[i];
        converted.push_back(temp);
    }
}
