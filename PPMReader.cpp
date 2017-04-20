#include "PPMReader.h"
PPMReader::PPMReader(const char* filename) {
    file.open(filename);
    std::string bleh;
    if(isValid()) {
        file>>bleh;
        std::cout<<"Magic number: "<<bleh<<std::endl;
        file.get();
        char buffer[40];
        std::cout<<"Next character: "<<(char)file.peek()<<std::endl;
        while(file.peek()=='#') {
            file.getline(buffer, 40);
            std::cout<<"Ignoring comment: "<<buffer<<std::endl;
        }
        file>>width>>height;
        std::cout<<"Dimensions: "<<width<<"x"<<height<<std::endl;
        int ignored = -1;
        file>>ignored;
        std::cout<<"Max brightness: "<<ignored<<std::endl;
        // Ignore newline, if any
        if(file.peek()=='\n')
            file.get();
        // remember start offset
        startOfData = file.tellg();
        std::cout<<"Data starts at byte offset "<<startOfData<<std::endl;
    }
}
Pixel PPMReader::nextColor() {
    cout << "HAHA" << endl;
    if(isValid()) {
        if(file.peek() == EOF) {
            file.seekg(startOfData);
            std::cout<<"Overflow in the file.";
        }
        Pixel c(file.get(), file.get(), file.get());
        return c;
    }
//std::cout<<"dfdsgdf\n";
    return Pixel(0,0,0);
}
void PPMReader::reset() {
    file.seekg(startOfData);
    std::cout<<"Reset to: "<<startOfData<<std::endl;
}
