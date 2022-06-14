#include "CircularBuffer.hpp"
//constructor
CircularBuffer::CircularBuffer(int s){
    size = s;
    end_i = size;
    window = std::vector<float>(size,0.0);
}

//overload operator []
float &CircularBuffer::operator[](int i){
    int a = i/size;  //ratio rounded in case i is few times higher than size
    int j = i + start_read_i; //set index 0 to start_read_i
    if( j < 0){
        if( j <= -size){
            return window [j + size*(a+1)];
        }
        return window [j + size];
    }
    if( j >= size){
        return window [j - size*a];
    }
    return window[j];
}

//flag
bool CircularBuffer::will_be_full(const int& buffer_size){
    if(start_read_i + buffer_size > end_i){
        return true;
    }
    return false;
}

//read and write of the buffer
std::vector<float> CircularBuffer::buffer_read_and_write(const std::vector<float>& bufferIN){
    std::vector<float> bufferOUT;
    int buffer_size = bufferIN.size();

    //sending output
    for (int i = 0; i<buffer_size; i++){
        bufferOUT.push_back((*this)[i]);
    }  

    //memorizing input
    for (int i = 0; i<buffer_size; i++){
        (*this)[i] = bufferIN[i];
    }

    //resetting start index, if it surpass the size of the vector i subtract the size form it to keep it from going up to infinity
    int temp = start_read_i + buffer_size;
    if(temp >= size) {
        start_read_i = temp - size;
    } else start_read_i = temp;
}


//sending window to elaborare
std::vector<float> CircularBuffer::get_window_to_elaborate(){
    std::vector<float> temp_window;
    
    for (int i = end_i + 1; i< start_read_i; i++){
        temp_window.push_back((*this)[i]);
    }

    return temp_window; 
}

//receiving elaborated window
void CircularBuffer::set_window_once_elaborate(const std::vector<float>& w){
    
    for (int i = end_i + 1, j=0; i< start_read_i, j<w.size(); i++, j++){
        (*this)[i]=w[j];
    }

    end_i = start_read_i-1;
}