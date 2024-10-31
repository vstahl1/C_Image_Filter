/**
* A program that applies three different Filters to an image
*
* Completion time: 8 hr
*
* @author Vivien Stahl, Ruben Acuna
* @version 10/30/2024
*/

// BMPHandler.h

#ifndef BMPHANDLER_H
#define BMPHANDLER_H

#include <stdio.h>
#include "Image.h"

// Structure for BMP Header (14 bytes)
struct BMP_Header {
    unsigned short bfType;      // File type ("BM")
    unsigned int bfSize;        // Size of the file in bytes
    unsigned short bfReserved1; // Reserved, must be 0
    unsigned short bfReserved2; // Reserved, must be 0
    unsigned int bfOffBits;     // Offset to start of Pixel Data
};

// Structure for DIB Header (40 bytes)
struct DIB_Header {
    unsigned int biSize;          // Size of this header (40 bytes)
    int biWidth;                  // Width of the bitmap in pixels
    int biHeight;                 // Height of the bitmap in pixels
    unsigned short biPlanes;      // Number of color planes (must be 1)
    unsigned short biBitCount;    // Number of bits per pixel
    unsigned int biCompression;   // Compression type (0 = BI_RGB)
    unsigned int biSizeImage;     // Image size (can be 0 for BI_RGB)
    int biXPelsPerMeter;          // Horizontal resolution (pixels per meter)
    int biYPelsPerMeter;          // Vertical resolution (pixels per meter)
    unsigned int biClrUsed;       // Number of colors in the color palette
    unsigned int biClrImportant;  // Number of important colors used
};

// Structure for a single 24-bit Pixel
struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

/**
 * Read BMP header of a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * Write BMP header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * Read DIB header from a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * Write DIB header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * Make BMP header based on width and height. Useful for creating a BMP file.
 *
 * @param  header: Pointer to the destination BMP header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height);

/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height);

/**
 * Read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);

/**
 * Write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);

#endif // BMPHANDLER_H