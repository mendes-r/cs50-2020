#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2 || argc > 2)
    {
        printf("It only accepts one argument!\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    // Check for lost file
    if (file == NULL)
    {
        printf("File not found!\n");
        return 1;
    }

    // Read 8bits (1Byte) at a time until buffer is full (512 Bytes)
    BYTE *buffer = malloc(sizeof(BYTE) * 512);

    FILE *new_file = NULL;

    // Start a file count
    int file_count = 0;

    // If fread doesnt read a chunck of 512 BYTEs is because there is nothing more to read
    while (fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        char file_str [10];

        if (file_count < 10)
        {
            sprintf(file_str, "00%i.jpg", file_count);
        }

        else
        {
            sprintf(file_str, "0%i.jpg", file_count);
        }

        // Find out if it is a new file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >= 0xe0) && (buffer[3] <= 0xef))
        {
            if (new_file != NULL)
            {
                fclose(new_file);
            }

            new_file = fopen(file_str, "w");

            // Check if a new file was succefully created
            if (new_file == NULL)
            {
                printf("File wasn't saved!\n");
                return 1;
            }

            fwrite(buffer, sizeof(BYTE), 512, new_file);
            file_count ++;
        }

        else
        {
            // Check if a new file exists
            if (new_file != NULL)
            {
                fwrite(buffer, sizeof(BYTE), 512, new_file);
            }
        }
    }

    free(buffer);
    fclose(file);
}