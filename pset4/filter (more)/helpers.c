#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = image[i][j].rgbtBlue;
            average += image[i][j].rgbtGreen;
            average += image[i][j].rgbtRed;
            average /= 3;
            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    double rounded_width = round((width - 1) / 2);
    RGBTRIPLE buffer;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width && j <= rounded_width; j++)
        {
            buffer = image[i][width - j - 1];
            image[i][width - j - 1] = image[i][j];
            image[i][j] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copying image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }
    double avg_Blue = 0;
    double avg_Green = 0;
    double avg_Red = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //resetting values for next pixel
            int add_counter = 0;
            avg_Blue = 0;
            avg_Green = 0;
            avg_Red = 0;
            
            // Iterate through rows
            for (int k = 1; k >= -1; k--)
            {
                //Iterate through columns
                for (int l = 1; l >= -1; l--)
                {
                    //check if the row is out of bounds
                    if ((i - k) == -1 || (i - k) == height)
                    {
                        continue;
                    }
                    
                    //check if the colum is out of bounds
                    if ((j - l) == -1 || (j - l) == width)
                    {
                        continue;
                    }
                    //add the indiviual values of RGB
                    add_counter++;
                    avg_Blue += image_copy[i - k][j - l].rgbtBlue;
                    avg_Green += image_copy[i - k][j - l].rgbtGreen;
                    avg_Red += image_copy[i - k][j - l].rgbtRed;
                }
            }
            
            //calculate and assign the values of averaged pixels
            if (add_counter != 0)
            {
                avg_Blue = round(avg_Blue / add_counter);
                avg_Green = round(avg_Green / add_counter);
                avg_Red = round(avg_Red / add_counter);
                image[i][j].rgbtBlue = avg_Blue;
                image[i][j].rgbtGreen = avg_Green;
                image[i][j].rgbtRed = avg_Red;
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Copy image for edge detection
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }
    
    //Kernel for x axis
    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    
    //Kernel for y axis
    int Gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    
    //Iterating through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Creating Gx and Gy sum for all colors
            double Gx_Blue = 0;
            double Gx_Green = 0;
            double Gx_Red = 0;
            double Gy_Blue = 0;
            double Gy_Green = 0;
            double Gy_Red = 0;
            double G_Blue = 0;
            double G_Green = 0;
            double G_Red = 0;
            //Iterate through rows on 3x3 grid
            for (int k = -1; k <= 1; k++)
            {
                //Iterate through colums on 3x3 grid
                for (int l = -1; l <= 1; l++)
                {
                    //check if the row is out of bounds
                    if ((i + k) == -1 || (i + k) == height)
                    {
                        continue;
                    }
                    
                    //check if the colum is out of bounds
                    if ((j + l) == -1 || (j + l) == width)
                    {
                        continue;
                    }
                    //Calc Gx values and add them
                    Gx_Blue += image_copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                    Gx_Green += image_copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                    Gx_Red += image_copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    
                    //Calc Gy values and add them
                    Gy_Blue += image_copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    Gy_Green += image_copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    Gy_Red += image_copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                }
            }
            
            //Convert the 2 axis Gx and Gy to 1 pixel value G
            G_Blue = round(sqrt(pow(Gx_Blue, 2) + pow(Gy_Blue, 2)));
            G_Green = round(sqrt(pow(Gx_Green, 2) + pow(Gy_Green, 2)));
            G_Red = round(sqrt(pow(Gx_Red, 2) + pow(Gy_Red, 2)));
            
            //Check if the sum exceeds 255
            if (G_Blue > 255)
            {
                G_Blue = 255;
            }
            if (G_Green > 255)
            {
                G_Green = 255;
            }
            if (G_Red > 255)
            {
                G_Red = 255;
            }
            image[i][j].rgbtBlue = G_Blue;
            image[i][j].rgbtGreen = G_Green;
            image[i][j].rgbtRed = G_Red;
            
        }
    }
    return;
}