#include "WindowMaker.hpp"

#include <iostream>

//constructor
WindowMaker::WindowMaker(int s){
    window = std::vector<float>(s,0);
}

//flag function used as a condition for the elaboration part
bool WindowMaker::will_be_full(const int& buffer_size){
    if(end_i+buffer_size>window.size())
        return true;
    return false;
}

//read and write of the buffer given by JUCE
std::vector<float> WindowMaker::buffer_read_and_write(const std::vector<float>& bufferIN){
    std::vector<float> bufferOUT;
    int buffer_size = bufferIN.size();

    //sending output
    for (int i = 0; i<buffer_size; i++)
        bufferOUT.push_back(window[i+end_i]);

    //memorizing input
    for (int i = 0; i<buffer_size; i++)
        window[i+end_i] = bufferIN[i];

    end_i+=buffer_size;
    return bufferOUT;
}


//sending window to elaborate
std::vector<float> WindowMaker::get_window_to_elaborate(){
    std::vector<float> temp_window(end_i,0);

    for (int i = 0; i<=end_i; i++)
        temp_window[i] = window[i];

    return temp_window; 
}

//receiving elaborated window
void WindowMaker::set_window_once_elaborate(const std::vector<float>& w){
    for(int i=0; i<=w.size(); i++)
        window[i]=w[i];
}