#include "error.h"
#include <iostream>

namespace nodewalker{
    error::error(const std::string str){
        std::cout<<str<<std::endl;
    }
}

