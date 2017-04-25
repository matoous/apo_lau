#pragma once
#include <iostream>
#include <fstream>
#include "../pixel.h"
class PPMReader {
    public:
        PPMReader(const char* filename);
        bool isValid() const {return file.is_open();}
        virtual Pixel nextColor();
        virtual void reset();
    protected:
        std::ifstream file;
        size_t width;
        size_t height;
        size_t startOfData;
};
