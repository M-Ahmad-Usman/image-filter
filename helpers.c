#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <omp.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double start_time = omp_get_wtime();

    BYTE shade;

    #pragma omp parallel for collapse(2) schedule(static) private(shade)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            shade = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            RGBTRIPLE grey = {shade, shade, shade};
            image[i][j] = grey;
        }
    }

    double end_time = omp_get_wtime();
    printf("Grayscale processing time: %f seconds\n", end_time - start_time);
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    double start_time = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE oRed = image[i][j].rgbtRed;
            BYTE oGreen = image[i][j].rgbtGreen;
            BYTE oBlue = image[i][j].rgbtBlue;

            WORD sRed = round((0.393 * oRed) + (0.769 * oGreen) + (0.189 * oBlue));
            WORD sGreen = round((0.349 * oRed) + (0.686 * oGreen) + (0.168 * oBlue));
            WORD sBlue = round((0.272 * oRed) + (0.534 * oGreen) + (0.131 * oBlue));

            sBlue = (sBlue > 255) ? 255 : sBlue;
            sGreen = (sGreen > 255) ? 255 : sGreen;
            sRed = (sRed > 255) ? 255 : sRed;

            RGBTRIPLE sepianPixel = {sBlue, sGreen, sRed};

            image[i][j] = sepianPixel;
        }
    }

    double end_time = omp_get_wtime();
    printf("Sepia processing time: %f seconds\n", end_time - start_time);
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    double start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];

            image[i][j] = image[i][width - 1 - j];

            image[i][width - 1 - j] = temp;
        }
    }

    double end_time = omp_get_wtime();
    printf("Reflect processing time: %f seconds\n", end_time - start_time);
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    double start_time = omp_get_wtime();

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    RGBTRIPLE cp[height + 2][width + 2];
    RGBTRIPLE outsideBorder = {0, 0, 0};

    #pragma omp parallel for
    for (int i = 0; i < width + 2; i++)
    {
        cp[0][i] = outsideBorder;
        cp[height + 1][i] = outsideBorder;
    }

    #pragma omp parallel for
    for (int i = 0; i < height + 2; i++)
    {
        cp[i][0] = outsideBorder;
        cp[i][width + 1] = outsideBorder;
    }

    #pragma omp parallel for collapse(2)
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            cp[i][j] = image[i - 1][j - 1];
        }
    }

    #pragma omp parallel for collapse(2)
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int bluex = (
                cp[i - 1][j - 1].rgbtBlue * Gx[0][0] + cp[i - 1][j].rgbtBlue * Gx[0][1] + cp[i - 1][j + 1].rgbtBlue * Gx[0][2] +
                cp[i][j - 1].rgbtBlue * Gx[1][0] + cp[i][j].rgbtBlue * Gx[1][1] + cp[i][j + 1].rgbtBlue * Gx[1][2] +
                cp[i + 1][j - 1].rgbtBlue * Gx[2][0] + cp[i + 1][j].rgbtBlue * Gx[2][1] + cp[i + 1][j + 1].rgbtBlue * Gx[2][2]
            );
            int greenx = (
                cp[i - 1][j - 1].rgbtGreen * Gx[0][0] + cp[i - 1][j].rgbtGreen * Gx[0][1] + cp[i - 1][j + 1].rgbtGreen * Gx[0][2] +
                cp[i][j - 1].rgbtGreen * Gx[1][0] + cp[i][j].rgbtGreen * Gx[1][1] + cp[i][j + 1].rgbtGreen * Gx[1][2] +
                cp[i + 1][j - 1].rgbtGreen * Gx[2][0] + cp[i + 1][j].rgbtGreen * Gx[2][1] + cp[i + 1][j + 1].rgbtGreen * Gx[2][2]
            );
            int redx = (
                cp[i - 1][j - 1].rgbtRed * Gx[0][0] + cp[i - 1][j].rgbtRed * Gx[0][1] + cp[i - 1][j + 1].rgbtRed * Gx[0][2] +
                cp[i][j - 1].rgbtRed * Gx[1][0] + cp[i][j].rgbtRed * Gx[1][1] + cp[i][j + 1].rgbtRed * Gx[1][2] +
                cp[i + 1][j - 1].rgbtRed * Gx[2][0] + cp[i + 1][j].rgbtRed * Gx[2][1] + cp[i + 1][j + 1].rgbtRed * Gx[2][2]
            );

            int bluey = (
                cp[i - 1][j - 1].rgbtBlue * Gy[0][0] + cp[i - 1][j].rgbtBlue * Gy[0][1] + cp[i - 1][j + 1].rgbtBlue * Gy[0][2] +
                cp[i][j - 1].rgbtBlue * Gy[1][0] + cp[i][j].rgbtBlue * Gy[1][1] + cp[i][j + 1].rgbtBlue * Gy[1][2] +
                cp[i + 1][j - 1].rgbtBlue * Gy[2][0] + cp[i + 1][j].rgbtBlue * Gy[2][1] + cp[i + 1][j + 1].rgbtBlue * Gy[2][2]
            );
            int greeny = (
                cp[i - 1][j - 1].rgbtGreen * Gy[0][0] + cp[i - 1][j].rgbtGreen * Gy[0][1] + cp[i - 1][j + 1].rgbtGreen * Gy[0][2] +
                cp[i][j - 1].rgbtGreen * Gy[1][0] + cp[i][j].rgbtGreen * Gy[1][1] + cp[i][j + 1].rgbtGreen * Gy[1][2] +
                cp[i + 1][j - 1].rgbtGreen * Gy[2][0] + cp[i + 1][j].rgbtGreen * Gy[2][1] + cp[i + 1][j + 1].rgbtGreen * Gy[2][2]
            );
            int redy = (
                cp[i - 1][j - 1].rgbtRed * Gy[0][0] + cp[i - 1][j].rgbtRed * Gy[0][1] + cp[i - 1][j + 1].rgbtRed * Gy[0][2] +
                cp[i][j - 1].rgbtRed * Gy[1][0] + cp[i][j].rgbtRed * Gy[1][1] + cp[i][j + 1].rgbtRed * Gy[1][2] +
                cp[i + 1][j - 1].rgbtRed * Gy[2][0] + cp[i + 1][j].rgbtRed * Gy[2][1] + cp[i + 1][j + 1].rgbtRed * Gy[2][2]
            );

            int blue = round(sqrt(bluex * bluex + bluey * bluey));
            int green = round(sqrt(greenx * greenx + greeny * greeny));
            int red = round(sqrt(redx * redx + redy * redy));

            blue = blue > 255 ? 255 : blue;
            green = green > 255 ? 255 : green;
            red = red > 255 ? 255 : red;

            image[i - 1][j - 1].rgbtBlue = blue;
            image[i - 1][j - 1].rgbtGreen = green;
            image[i - 1][j - 1].rgbtRed = red;
        }
    }

    double end_time = omp_get_wtime();
    printf("Edges processing time: %f seconds\n", end_time - start_time);
}