#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    //Open the memory card and create FAT32 buffer
    FILE *card = fopen(argv[1], "r");
    BYTE buffer[513];
    
    //read data on mem card
    int no_read = fread(buffer, 1, 512, card);
    
    //Image name string decleration
    char name[10];
    
    int flag = 0;
    int previous_flag = 0;
    
    //Output image pointer
    FILE *img = NULL;
    
    //Iterating through every image data
    while (no_read == 512)
    {
        for (int i = 0; i < 512; i++)
        {
            //Finding the start of each image
            if (buffer[i] == 0xff && buffer[i + 1] == 0xd8 && buffer[i + 2] == 0xff && (buffer[i + 3] & 0xf0) == 0xe0)
            {
                flag++;
            }
            
        }
        
        //Check and write the image to output file
        if (flag == 0)
        {
            ;
        }
        else if (previous_flag != flag)
        {
            previous_flag = flag;
            sprintf(name, "%03i.jpg", flag - 1);
            img = fopen(name, "w");
            fwrite(buffer, 1, 512, img);
        }
        else
        {
            fwrite(buffer, 1, 512, img);
        }
        no_read = fread(buffer, 1, 512, card);
    }
    return 0;

}