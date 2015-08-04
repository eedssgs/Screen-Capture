# include "getfb.h"

// Get whole screen data from fb.
int GetFramebuffer(IMAGE_DATA * pData)
{
	//clock_t start_time;
	//clock_t end_time;
	// Init parameters.
	unsigned char * fb_data;
	struct 		fb_var_screeninfo vinfo;
//	struct 		fb_fix_screeninfo finfo;
	long 		fb_size = 0;
	int 		fd = 0;
	int 		num = 0;
	int 		i = 0;

	// Open the framebuffer.
	fd = open("/dev/graphics/fb0", O_RDWR | O_NONBLOCK, 0); 
	if (-1 == fd)
	{
		return 0; // Open framebuffer failed.
	}
	// Get the framebuffer var information.
	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
	{	
		return 0;
	}
	// Map the into memmory.
	//fb_size = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	//fb_data = (unsigned char *)mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// Get the frame data.
	pData->pixHeight = vinfo.yres;
	pData->pixWidth = vinfo.xres_virtual;
	pData->pixChannel = vinfo.bits_per_pixel / 8;
	num = vinfo.xres_virtual * pData->pixChannel;
	//start_time = clock();
	fb_size = vinfo.yres * num;
	// Updata framebuffer var information.
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	// Map fb data into memory
	fb_data = (unsigned char *)mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, vinfo.yoffset * num);

	memcpy(pData->iData, fb_data, fb_size);
	if (-1 == munmap(fb_data, fb_size))
	{
		return 0;// Unmap failed.
	}
	printf("i + 1 = %d \n",i+1);	
	//end_time = clock();
	//printf(" mcpy mmap Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间
	
	// Unmap the fb_tmp.
	if (-1 == munmap(fb_data, fb_size))
	{
		return 0;// Unmap failed.
	}
	close(fd);
	return 1; // Success.
} // End of GetFramebuffer.


// Cut the fb data to satisfy the needs.
int CutFbData(IMAGE_DATA * src, IMAGE_DATA * dest)
{
	int m = 0;
	int n = 0;
	int i = 0;
	int j = 0;
	dest->pixChannel = 3;
	dest->pixHeight = 768;
	dest->pixWidth = 1366;
	for (i = 0; i < dest->pixHeight - 1; i++)
	{
		for (j = 0; j < dest->pixWidth; j++)
		{
			m = (i * src->pixWidth + j) * src->pixChannel;
			n = (i * dest->pixWidth + j) * dest->pixChannel;
			dest->iData[n] = src->iData[m];
			dest->iData[n + 1] = src->iData[m + 1];
			dest->iData[n + 2] = src->iData[m + 2];
		}
	}
	return 1;
} // End of CutFbData.
