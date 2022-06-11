#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include <algorithm>

class CircularBuffer {
    private:
        //pricate variables
        const static int size = 10;
        std::vector<float> buffer;
        int start_i = 0;
        int end_i = 0;
        bool is_full_flag = false;
        
        //private methods

    public: 
        //constructor
        CircularBuffer();

        //public variables

        //public methods
        void in_out (std::vector<float> inout);

        std::vector<float>* get_buffer();
};

#endif //RATIO_FINDER_H