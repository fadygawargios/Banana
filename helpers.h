#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

//encrypt 
void encrypt(int height, int width, RGBTRIPLE image[height][width], int mode);

//decrypt
void decrypt(int height, int width, RGBTRIPLE image[height][width], int mode);

void test_aes(void);
