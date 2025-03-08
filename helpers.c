#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Shades of grey can be obtained by setting equal values for all three colors

    // Stores the obtained shade of pixel
    BYTE shade;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            shade = 
            round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // To convert an rgb pixel into graysacale; all rgb must have equal values.
            RGBTRIPLE grey = {shade, shade, shade};

            image[i][j] = grey;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
