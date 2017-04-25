#include "PPMReader.h"
PPMReader::PPMReader(const char* filename) {
    file.open(filename);
    std::string bleh;
    if(isValid()) {
        file>>bleh;
        file.get();
        char buffer[40];
        while(file.peek()=='#') {
            file.getline(buffer, 40);
        }
        file>>width>>height;
        int ignored = -1;
        file>>ignored;
        // Ignore newline, if any
        if(file.peek()=='\n')
            file.get();
        // remember start offset
        startOfData = file.tellg();
    }
}
Pixel PPMReader::nextColor() {
    if(isValid()) {
        if(file.peek() == EOF) {
            file.seekg(startOfData);
            std::cout<<"Overflow in the file.";
        }
        Pixel c(file.get(), file.get(), file.get());
        return c;
    }
    return Pixel(0,0,0);
}

void PPMReader::reset() {
    file.seekg(startOfData);
    std::cout<<"Reset to: "<<startOfData<<std::endl;
}
