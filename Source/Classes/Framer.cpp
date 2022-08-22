//includes on top
#include "Framer.hpp"

//constructor
Framer::Framer(){
    
}

//implementation of functions in Framer.h
void Framer::createFrames(std::vector<float> window){
    
    int len = window.size();
    int hopsize = len/64;
    int winsize = len/16;

    // Find the max number of slices that can be obtained
    int numberSlices = floor((len-winsize)/hopsize);

    // Truncate if needed to get only a integer number of hop
    int trunc = len*hopsize+winsize;
    window.resize(trunc);
    
    // Frames matrix!!


}

void Framer::fusionFrames(/*definearguments*/){
    //write code
}