#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <queue>

class Controller
{

private:
    typedef unsigned char TYPE_rx;
    std::queue<TYPE_rx> usbRxBuff;

public:

    Controller();
    bool fifo_is_empty(){
        return  usbRxBuff.empty();
    }
    void fifo_set(TYPE_rx rx_byte){
        usbRxBuff.push(rx_byte);
    }
    size_t  count(){
        return usbRxBuff.size();

    }
    unsigned char fifo_get(){
        TYPE_rx item =  usbRxBuff.front();
        usbRxBuff.pop();
        return item;
    }

};

#endif // CONTROLLER_H
