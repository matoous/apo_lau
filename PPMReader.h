#pragma once
#include <iostream>
#include <fstream>
#include "pixel.h"
class PPMReader {
    public:
        PPMReader(const char* filename);
        bool isValid() const {return file.is_open();}
        // Returns black if invalid
        virtual Pixel nextColor();
        // reset reader to first color
        virtual void reset();
    protected:
        std::ifstream file;
        size_t width;
        size_t height;
        // Byte where raw bytes start
        size_t startOfData;
};
