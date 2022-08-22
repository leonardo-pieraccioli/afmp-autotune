#ifndef FRAMER_H
#define FRAMER_H

#include <vector>
#include <math.h>
class Framer {
    private:
        //private variables

        //private methods

    public: 
        //constructor
        Framer();
        
        //public variables
        std::vector<std::vector<float>> Frames;
        
        //methods
        void createFrames(std::vector<float> window);
        void fusionFrames(/*add arguments*/);
};

#endif //FRAMER_H