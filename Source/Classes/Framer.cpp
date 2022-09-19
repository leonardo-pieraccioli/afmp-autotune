//includes on top
#include "Framer.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

using namespace std;

//constructor
Framer::Framer(){
    
}

//frames matrix setter
void Framer::setFrames(std::vector<std::vector<float>> framesMatrix){
    for(int i = 0; i < framesMatrix.size(); i++){
        for(int j = 0; j < framesMatrix[i].size(); j++){
            Frames[i][j] = framesMatrix[i][j];
        }
    }
    cout << "\tFrames saved" << endl;
}

//getters
std::vector<std::vector<float>> Framer::getFrames(){
    return Frames;
}
std::vector<float> Framer::getVectorOutput(){
    return vectorOutput;
}
int Framer::getWinSize() const {
    return winSize;
}
int Framer::getHopsize() const {
    return hopsize;
}

//frames matrix creation
void Framer::createFrames(std::vector<float> window){
    std::cout << "\tStart create frames" << std::endl;
    len = window.size();
    hopsize = floor(len/64);
    winSize = floor(len/16);
    // find the max number of slices that can be obtained
    int numberSlices = floor((len-winSize)/hopsize);
    // truncate if needed to get only a integer number of hop
    int trunc = numberSlices*hopsize+winSize;
    window.resize(trunc);
    std::cout << "\tWindow resized to " << window.size() << std::endl;
    // frames matrix
    for ( int i = 0; i <= numberSlices; i++) {
        int indexTimeStart = i*hopsize;
        int indexTimeEnd = i*hopsize + winSize - 1;
        Frames.push_back(std::vector<float>(window.begin()+indexTimeStart, window.begin()+indexTimeEnd));
        //std::cout << "Frame " << i << "created " << std::endl;
    }
    std::cout << "\tMatrix created" << std::endl;
}

//window reconstruction
void Framer::fusionFrames(int hopOut){
    cout << "\tFusion Frames started with hop out: " << hopOut << " with hop: " << hopsize << endl;
    int numberFrames = Frames.size();
    int timeIndex = 0;
    double ratioSample;
    auto vectorStretch = std::vector<float>(numberFrames*hopOut-hopOut+winSize, 0);
    cout << "\tVector Stretch size: " << vectorStretch.size() << " Window length: " << len << endl;

    for ( int i = 0; i < numberFrames; i++) {
        for ( int j = 0; j < Frames[i].size(); j++) {
            vectorStretch[j+timeIndex] += Frames[i][j];
        }
        timeIndex += hopOut;
    }
    //cout << "Vector stretched ok" << endl;
    
    // interpolation
    juce::Interpolators::Lagrange interpol;
    newLen = vectorStretch.size();
    ratioSample = (double) newLen/ (double) len;
    cout << "\tRatio for resampling is " << ratioSample << endl;
    vectorOutput.resize(len);

    interpol.process(ratioSample,vectorStretch.data(),vectorOutput.data(),len);
    cout << "\tInterpolation done" << endl;

    Frames.clear();
}