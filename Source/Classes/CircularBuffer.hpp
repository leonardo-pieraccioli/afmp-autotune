#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>


class CircularBuffer {
    private:
    int size = 0; 
    int start_i = 0;
    int end_i = 0; //end of the array, end_i+1 is the start of the computed part
    std::vector<float> window;
    
    public:
    //constructor
    CircularBuffer(){};
    CircularBuffer(int s);
    //overload operator
    float &operator[](int i);
    //flag
    bool will_be_full(const int& buffer_size);
    //elaboration
    std::vector<float> get_window_to_elaborate();
    void set_window_once_elaborate(const std::vector<float>& w);
    //inout
    std::vector<float> buffer_read_and_write( const std::vector<float>& bufferIN);
};


























// class CircularBuffer {
//     private:
//         //pricate variables
//         const static int size = 10;
//         std::vector<float> window;
//         std::vector<float> memory;
//         int start_i = 0;
//         int end_i = 0;
//         bool is_full_flag = false;
//         int offset=0;
        
//         //private methods

//     public: 
//         //constructor
//         CircularBuffer();

//         //public variables

//         //public methods
//         void in_out (std::vector<float>& inout);

//         //float &operator[](int i);

//         std::vector<float> get_window();
//         void set_window(std::vector<float> buffer);
//         bool get_flag ();
// };

#endif //CIRCULAR_BUFFER_H