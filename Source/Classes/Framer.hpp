#ifndef FRAMER_H
#define FRAMER_H

#include <vector>
#include <math.h>
class Framer {
    private:
        //private variables
        int len;
        int hopsize;
        int newLen;
        int winSize;
        std::vector<std::vector<float>> Frames;
        std::vector<float> vectorOutput = std::vector<float>();
        
        //private methods
    

    public: 
        //constructor
        Framer();
        
        //public variables
        
        //methods
        void setFrames(std::vector<std::vector<float>> framesMatrix);
        std::vector<std::vector<float>> getFrames();
        std::vector<float> getVectorOutput();
        void createFrames(std::vector<float> window);
        void fusionFrames(int hopOut);

        int getWinSize() const;
        int getHopsize() const;
};

#endif //FRAMER_H