#include "helpers.h"
#include <stdio.h>
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

    /* Algorithm for applying Sepian tone to a pixel
    sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
    sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
    sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue */

    BYTE oRed;
    WORD sRed;

    BYTE oGreen;
    WORD sGreen;

    BYTE oBlue;
    WORD sBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Aliases for original colors
            oRed = image[i][j].rgbtRed;
            oGreen = image[i][j].rgbtGreen;
            oBlue = image[i][j].rgbtBlue;

            // Obtaining Sepian tone of original colors
            sRed = round((0.393 * oRed) + (0.769 * oGreen) + (0.189 * oBlue));
            sGreen = round((0.349 * oRed) + (0.686 * oGreen) + (0.168 * oBlue));
            sBlue = round((0.272 * oRed) + (0.534 * oGreen) + (0.131 * oBlue));
            
            // Rounding Sepian tone colors to nearest integer in domain of [0,255]
            sBlue = (sBlue > 255)? 255: sBlue;
            sGreen = (sGreen > 255)? 255: sGreen;
            sRed = (sRed > 255)? 255: sRed;       

            // Creating a pixel with sepian tone of the origianl pixel
            RGBTRIPLE sepianPixel = {sBlue, sGreen, sRed};
            
            // Applying Sepian tone to original image's pixel
            image[i][j] = sepianPixel;
        }
    }

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
