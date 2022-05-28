#pragma once

#include <iostream>

class Demux
{
public:
    Demux()
    {
        std::cout << __func__ << std::endl;
    }

    ~Demux()
    {
        std::cout << __func__ << std::endl;
    }

};