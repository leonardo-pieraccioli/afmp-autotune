#ifndef WINDOW_MAKER_H
#define WINDOW_MAKER_H

#include <vector>


class WindowMaker {
    private:
    //variables
    int end_i=0;
    
    //window
    std::vector<float> window;
    
    public:
    //constructor
    WindowMaker(){};
    WindowMaker(int s);

    //resetter
    void reset_end_i(){
        end_i=0;
    };

    //flag
    bool will_be_full(const int& buffer_size);

    //elaboration
    std::vector<float> get_window_to_elaborate();
    void set_window_once_elaborate(const std::vector<float>& w);

    //inout
    std::vector<float> buffer_read_and_write( const std::vector<float>& bufferIN);
};


#endif //WINDOW_MAKER_H