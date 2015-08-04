// -----------------------------------------------------------
// Name			:screenshot.h
// Description	:The functions that operate framebuffer, save 
//				 data into BMP and load BMP data into memory.
// History		:2015.01.16. By eeds.
// -----------------------------------------------------------

# include <stdio.h>
# include <stdlib.h>
# include <linux/fb.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/mman.h>
# include "fb2bmp.h"

int LoadBMP(IMAGE_DATA * pData, char * pPath)
{
	FILE * 				pFile;
	unsigned short 		bfType;
	BitmapFileHeader	bFileHeader;
	BitmapInfoHeader	bInfoHeader;
//	RGBQuad * 			bQuad;
	unsigned char		pixVal = '\0';
	int 				i = 0;
	int 				j = 0;
	int 				offset = 0;
	int 				num = 0;

	// Open the file.
	pFile = fopen(pPath, "rb");
	if (!pFile)
	{
		return 0;
	}
	// Read the file type.
	fread(&bfType, sizeof(unsigned short), 1, pFile); 
	// Read the file header of the BMP file.
	fread(&bFileHeader, sizeof(BitmapFileHeader), 1, pFile); 
	// Read the info header of the BMP file.
	fread(&bInfoHeader, sizeof(BitmapInfoHeader), 1, pFile); 
	// 0x4D42 means "BM".
	if (0x4D42 == bfType)
	{
		// Get the discription of BMP data.
		pData->pixChannel = 3;
		pData->pixHeight = bInfoHeader.biHeight;
		pData->pixWidth = bInfoHeader.biWidth;
		offset = (pData->pixChannel * pData->pixWidth) % 4;
		if (offset != 0)
		{
			offset = 4 - offset;
		}
		// Read the BMP data.
		for (i = pData->pixHeight - 1; i > -1; i--)
		{
			for (j = 0; j < pData->pixWidth; j++)	
			{
				num = (pData->pixWidth * i + j) * pData->pixChannel;
				// Change the color order of every pixel from BGR to RGB.
				fread(&pixVal, sizeof(unsigned char), 1, pFile);// Read blue.
				pData->iData[num + 2] = pixVal;
				fread(&pixVal, sizeof(unsigned char), 1, pFile);// Read green.
				pData->iData[num + 1] = pixVal;
				fread(&pixVal, sizeof(unsigned char), 1, pFile);// Read red.
				pData->iData[num + 0] = pixVal;
			} // End of for(j=0).
			if (offset != 0)
			{
				for (j = 0; j < offset; j++)
				{
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}// End of for().	
	}else
	{
		fclose(pFile);
		return 0;
	}
	fclose(pFile);
	return 1;
} // End of LoadBMP.

// Save fb data as BMP file.
int SaveBMP(IMAGE_DATA * pData, char * pPath)
{
	FILE * 				pFile;
	unsigned short 		bfType;
	BitmapFileHeader	bFileHeader;
	BitmapInfoHeader	bInfoHeader;
//	RGBQuad * 			bQuad;
	unsigned char		pixVal = '\0';
	int 				i = 0;
	int 				j = 0;
	int 				offset = 0;
	int 				channel = 0;
	int 				step = 0; 
	int  				num = 0;	

	// Open or create a BMP file.
	if (NULL == (pFile = fopen(pPath,"wb+")))
	{
		return 0;
	}
	// Write the file type.
	bfType	= 0x4D42;	// 0x4D42 = BM. 
	fwrite(&bfType,sizeof(bfType),1,pFile);

	// In order to be quadword aligned.
	step =  3 * pData->pixWidth;		
	offset = step % 4;
	if (offset != 0)
	{
		step += 4 - offset;
	}

	// Write the BMP file header.
	bFileHeader.bfSize	= 54 + pData->pixHeight * step;
	bFileHeader.bfReserved1	= 0;
	bFileHeader.bfReserved2	= 0;
	bFileHeader.bfOffBits	= 54;
	fwrite(&bFileHeader, sizeof(bFileHeader), 1, pFile);

	// Write the BMP info header.
	bInfoHeader.biSize	= 40;
	bInfoHeader.biWidth	= pData->pixWidth;
	bInfoHeader.biHeight	= pData->pixHeight;
	bInfoHeader.biPlanes	= 1;
	bInfoHeader.biBitCount	= 24;
	bInfoHeader.biCompress	= 0;		// No compress.
	bInfoHeader.biSizeImage	= pData->pixHeight * step;
	bInfoHeader.biClrUsed	= 0;		// Do not care.
	bInfoHeader.biClrImportant = 0; 	// Do not care.
	bInfoHeader.biXPelsPerMeter = 0;	// Do not care.
	bInfoHeader.biYPelsPerMeter = 0;	// Do not care.	 
	fwrite(&bInfoHeader, sizeof(bInfoHeader), 1, pFile);
	
	// Write the BMP image data.
	for (i = pData->pixHeight - 1; i > -1; i--) 
	{
		for (j = 0; j < pData->pixWidth; j++)
		{
			num = (i * pData->pixWidth + j) * pData->pixChannel;
			// Write blue pixel				
			pixVal = pData->iData[num + 2];
			fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			// Write green pixel
			pixVal = pData->iData[num + 1];
			fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			// Write red pixel
			pixVal = pData->iData[num + 0];
			fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
		}
		if (offset != 0)
		{
			for (j = 0; j < 4 - offset; j++)
			{
				pixVal = 0;
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}
		}
	}

	fclose(pFile);
	return 1;
} // End of SaveBMP.















