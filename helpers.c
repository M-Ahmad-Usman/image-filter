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
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        /*  Iterating over half of the row so that first half can be swapped with second half*/
        for (int j = 0; j < width / 2; j++)
        {
            // Swapping the pixels
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Gx kernel for horizontal edges
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    // Gy kernel for vertical edges
    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // Creating copy of original image with with black border outside whole image
    RGBTRIPLE cp[height+2][width+2];

    RGBTRIPLE outsideBorder = {0,0,0};

    // Setting black border on rows
    for (int i = 0; i < width + 2; i++)
    {
        // Setting black color to first row
        cp[0][i] = outsideBorder;

        // Setting black color to last row
        cp[height+1][i] = outsideBorder;
    }

    // Setting black border on columns
    for (int i = 0; i < height + 2; i++)
    {
        // Setting black border to first column of whole image
        cp[i][0] = outsideBorder;

        // Setting black border to last column of whole image
        cp[i][width+1] = outsideBorder;
    }

    // Copying the original image into cp
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            cp[i][j] = image[i-1][j-1];
        }
    }

    // Applying Sobel operator
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            // Calculating Gx of each color in current pixel
            int bluex = (
                    cp[i-1][j-1].rgbtBlue * Gx[0][0] + cp[i-1][j].rgbtBlue * Gx[0][1] + cp[i-1][j+1].rgbtBlue * Gx[0][2] +
                    cp[i][j-1].rgbtBlue * Gx[1][0] + cp[i][j].rgbtBlue * Gx[1][1] + cp[i][j+1].rgbtBlue * Gx[1][2] +
                    cp[i+1][j-1].rgbtBlue * Gx[2][0] + cp[i+1][j].rgbtBlue * Gx[2][1] + cp[i+1][j+1].rgbtBlue * Gx[2][2] );
            int greenx = (
                    cp[i-1][j-1].rgbtGreen * Gx[0][0] + cp[i-1][j].rgbtGreen * Gx[0][1] + cp[i-1][j+1].rgbtGreen * Gx[0][2] +
                    cp[i][j-1].rgbtGreen * Gx[1][0] + cp[i][j].rgbtGreen * Gx[1][1] + cp[i][j+1].rgbtGreen * Gx[1][2] +
                    cp[i+1][j-1].rgbtGreen * Gx[2][0] + cp[i+1][j].rgbtGreen * Gx[2][1] + cp[i+1][j+1].rgbtGreen * Gx[2][2] );
            int redx = (
                    cp[i-1][j-1].rgbtRed * Gx[0][0] + cp[i-1][j].rgbtRed * Gx[0][1] + cp[i-1][j+1].rgbtRed * Gx[0][2] +
                    cp[i][j-1].rgbtRed * Gx[1][0] + cp[i][j].rgbtRed * Gx[1][1] + cp[i][j+1].rgbtRed * Gx[1][2] +
                    cp[i+1][j-1].rgbtRed * Gx[2][0] + cp[i+1][j].rgbtRed * Gx[2][1] + cp[i+1][j+1].rgbtRed * Gx[2][2] );

            // Calculating Gy of each color in current pixel
            int bluey = (
                    cp[i-1][j-1].rgbtBlue * Gy[0][0] + cp[i-1][j].rgbtBlue * Gy[0][1] + cp[i-1][j+1].rgbtBlue * Gy[0][2] +
                    cp[i][j-1].rgbtBlue * Gy[1][0] + cp[i][j].rgbtBlue * Gy[1][1] + cp[i][j+1].rgbtBlue * Gy[1][2] +
                    cp[i+1][j-1].rgbtBlue * Gy[2][0] + cp[i+1][j].rgbtBlue * Gy[2][1] + cp[i+1][j+1].rgbtBlue * Gy[2][2] );
            int greeny = (
                    cp[i-1][j-1].rgbtGreen * Gy[0][0] + cp[i-1][j].rgbtGreen * Gy[0][1] + cp[i-1][j+1].rgbtGreen * Gy[0][2] +
                    cp[i][j-1].rgbtGreen * Gy[1][0] + cp[i][j].rgbtGreen * Gy[1][1] + cp[i][j+1].rgbtGreen * Gy[1][2] +
                    cp[i+1][j-1].rgbtGreen * Gy[2][0] + cp[i+1][j].rgbtGreen * Gy[2][1] + cp[i+1][j+1].rgbtGreen * Gy[2][2] );
            int redy = (
                    cp[i-1][j-1].rgbtRed * Gy[0][0] + cp[i-1][j].rgbtRed * Gy[0][1] + cp[i-1][j+1].rgbtRed * Gy[0][2] +
                    cp[i][j-1].rgbtRed * Gy[1][0] + cp[i][j].rgbtRed * Gy[1][1] + cp[i][j+1].rgbtRed * Gy[1][2] +
                    cp[i+1][j-1].rgbtRed * Gy[2][0] + cp[i+1][j].rgbtRed * Gy[2][1] + cp[i+1][j+1].rgbtRed * Gy[2][2] );

            
            int red, green, blue;
            // Combining both Gx and Gy as round(sqrt(Gx^2 + Gy^2)) of each color in current pixel
            blue = round(sqrt(bluex * bluex + bluey * bluey));
            green = round(sqrt(greenx * greenx + greeny * greeny));
            red = round(sqrt(redx * redx + redy * redy));

            // Capping color values at 255 if they are greater than 255
            blue = blue > 255? 255: blue;
            green = green > 255? 255: green;
            red = red > 255? 255: red;

            image[i-1][j-1].rgbtBlue = blue;
            image[i-1][j-1].rgbtGreen = green;
            image[i-1][j-1].rgbtRed = red;
        }
    }

    return;
}
