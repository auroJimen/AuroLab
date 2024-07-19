#include <string>
#include <iostream>
#include <inttypes.h>
#include <cmath>

using namespace std;

//BMP file structure
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

uint16_t convert(uint8_t lowByte, uint8_t highByte){

    return (unsigned int)(highByte << 8 | lowByte);
                    
};

int main(int argc, char* argv[]){
    if (argc != 2) return 1;
    else {
        //Variable defs
        uint32_t width;
        uint32_t height;
        uint32_t bytesPerPixel;
        uint32_t dataOffset;
        uint16_t bitsPerPixel;

        //Read important info from header
        string filename = (string)argv[1];
        FILE* imageFile = fopen(filename.c_str(), "rb");            //Open file

        if (imageFile == NULL) {
            cout << "File not found";
            return 1;
        }
        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);     //Move to where data offset is stored
        fread(&dataOffset, 4, 1, imageFile);                //Save data offset
        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);           //Move to width offset location
        fread(&width, 4, 1, imageFile);                     //Save width value
        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);          //Move to height offset location
        fread(&height, 4, 1, imageFile);                    //Save height value
        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);  //Move to bitsPerPixel location
        fread(&bitsPerPixel, 2, 1, imageFile);              //Save bitsPerPixel value
        bytesPerPixel = ((uint32_t)bitsPerPixel) / 8;       //Convert val to bytes

        //Output header info
        cout << ("Information read from "+(string)filename+" header\n");
        cout << "Data offset: ";
        cout << dataOffset;
        cout << "\nSize: ";
        cout << width;
        cout << "x";
        cout << height;
        cout << " px\n";
        cout << ("Bytes per pixel: "+ to_string(bytesPerPixel)+'\n');
        if (bytesPerPixel != 2) return 1;

        //Manipulate image data:

        //The bmp format stores images as padded "scanlines" whose size must be a multiple of 4 bytes
        int paddedRowSize = (int)(4 * ceil((float)(width) / 4.0f))*(bytesPerPixel);
        int unpaddedRowSize = width*bytesPerPixel;
        //The total size of the image data (in bytes) is:
        int totalSize = unpaddedRowSize*height; 

        //We are assuming rgb565 format, so two bytes per pixel
        uint8_t aux[height][unpaddedRowSize];
        uint8_t lowByte;
        uint8_t highByte;
        uint16_t value;

        for (int i = 0; i < height; i++){
            
            //Seek current row & save to aux (rows are saved inversly in the bmp)
            fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
            fread(aux[height-1-i], 1, unpaddedRowSize, imageFile);

        }

        fclose(imageFile);

        //Change file extention & create new file:
        int size = filename.length();
        filename.erase(size-3,size-1);
        string imageName = filename;
        imageName.erase(imageName.length()-1);
        filename.append("h");
        FILE* cfile = fopen(filename.c_str(), "w");

        //Begin writing image data to the text file
        fprintf(cfile, "#include <cstdint>\n");
        fprintf(cfile, "static const uint16_t %s[] = {\n", imageName.c_str());

        for(int i= 0; i < (unsigned int)height; i++) {

            for(int j= 0; j < (unsigned int)width; j++) {

                //Manipulate pixel data to convert to rgb365 (2bits per pixel)
                highByte = aux[i][2*j];
                lowByte = aux[i][2*j +1];
                value = (unsigned int)convert(lowByte, highByte);

                //printf("%i, %i, %u, %u, %u\n", i, j, highByte, lowByte, value);

                if (j == (width-1)) fprintf(cfile, "0x%04x,\n ", value);
                else fprintf(cfile, "0x%04x, ", value);

            };

        };

        fprintf(cfile, "};");
        fclose(cfile);

    }
}