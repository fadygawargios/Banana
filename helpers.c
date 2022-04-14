#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RED_COLOR 1
#define GREEN_COLOR 2
#define BLUE_COLOR 3

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    int average = 0;

    for (int row = 0; row <  height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            average += image[row][column].rgbtRed;
            average += image[row][column].rgbtGreen;
            average += image[row][column].rgbtBlue;
            average = round(average / 3.0);

            image[row][column].rgbtRed = average;
            image[row][column].rgbtGreen = average;
            image[row][column].rgbtBlue = average;
            average = 0;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Why the +1
            reflected[row][column] = image[row][width - (column + 1)];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column] = reflected[row][column];
        }
    }
    return;
}

    // Function prototype: SEND HELP
int get_blur(int i, int j, int height, int width, RGBTRIPLE image[height][width], int COLOR)

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{


    RGBTRIPLE copy[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // BUG HERE
            image[row][column].rgbtRed = get_blur(row, column, height, width, copy[height][width], RED_COLOR);
            image[row][column].rgbtGreen = get_blur(row, column, height, width, copy[height][width], GREEN_COLOR);
            image[row][column].rgbtBlue = get_blur(row, column, height, width, copy[height][width], BLUE_COLOR);
        }
    }
    return;
}

    int get_blur(int i, int j, int height, int width, RGBTRIPLE image[height][width], int COLOR)
    {
    int average = 0;
    int total = 0;
    int sum = 0;

    for (int row = i - 1; row <= (i + 1); i++)
    {
        for (int column = j - 1; column <= (j + 1); j++)
        {
            if (row < 0 || row >= height || column < 0 || column >= width)
            {
                continue;
            }
            // If the color is red
            if (COLOR == RED_COLOR)
            {
                sum += image[row][column].rgbtRed;
            }
            // If the color is green
            else if (COLOR == GREEN_COLOR)
            {
                sum += image[row][column].rgbtGreen;
            }
            // If the color is blue
            else if (COLOR == BLUE_COLOR)
            {
                sum += image[row][column].rgbtBlue;
            }
            total++;
        }
    }

    average = round(sum / total);
    return average;
    }


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int GX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int GY[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    RGBTRIPLE edges[height][width];
    int G_counter = 0;
    int GX_RED = 0;
    int GX_GREEN = 0;
    int GX_BLUE = 0;

    int GY_RED = 0;
    int GY_GREEN = 0;
    int GY_BLUE = 0;

    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    // Bug somewhere here: use debugger
                    // Is this giving me the desired effect?
                    if ((column + i < 0) || (column + i > height))
                    {
                        G_counter++;
                        break;
                    }
                    else if ((row + j < 0) || (row + j > width))
                    {
                        G_counter++;
                        continue;
                    }
                    // GX
                    GX_RED += GX[G_counter] * image[column + i][row + j].rgbtRed;
                    GX_GREEN += GX[G_counter] * image[column + i][row + j].rgbtGreen;
                    GX_BLUE += GX[G_counter] * image[column + i][row + j].rgbtBlue;
                    // GY
                    GY_RED += GY[G_counter] * image[column + i][row + j].rgbtRed;
                    GY_GREEN += GY[G_counter] * image[column + i][row + j].rgbtGreen;
                    GY_BLUE += GY[G_counter] * image[column + i][row + j].rgbtBlue;
                    G_counter++;

                }
            }
            // Compute new pixel
            edges[column][row].rgbtRed = (int) sqrt((GX_RED * GX_RED) + (GY_RED * GY_RED));
            edges[column][row].rgbtGreen = (int) sqrt((GX_GREEN * GX_GREEN) + (GY_GREEN * GY_GREEN));
            edges[column][row].rgbtBlue = (int) sqrt((GX_BLUE * GX_BLUE) + (GY_BLUE * GY_BLUE));

            // Reset all the variables
            G_counter = 0;
            GX_RED = 0;
            GX_GREEN = 0;
            GX_BLUE = 0;

            GY_RED = 0;
            GY_GREEN = 0;
            GY_BLUE = 0;
        }
    }

    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            image[column][row] = edges[column][row];
        }
    }
    return;
}
