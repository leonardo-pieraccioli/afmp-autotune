//includes on top
#include "Framer.hpp"

//constructor
Framer::Framer(){
    
}

//implementation of functions in Framer.h
void Framer::createFrames(std::vector<float> window){
    
    len = window.size();
    hopsize = len/64;
    int winsize = len/16;

    // Find the max number of slices that can be obtained
    int numberSlices = floor((len-winsize)/hopsize);

    // Truncate if needed to get only a integer number of hop
    int trunc = len*hopsize+winsize;
    window.resize(trunc);
    
    // Frames matrix
    for ( int i = 0; i < numberSlices; i++) {
        int indexTimeStart = i*hopsize;
        int indexTimeEnd = i*hopsize + len;
        Frames[i].insert(Frames[i].begin(), window.begin()+indexTimeStart, window.begin()+indexTimeEnd);
    }

}

void Framer::fusionFrames(std::vector<std::vector<float>> Frames){
    int numberFrames = Frames.size();
    int sizeFrames = len;
    int timeIndex = 0;
    std::vector<float> vectorStretch;

    vectorStretch.assign(numberFrames*hopsize-hopsize+sizeFrames, 0);

    for ( int i = 0; i < numberFrames; i++) {
        vectorStretch[timeIndex] = vectorStretch[timeIndex] + Frames[i]; // HOW DO YOU SUM WHILE INSERTING??
        timeIndex = timeIndex + hopsize;
    }

}