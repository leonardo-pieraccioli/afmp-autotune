#include "CircularBuffer.hpp"
//constructor
CircularBuffer::CircularBuffer(int s){
    size = s;
    end_i = size-1;
    window = std::vector<float>(size,0.0);
}

//overload operator []
float &CircularBuffer::operator[](int i){
    int j = i + start_read_i; //set index 0 to start_read_i
    int a = j/size;  //ratio rounded in case i is few times higher than size
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

    return bufferOUT;
}


//sending window to elaborare
std::vector<float> CircularBuffer::get_window_to_elaborate(){
    int a = size+start_read_i-end_i-1;
    std::vector<float> temp_window(a,0);
    for (int i = 1; i<=a; i++){
        temp_window[a-i] = (*this)[-i];
    }

    return temp_window; 
}

//receiving elaborated window
void CircularBuffer::set_window_once_elaborate(const std::vector<float>& w){
    int a = size+start_read_i-end_i-1;
    
    for (int i = 1; i<=a; i++){
        (*this)[-i] = w[a-i];
    }

    end_i = start_read_i-1;
}