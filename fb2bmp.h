// -----------------------------------------------------------
// Name			:screenshot.h
// Description	:Declare the structure of fb data structure
//				 and BMP file header information.
// History		:2015.01.16. By eeds.
// -----------------------------------------------------------
#ifndef FB2BMP_H_
#define FB2BMP_H_

# include "getfb.h"

// BMP file header using 14 byte.
typedef struct
{
	//unsigned short bfType; // Write the file type individually for byte alignment. 
	unsigned long  	bfSize;
	unsigned short 	bfReserved1;
	unsigned short 	bfReserved2;
	unsigned long  	bfOffBits;
}BitmapFileHeader; 

// BMP information header using 40 byte.
typedef struct
{
	unsigned long	biSize;
	long			biWidth;
	long			biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned long	biCompress;
	unsigned long	biSizeImage;
	long			biXPelsPerMeter;
	long			biYPelsPerMeter;
	unsigned long	biClrUsed;
	unsigned long	biClrImportant;
}BitmapInfoHeader;

// BMP file color list.
/*typedef struct
{
	unsigned char	rgbBlue;
	unsigned char	rgbGreen;
	unsigned char	rgbRed;
	unsigned char	rgbReserved;
}RGBQuad;*/ // for 8 bit BMP.

// Load data in BMP file into memory.
int LoadBMP(IMAGE_DATA * pData, char * pPath);

// Save fb data as BMP file.
int SaveBMP(IMAGE_DATA * pData, char * pPath);

#endif
