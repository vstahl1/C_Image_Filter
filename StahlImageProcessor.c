/**
* A program that applies three different Filters to an image
*
* Completion time: 8 hr
*
* @author Vivien Stahl, Ruben Acuna
* @version 10/30/2024
*/

// StahlImageProcessor.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "BMPHandler.h"
#include "Image.h"

// Function to display usage
void print_usage(char* program_name) {
    fprintf(stderr, "Usage: %s input.bmp [options]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -o output.bmp           Specify output file name.\n");
    fprintf(stderr, "  -w                      Apply grayscale filter.\n");
    fprintf(stderr, "  -r <value>              Shift red channel by <value>.\n");
    fprintf(stderr, "  -g <value>              Shift green channel by <value>.\n");
    fprintf(stderr, "  -b <value>              Shift blue channel by <value>.\n");
    fprintf(stderr, "  -s <factor>             Scale image by <factor>.\n");
}

// Function to parse command line arguments
int parse_arguments(int argc, char *argv[], char **input_filename, char **output_filename,
                    int *apply_grayscale, int *shift_red, int *rShift, int *shift_green, int *gShift,
                    int *shift_blue, int *bShift, int *apply_scale, float *scale_factor) {
    if(argc < 2){
        print_usage(argv[0]);
        return -1;
    }

    int opt;
    // Reset getopt
    opterr = 0;
    while((opt = getopt(argc, argv, "o:wr:g:b:s:")) != -1){
        char *endptr;
        switch(opt){
            case 'o':
                *output_filename = optarg;
                break;
            case 'w':
                *apply_grayscale = 1;
                break;
            case 'r':
                *rShift = strtol(optarg, &endptr, 10);
                if(*endptr != '\0'){
                    fprintf(stderr, "Invalid value for -r: %s\n", optarg);
                    return -1;
                }
                *shift_red = 1;
                break;
            case 'g':
                *gShift = strtol(optarg, &endptr, 10);
                if(*endptr != '\0'){
                    fprintf(stderr, "Invalid value for -g: %s\n", optarg);
                    return -1;
                }
                *shift_green = 1;
                break;
            case 'b':
                *bShift = strtol(optarg, &endptr, 10);
                if(*endptr != '\0'){
                    fprintf(stderr, "Invalid value for -b: %s\n", optarg);
                    return -1;
                }
                *shift_blue = 1;
                break;
            case 's':
                *scale_factor = strtof(optarg, &endptr);
                if(*endptr != '\0'){
                    fprintf(stderr, "Invalid value for -s: %s\n", optarg);
                    return -1;
                }
                if(*scale_factor <= 0){
                    fprintf(stderr, "Scaling factor must be greater than 0.\n");
                    return -1;
                }
                *apply_scale = 1;
                break;
            case '?':
                if(optopt == 'o' || optopt == 'r' || optopt == 'g' || optopt == 'b' || optopt == 's'){
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else{
                    fprintf(stderr, "Unknown option -%c.\n", optopt);
                }
                print_usage(argv[0]);
                return -1;
            default:
                print_usage(argv[0]);
                return -1;
        }
    }

    // The first non-option argument is the input file
    if(optind < argc){
        *input_filename = argv[optind];
    }
    else{
        fprintf(stderr, "Input file not specified.\n");
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}

// Function to generate default output filename
char* generate_output_filename(char* input_filename) {
    char* output_filename = NULL;
    char* dot = strrchr(input_filename, '.');
    if(dot){
        int basename_length = dot - input_filename;
        char* basename = (char*)malloc((basename_length + 1) * sizeof(char));
        if(basename == NULL){
            perror("Failed to allocate memory for basename");
            return NULL;
        }
        strncpy(basename, input_filename, basename_length);
        basename[basename_length] = '\0';

        output_filename = (char*)malloc((basename_length + 6 + strlen(dot) + 1) * sizeof(char)); // "_copy" + extension + null
        if(output_filename == NULL){
            perror("Failed to allocate memory for output filename");
            free(basename);
            return NULL;
        }
        strcpy(output_filename, basename);
        strcat(output_filename, "_copy");
        strcat(output_filename, dot);

        free(basename);
    }
    else{
        // No extension, just append "_copy"
        int len = strlen(input_filename) + 6 + 1;
        output_filename = (char*)malloc(len * sizeof(char));
        if(output_filename == NULL){
            perror("Failed to allocate memory for output filename");
            return NULL;
        }
        strcpy(output_filename, input_filename);
        strcat(output_filename, "_copy");
    }
    return output_filename;
}

// Function to free pixel array
void free_pixel_array(struct Pixel** pArr, int height) {
    for(int i = 0; i < height; i++){
        free(pArr[i]);
    }
    free(pArr);
}

// Main function
int main(int argc, char *argv[]) {
    char* input_filename = NULL;
    char* output_filename = NULL;
    int output_filename_allocated = 0;

    // Filter options
    int apply_grayscale = 0;
    int rShift = 0, gShift = 0, bShift = 0;
    int shift_red = 0, shift_green = 0, shift_blue = 0;
    float scale_factor = 1.0;
    int apply_scale = 0;

    // Parse command-line arguments
    if(parse_arguments(argc, argv, &input_filename, &output_filename,
                       &apply_grayscale, &shift_red, &rShift, &shift_green, &gShift,
                       &shift_blue, &bShift, &apply_scale, &scale_factor) != 0) {
        return EXIT_FAILURE;
    }

    // If output filename not specified, create default name
    if(output_filename == NULL){
        output_filename = generate_output_filename(input_filename);
        if(output_filename == NULL){
            // Error message already printed
            return EXIT_FAILURE;
        }
        output_filename_allocated = 1;
    }

    // Open input file
    FILE* input_file = fopen(input_filename, "rb");
    if(input_file == NULL){
        perror("Error opening input file");
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    // Read BMP Header
    struct BMP_Header bmp_header;
    readBMPHeader(input_file, &bmp_header);

    // Validate BMP file
    if(bmp_header.bfType != 0x4D42){
        fprintf(stderr, "Input file is not a valid BMP file.\n");
        fclose(input_file);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    // Read DIB Header
    struct DIB_Header dib_header;
    readDIBHeader(input_file, &dib_header);

    // Validate BMP format (24-bit uncompressed)
    if(dib_header.biBitCount != 24 || dib_header.biCompression != 0){
        fprintf(stderr, "Unsupported BMP format. Only 24-bit uncompressed BMP files are supported.\n");
        fclose(input_file);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    int width = dib_header.biWidth;
    int height = dib_header.biHeight;

    // Allocate memory for pixels
    struct Pixel** pArr = (struct Pixel**)malloc(height * sizeof(struct Pixel*));
    if(pArr == NULL){
        perror("Failed to allocate memory for pixel array");
        fclose(input_file);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    int allocation_failed = 0;
    for(int i = 0; i < height; i++){
        pArr[i] = (struct Pixel*)malloc(width * sizeof(struct Pixel));
        if(pArr[i] == NULL){
            perror("Failed to allocate memory for pixel row");
            allocation_failed = 1;
            // Free previously allocated rows
            for(int j = 0; j < i; j++) free(pArr[j]);
            break;
        }
    }

    if(allocation_failed){
        free(pArr);
        fclose(input_file);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    // Read pixel data
    readPixelsBMP(input_file, pArr, width, height);
    fclose(input_file);

    // Create Image object
    Image* img = image_create(pArr, width, height);
    if(img == NULL){
        // Error message already printed
        free_pixel_array(pArr, height);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    // Apply filters
    if(apply_grayscale){
        image_apply_bw(img);
    }

    if(shift_red || shift_green || shift_blue){
        image_apply_colorshift(img, shift_red ? rShift : 0, shift_green ? gShift : 0, shift_blue ? bShift : 0);
    }

    if(apply_scale){
        if(!image_apply_resize(img, scale_factor)){
            // Resize failed
            image_destroy(&img);
            if(output_filename_allocated){
                free(output_filename);
            }
            return EXIT_FAILURE;
        }
    }

    // Update headers if resized
    if(apply_scale){
        // Update BMP and DIB headers
        makeBMPHeader(&bmp_header, img->width, img->height);
        makeDIBHeader(&dib_header, img->width, img->height);
    }

    // Open output file
    FILE* output_file = fopen(output_filename, "wb");
    if(output_file == NULL){
        perror("Error opening output file");
        image_destroy(&img);
        if(output_filename_allocated){
            free(output_filename);
        }
        return EXIT_FAILURE;
    }

    // Write BMP Header
    writeBMPHeader(output_file, &bmp_header);

    // Write DIB Header
    writeDIBHeader(output_file, &dib_header);

    // Write pixel data
    writePixelsBMP(output_file, img->pArr, img->width, img->height);
    fclose(output_file);

    printf("Output file name was %s.\n", output_filename);

    // Free resources
    image_destroy(&img);
    if(output_filename_allocated){
        free(output_filename);
    }

    return EXIT_SUCCESS;
}
