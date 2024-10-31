/**
* A program that applies three different Filters to an image
*
* Completion time: 8 hr
*
* @author Vivien Stahl, Ruben Acuna
* @version 10/30/2024
*/

// Image.c

#include "Image.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Define the Pixel structure
struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

// Function to create a new image
Image* image_create(struct Pixel** pArr, int width, int height) {
    if(pArr == NULL){
        fprintf(stderr, "Pixel array is NULL.\n");
        return NULL;
    }
    Image* img = (Image*)malloc(sizeof(Image));
    if(img == NULL){
        perror("Failed to allocate memory for Image");
        return NULL;
    }
    img->pArr = pArr;
    img->width = width;
    img->height = height;
    return img;
}

// Function to destroy an image
void image_destroy(Image** img) {
    if(img && *img){
        // Free each row
        for(int i = 0; i < (*img)->height; i++){
            free((*img)->pArr[i]);
        }
        // Free pixel array
        free((*img)->pArr);
        // Free image structure
        free(*img);
        *img = NULL;
    }
}

// Function to get pixel array
struct Pixel** image_get_pixels(Image* img) {
    return img->pArr;
}

// Function to get image width
int image_get_width(Image* img) {
    return img->width;
}

// Function to get image height
int image_get_height(Image* img) {
    return img->height;
}

// Function to apply grayscale filter
void image_apply_bw(Image* img) {
    for(int i = 0; i < img->height; i++) {
        for(int j = 0; j < img->width; j++) {
            struct Pixel* p = &img->pArr[i][j];
            unsigned char grayscale = (unsigned char)(0.299 * p->red + 0.587 * p->green + 0.114 * p->blue);
            p->red = grayscale;
            p->green = grayscale;
            p->blue = grayscale;
        }
    }
}

// Helper function to clamp color values
unsigned char clamp(int value) {
    if(value < 0) return 0;
    if(value > 255) return 255;
    return (unsigned char)value;
}

// Function to apply color shift
void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift) {
    for(int i = 0; i < img->height; i++) {
        for(int j = 0; j < img->width; j++) {
            struct Pixel* p = &img->pArr[i][j];
            p->red = clamp(p->red + rShift);
            p->green = clamp(p->green + gShift);
            p->blue = clamp(p->blue + bShift);
        }
    }
}

// Function to apply resize using nearest neighbor
int image_apply_resize(Image* img, float factor) {
    if(factor <= 0){
        fprintf(stderr, "Scaling factor must be greater than 0.\n");
        return 0;
    }

    int new_width = (int)(img->width * factor);
    int new_height = (int)(img->height * factor);

    if(new_width == 0) new_width = 1;
    if(new_height == 0) new_height = 1;

    // Allocate new pixel array
    struct Pixel** new_pArr = (struct Pixel**)malloc(new_height * sizeof(struct Pixel*));
    if(new_pArr == NULL){
        perror("Failed to allocate memory for resized pixel array");
        return 0;
    }

    int allocation_failed = 0;
    for(int i = 0; i < new_height; i++) {
        new_pArr[i] = (struct Pixel*)malloc(new_width * sizeof(struct Pixel));
        if(new_pArr[i] == NULL){
            perror("Failed to allocate memory for resized pixel row");
            allocation_failed = 1;
            // Free previously allocated rows
            for(int j = 0; j < i; j++) free(new_pArr[j]);
            break;
        }
    }

    if(allocation_failed){
        free(new_pArr);
        return 0;
    }

    // Apply nearest neighbor
    for(int i = 0; i < new_height; i++) {
        for(int j = 0; j < new_width; j++) {
            int orig_i = (int)(i / factor);
            int orig_j = (int)(j / factor);

            if(orig_i >= img->height) orig_i = img->height -1;
            if(orig_j >= img->width) orig_j = img->width -1;

            new_pArr[i][j] = img->pArr[orig_i][orig_j];
        }
    }

    // Free old pixel array
    for(int i = 0; i < img->height; i++) {
        free(img->pArr[i]);
    }
    free(img->pArr);

    // Update image
    img->pArr = new_pArr;
    img->width = new_width;
    img->height = new_height;

    return 1;
}
