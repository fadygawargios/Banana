#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <openssl/aes.h>
#include <openssl/rand.h>

#define RED_COLOR 1
#define GREEN_COLOR 2
#define BLUE_COLOR 3

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


BYTE aes_key[AES_BLOCK_SIZE] = {0,23,3,4,15,6,7,28,9,10,113,12,13,14,15,16};
BYTE iv[AES_BLOCK_SIZE] = {2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32};

//Function Prototypes
static int get_blur(int pxl_row, int pxl_col, int height, int width, 
                    RGBTRIPLE image[height][width], int COLOR);

void test_aes(void) 
{
    AES_KEY enc_key, dec_key;
    //                           1234567890123456
    unsigned char plaintext[AES_BLOCK_SIZE]; //Set to 16 bytes
    unsigned char ciphertext[AES_BLOCK_SIZE]; //Set to 16 bytes
    unsigned char decryptText[AES_BLOCK_SIZE]; //Set to 16 bytes
    int i;


    AES_set_encrypt_key(aes_key, 128, &enc_key); 
    AES_set_decrypt_key(aes_key, 128, &dec_key);

    printf("Encryption Key: ");
    for (i = 0; i < AES_BLOCK_SIZE/4; i++) {
        printf("%X ",enc_key.rd_key[i]);
    }
    printf("\nDecryption Key: ");
    for (i = 0; i < AES_BLOCK_SIZE/4; i++) {
        printf("%X ",dec_key.rd_key[i]);
    }
    
    
    plaintext[10] = 'a';
    plaintext[11] = 0x5;
    plaintext[12] = 0x5;
    plaintext[13] = 0x5;
    plaintext[14] = 0x5;
    plaintext[15] = 0x5;
    printf("\nplaintext: ");
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",plaintext[i]);
    }
    printf("\nCiphertext: ");
    AES_encrypt(&plaintext[0], &ciphertext[0], &enc_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",ciphertext[i]);
    }

    printf("\ndecryText: ");
    AES_decrypt(&ciphertext[0], &decryptText[0], &dec_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",decryptText[i]);
    }    
    printf("\nplaintext: ");
    AES_decrypt(&ciphertext[0], &decryptText[0], &dec_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",plaintext[i]);
    }
    printf("\n");

    
    plaintext[0] = 'a';
    printf("\nplaintext: ");
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",plaintext[i]);
    }
    printf("\nCiphertext: ");
    AES_encrypt(&plaintext[0], &ciphertext[0], &enc_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",ciphertext[i]);
    }

    printf("\ndecryText: ");
    AES_decrypt(&ciphertext[0], &decryptText[0], &dec_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",decryptText[i]);
    }    
    printf("\nplaintext: ");
    AES_decrypt(&ciphertext[0], &decryptText[0], &dec_key);
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%X ",plaintext[i]);
    }
    printf("\n");
}

void encrypt(int height, int width, RGBTRIPLE image[height][width], int mode) 
{
    /* generate a key with a given length */
    AES_KEY enc_key, dec_key;
    unsigned char plaintext[AES_BLOCK_SIZE]; //Set to 16 bytes
    unsigned char ciphertext[AES_BLOCK_SIZE]; //Set to 16 bytes
    
    BYTE *byte = (BYTE*)&image[0][0]; 
    int total_pixels = height*width*3;
    int i = 0;

    AES_set_encrypt_key(aes_key, 128, &enc_key); 
    AES_set_decrypt_key(aes_key, 128, &dec_key);

    for (int pxl = 0; pxl < total_pixels; pxl += MIN(AES_BLOCK_SIZE, total_pixels-pxl)) {
        for (i = 0; i < AES_BLOCK_SIZE; i++) {
            if (total_pixels - pxl < i) {
                plaintext[i] = 0;
            }
            else {
                plaintext[i] = *(byte+i);
            }
        }


        if (mode == 0 ) {
            AES_ecb_encrypt(&plaintext[0], &ciphertext[0], &enc_key, AES_ENCRYPT);
        }
        else if (mode == 1) {
		    AES_cbc_encrypt(&plaintext[0], &ciphertext[0], AES_BLOCK_SIZE, &enc_key, iv, AES_ENCRYPT);
        }

        for (i = 0; i < MIN(AES_BLOCK_SIZE, total_pixels-pxl); i++) {
            *byte = ciphertext[i];
            byte++;
        }
    }
    return;
}


void decrypt(int height, int width, RGBTRIPLE image[height][width], int mode) 
{
    /* generate a key with a given length */
    AES_KEY enc_key, dec_key;
    unsigned char plaintext[AES_BLOCK_SIZE]; //Set to 16 bytes
    unsigned char ciphertext[AES_BLOCK_SIZE]; //Set to 16 bytes

    BYTE *byte = (BYTE*)&image[0][0]; 
    int total_pixels = height*width*3;
    int i = 0;

    AES_set_encrypt_key(aes_key, 128, &enc_key); 
    AES_set_decrypt_key(aes_key, 128, &dec_key);

    for (int pxl = 0; pxl < total_pixels; pxl += MIN(AES_BLOCK_SIZE, total_pixels-pxl)) {
        for (i = 0; i < AES_BLOCK_SIZE; i++) {
            if (total_pixels - pxl < i) {
                ciphertext[i] = 0;
            }
            else {
                ciphertext[i] = *(byte+i);
            }
        }
        
        if (mode == 0 ) {
            AES_ecb_encrypt(&ciphertext[0], &plaintext[0], &dec_key, AES_DECRYPT);
        }
        else if (mode == 1) {
            AES_cbc_encrypt(&ciphertext[0], &plaintext[0], AES_BLOCK_SIZE, &enc_key, iv, AES_DECRYPT);
        }

        for (i = 0; i < MIN(AES_BLOCK_SIZE, total_pixels-pxl); i++) {
            *byte = plaintext[i];
            byte++;
        }
    }
    return;
}

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
            image[row][column].rgbtRed = get_blur(row, column, height, width, copy, RED_COLOR);
            image[row][column].rgbtGreen = get_blur(row, column, height, width, copy, GREEN_COLOR);
            image[row][column].rgbtBlue = get_blur(row, column, height, width, copy, BLUE_COLOR);
        }
    }
    return;
}

static int get_blur(int pxl_row, int pxl_col, int height, int width, RGBTRIPLE image[height][width], int COLOR)
{
    int average = 0;
    int total = 0;
    int sum = 0;

    for (int row = pxl_row - 1; row <= (pxl_row + 1); row++)
    {
        if (row < 0 || row >= height) 
        {
            continue;
        }
        for (int column = pxl_col - 1; column <= (pxl_col + 1); column++)
        {
            if (column < 0 || column >= width)
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
    if (total != 0) {
        average = round(sum / total);
    }
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
