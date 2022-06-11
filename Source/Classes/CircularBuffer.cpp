//includes on top
#include "CircularBuffer.hpp"

//constructor
CircularBuffer::CircularBuffer(){
    for(int i = 0; i<size; i++){
        buffer.push_back(0);
    }
}

//main method 
void CircularBuffer::in_out (std::vector<float> inout){
    end_i += inout.size();
    
    std::vector<float> temp_in;
    //copying
    for(int i=0; i<inout.size(); i++){
        temp_in[i] = inout[i];
    }
    //sending the output
    for(int i=0; i<inout.size(); i++){
        inout[i] = buffer[start_i + i];
    }
    //memorizing the input
    for(int i=0; i<inout.size(); i++){
        buffer[start_i + i] = temp_in[i];
    }
}

//output method
std::vector<float>* CircularBuffer::get_buffer(){
    return &buffer;
}
