#include "helpers.h"
#include <math.h>

int threshold(int value)
{
    if (value > 255)
    {
        value = 255;
    }
    return value;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Find the average value from the scanned pixel
            int rgb_average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);
            
            image[i][j].rgbtBlue = rgb_average;
            image[i][j].rgbtGreen = rgb_average;
            image[i][j].rgbtRed = rgb_average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Creat a placeholder for the switching
    int placeholder;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            placeholder = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][(width - 1) - j].rgbtBlue;
            image[i][(width - 1) - j].rgbtBlue = placeholder;

            placeholder = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][(width - 1) - j].rgbtGreen;
            image[i][(width - 1) - j].rgbtGreen = placeholder;

            placeholder = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][(width - 1) - j].rgbtRed;
            image[i][(width - 1) - j].rgbtRed = placeholder;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur_image[height][width];

    // Iteration through image pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int b_sum = 0;
            int g_sum = 0;
            int r_sum = 0;
            float divider = 0;

            // loop whithin 3x3 box
            for (int x = -1; x < 2; x ++)
            {
                for (int y = -1; y < 2; y ++)
                {
                    // if pixel [x][y] from box overflows the image
                    if (!(i + x > height - 1 || i + x < 0 || j + y > width - 1 || j + y < 0))
                    {
                        b_sum += image[i + x][j + y].rgbtBlue;
                        g_sum += image[i + x][j + y].rgbtGreen;
                        r_sum += image[i + x][j + y].rgbtRed;

                        divider++;
                    }
                }
            }

            blur_image[i][j].rgbtBlue = round(b_sum / divider);
            blur_image[i][j].rgbtGreen = round(g_sum / divider);
            blur_image[i][j].rgbtRed = round(r_sum / divider);

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = blur_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = blur_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = blur_image[i][j].rgbtRed;
        }
    }

}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create Array that stores processed image
    RGBTRIPLE edge_image[height][width];
    
    // Kernel
    const int GX[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    const int GY[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // Iteration through image pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int b_sumX = 0, b_sumY = 0;
            int g_sumX = 0, g_sumY = 0;
            int r_sumX = 0, r_sumY = 0;

            // loop whithin 3x3 box
            for (int x = -1; x < 2; x ++)
            {
                for (int y = -1; y < 2; y ++)
                {
                    // if pixel [x][y] from box overflows the image
                    if (!(i + x > height - 1 || i + x < 0 || j + y > width - 1 || j + y < 0))
                    {
                        b_sumX += image[i + x][j + y].rgbtBlue * GX[x + 1][y + 1];
                        g_sumX += image[i + x][j + y].rgbtGreen * GX[x + 1][y + 1];
                        r_sumX += image[i + x][j + y].rgbtRed * GX[x + 1][y + 1];

                        b_sumY += image[i + x][j + y].rgbtBlue * GY[x + 1][y + 1];
                        g_sumY += image[i + x][j + y].rgbtGreen * GY[x + 1][y + 1];
                        r_sumY += image[i + x][j + y].rgbtRed * GY[x + 1][y + 1];
                    }
                }
            }

            edge_image[i][j].rgbtBlue = threshold(round(sqrt(b_sumX * b_sumX + b_sumY * b_sumY)));
            edge_image[i][j].rgbtGreen = threshold(round(sqrt(g_sumX * g_sumX + g_sumY * g_sumY)));
            edge_image[i][j].rgbtRed = threshold(round(sqrt(r_sumX * r_sumX + r_sumY * r_sumY)));

        }
    }
    
    // Overwrite original image with the processed imaged
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = edge_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = edge_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = edge_image[i][j].rgbtRed;
        }
    }

}