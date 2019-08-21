#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include "command.h"
#include "dmfb.h"
using std::istream;


class Receiver
{
public:
    enum{
        NO_FILE_ERROR = 3
    };
    Receiver(DMFB* d);
    void get_command();
    ~Receiver();
private:
    DMFB* dmfb;
};

#endif // RECEIVER_H
