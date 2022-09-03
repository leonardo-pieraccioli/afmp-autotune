//includes on top
#include "PitchShifter.hpp"


//constructor
PitchShifter::PitchShifter(){
    
}

//implementation of functions in PitchShifter.h
void PitchShifter::execute(std::vector <std::vector<float>> outputy,const unsigned int & winSize, const unsigned int& hop,const float & ratio){
    //write code   
    float hopOut = round(hop * ratio); //computing the hop scaled by the ratio
    std::vector <float> wn = hann(wn, winSize); //creating the Hanning window of winSize points
    
    auto fft = juce::dsp::FFT (floor(log2(winSize))); //TODO attento all'ordine
    
    //Vectors initializations
    phaseCumulative = std::vector<float>(winSize);
    phaseFrame = std::vector<float>(winSize);
    magFrame = std::vector<float>(winSize);
    previousPhase = 0.0;
    deltaPhi = std::vector<float>(winSize);
    trueFreq = std::vector<float>(winSize);

    
    for (int i = 0; i < numberFrames; ++i) {

        //Analysis
        toComplex(outputy[i], currentFrame);
        for (int j = 0; j < currentFrame.size(); ++j){
            currentFrame[j] = currentFrame[j] * wn[j]; //applies the Hann window to the frame
            //FFT
            fft.perform(&currentFrame[j], &currentFrame[j], false); //ATTENTO CHE HAI MESSO INPUT E OUTPUT UGUALI 
            //Gets the magnitude and the phase of the frame in elaboration
            magFrame[j] = std::abs(currentFrame[j]);
            phaseFrame[j] = std::arg(currentFrame[j]);

        //Processing
            deltaPhi[j] = (phaseFrame[j] - previousPhase) - hop * 2 * M_PI * j / winSize;  //lines 102 and 106 of Matlab pitchShift.m script
            deltaPhi[j] = fmod((deltaPhi[j] + M_PI), (2 * M_PI)) - M_PI; //fmod gives out the floating-point remainder
            previousPhase = phaseFrame[j];
            trueFreq[j] = 2 * M_PI * j / winSize + deltaPhi[j]/hop;
            phaseCumulative[j] = phaseCumulative[j] + hopOut * trueFreq[j];
        
        //Synthesis      
            std::complex<float> tempComplex(0, phaseCumulative[j]);
            tempComplex = magFrame[j] * std::exp(tempComplex);
            //IFFT
            fft.perform(&currentFrame[j], &currentFrame[j], true); //ATTENTO CHE HAI MESSO INPUT E OUTPUT UGUALI
            outputy[i][j] = real(currentFrame[j]);
            outputy[i][j] *= wn[j];
        }
    }
}

//Hann function generator
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
