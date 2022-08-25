#ifndef FRAMER_H
#define FRAMER_H

#include <vector>
#include <math.h>
class Framer {
    private:
        //private variables
        int len;
        int hopsize;
        //private methods

    public: 
        //constructor
        Framer();
        
        //public variables
        std::vector<std::vector<float>> Frames;
        std::vector<float> vectorOutput;
        
        //methods
        void createFrames(std::vector<float> window);
        void fusionFrames(std::vector<std::vector<float>> Frames);
};

#endif //FRAMER_H