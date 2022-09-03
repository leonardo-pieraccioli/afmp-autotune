//includes on top
#include "Framer.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

//constructor
Framer::Framer(){
    
}

void Framer::setFrames(std::vector<std::vector<float>> framesMatrix){
    for(int i = 0; i < framesMatrix.size(); i++){
        for(int j = 0; j < framesMatrix[i].size(); j++){
            Frames[i][j] = framesMatrix[i][j];
        }
    }
}
std::vector<std::vector<float>> Framer::getFrames(){
    return Frames;
}

std::vector<float> Framer::getVectorOutput(){
    return vectorOutput;
}


//implementation of functions in Framer.h
void Framer::createFrames(std::vector<float> window){
    
    len = window.size();
    hopsize = len/64;
    winSize = len/16;

    // Find the max number of slices that can be obtained
    int numberSlices = floor((len-winSize)/hopsize);

    // Truncate if needed to get only a integer number of hop
    int trunc = len*hopsize+winSize;
    window.resize(trunc);
    
    // Frames matrix
    for ( int i = 0; i < numberSlices; i++) {
        int indexTimeStart = i*hopsize;
        int indexTimeEnd = i*hopsize + len;
        Frames[i].insert(Frames[i].begin(), window.begin()+indexTimeStart, window.begin()+indexTimeEnd);
    }

}

void Framer::fusionFrames(int hopOut){
    int numberFrames = Frames.size();
    int sizeFrames = winSize;
    int timeIndex = 0;
    double ratioSample;
    std::vector<float> vectorStretch;

    vectorStretch.assign(numberFrames*hopsize-hopsize+sizeFrames, 0);

    for ( int i = 0; i < numberFrames; i++) {
        for ( int j = 0; j < Frames[i].size(); j++) {
        vectorStretch[j+timeIndex] += Frames[i][j];
        }
        timeIndex += hopOut;
    }
    // interpolation
    juce::Interpolators::Linear interpol;
    newLen = vectorStretch.size();
    ratioSample = newLen/sizeFrames;

    interpol.process(ratioSample,vectorStretch.data(),vectorOutput.data(),sizeFrames);

}