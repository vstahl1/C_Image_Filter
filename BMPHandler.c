/**
* A program that applies three different Filters to an image
*
* Completion time: 8 hr
*
* @author Vivien Stahl, Ruben Acuna
* @version 10/30/2024
*/

// BMPHandler.c

#include "BMPHandler.h"
#include <stdlib.h>

// Function to read BMP Header from a file
void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(&header->bfType, sizeof(unsigned short), 1, file);
    fread(&header->bfSize, sizeof(unsigned int), 1, file);
    fread(&header->bfReserved1, sizeof(unsigned short), 1, file);
    fread(&header->bfReserved2, sizeof(unsigned short), 1, file);
    fread(&header->bfOffBits, sizeof(unsigned int), 1, file);
}

// Function to write BMP Header to a file
void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->bfType, sizeof(unsigned short), 1, file);
    fwrite(&header->bfSize, sizeof(unsigned int), 1, file);
    fwrite(&header->bfReserved1, sizeof(unsigned short), 1, file);
    fwrite(&header->bfReserved2, sizeof(unsigned short), 1, file);
    fwrite(&header->bfOffBits, sizeof(unsigned int), 1, file);
}

// Function to create BMP Header based on width and height
void makeBMPHeader(struct BMP_Header* header, int width, int height) {
    header->bfType = 0x4D42; // "BM" in little endian
    // Calculate padding
    int rowSize = (3 * width + 3) & (~3);
    header->bfSize = 14 + 40 + rowSize * height;
    header->bfReserved1 = 0;
    header->bfReserved2 = 0;
    header->bfOffBits = 14 + 40; // Header sizes
}

// Function to read DIB Header from a file
void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->biSize, sizeof(unsigned int), 1, file);
    fread(&header->biWidth, sizeof(int), 1, file);
    fread(&header->biHeight, sizeof(int), 1, file);
    fread(&header->biPlanes, sizeof(unsigned short), 1, file);
    fread(&header->biBitCount, sizeof(unsigned short), 1, file);
    fread(&header->biCompression, sizeof(unsigned int), 1, file);
    fread(&header->biSizeImage, sizeof(unsigned int), 1, file);
    fread(&header->biXPelsPerMeter, sizeof(int), 1, file);
    fread(&header->biYPelsPerMeter, sizeof(int), 1, file);
    fread(&header->biClrUsed, sizeof(unsigned int), 1, file);
    fread(&header->biClrImportant, sizeof(unsigned int), 1, file);
}

// Function to write DIB Header to a file
void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->biSize, sizeof(unsigned int), 1, file);
    fwrite(&header->biWidth, sizeof(int), 1, file);
    fwrite(&header->biHeight, sizeof(int), 1, file);
    fwrite(&header->biPlanes, sizeof(unsigned short), 1, file);
    fwrite(&header->biBitCount, sizeof(unsigned short), 1, file);
    fwrite(&header->biCompression, sizeof(unsigned int), 1, file);
    fwrite(&header->biSizeImage, sizeof(unsigned int), 1, file);
    fwrite(&header->biXPelsPerMeter, sizeof(int), 1, file);
    fwrite(&header->biYPelsPerMeter, sizeof(int), 1, file);
    fwrite(&header->biClrUsed, sizeof(unsigned int), 1, file);
    fwrite(&header->biClrImportant, sizeof(unsigned int), 1, file);
}

// Function to create DIB Header based on width and height
void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->biSize = 40; // DIB Header size
    header->biWidth = width;
    header->biHeight = height;
    header->biPlanes = 1;
    header->biBitCount = 24; // 24-bit bitmap
    header->biCompression = 0; // BI_RGB, no compression
    // Calculate padding
    int rowSize = (3 * width + 3) & (~3);
    header->biSizeImage = rowSize * height;
    header->biXPelsPerMeter = 2835; // 72 DPI
    header->biYPelsPerMeter = 2835; // 72 DPI
    header->biClrUsed = 0;
    header->biClrImportant = 0;
}

// Function to read pixel data from BMP file
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    // Move to pixel array
    fseek(file, 14 + 40, SEEK_SET);
    int padding = (4 - (width * 3) % 4) % 4;
    for(int i = height -1; i >=0 ; i--){
        fread(pArr[i], sizeof(struct Pixel), width, file);
        fseek(file, padding, SEEK_CUR);
    }
}

// Function to write pixel data to BMP file
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    // Move to pixel array position
    fseek(file, 14 + 40, SEEK_SET);
    int padding = (4 - (width * 3) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};
    for(int i = height -1; i >=0 ; i--){
        fwrite(pArr[i], sizeof(struct Pixel), width, file);
        fwrite(pad, sizeof(unsigned char), padding, file);
    }
}
