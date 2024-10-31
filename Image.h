/**
* A program that applies three different Filters to an image
*
* Completion time: 8 hr
*
* @author Vivien Stahl, Ruben Acuna
* @version 10/30/2024
*/

// Image.h

#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

// Forward declaration of Pixel struct
struct Pixel;

// Image ADT
typedef struct Image Image;

// Image structure
struct Image {
    struct Pixel** pArr;
    int width;
    int height;
};

// Function Declarations

/* Creates a new image and returns it.
 * 
 * @param  pArr: Pixel array of this image.
 * @param  width: Width of this image.
 * @param  height: Height of this image.
 * @return A pointer to a new image.
*/
Image* image_create(struct Pixel** pArr, int width, int height);

/* Destroys an image. Does not deallocate internal pixel array.
 * 
 * @param  img: the image to destroy.
*/
void image_destroy(Image** img);

/* Returns a double pointer to the pixel array.
 *
 * @param  img: the image.
*/
struct Pixel** image_get_pixels(Image* img);

/* Returns the width of the image.
 *
 * @param  img: the image.
*/
int image_get_width(Image* img);

/* Returns the height of the image.
 *
 * @param  img: the image.
*/
int image_get_height(Image* img);

/* Converts the image to grayscale.
 *
 * @param  img: the image.
*/
void image_apply_bw(Image* img);

/**
 * Shift color of the internal Pixel array. The dimension of the array is width * height.
 * The shift value of RGB is rShift, gShift, bShift. Useful for color shift.
 *
 * @param  img: the image.
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift);

/* Resizes the image using nearest neighbor scaling.
 *
 * @param  img: the image.
 * @param  factor: the scaling factor
 * @return 1 on success, 0 on failure.
*/
int image_apply_resize(Image* img, float factor);

#endif // IMAGE_H
