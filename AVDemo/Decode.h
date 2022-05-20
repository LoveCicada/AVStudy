#pragma once

#include <iostream>

class Decode
{
public:
    Decode()
    {
        std::cout << __func__ << std::endl;
    }

    ~Decode()
    {
        std::cout << __func__ << std::endl;
    }

};