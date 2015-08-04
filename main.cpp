#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "fb2jpeg.h"
#include "fb2bmp.h"
#include "getfb.h"

int main()
{

	// Init parameter
	IMAGE_DATA * sdata = (IMAGE_DATA *)malloc(sizeof(IMAGE_DATA)); // Screen data
	IMAGE_DATA * cdata = (IMAGE_DATA *)malloc(sizeof(IMAGE_DATA)); // Cut data
	int 		height = 768;
	int 		width  = 1376;
	int 		flag   = 0;
	sdata->iData       = (unsigned char *)malloc(height * width * 4);
	cdata->iData       = (unsigned char *)malloc(height * 1366 * 3);
	clock_t start_time;
	clock_t end_time;

	printf("\n");
	printf("============================================================================\n");
	// get fb
	start_time = clock();
	flag = GetFramebuffer(sdata);
	if (!flag)
	{
		printf("Get failed!\n");
		return 0;
	}
	end_time=clock();
	printf(" Get fb Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间
	// cut data
	start_time = clock();
	flag = CutFbData(sdata, cdata);
	if (!flag)
	{
		printf("Cut failed!\n");
		return 0;
	}
	end_time=clock();
	printf(" Cut data Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间
	// Save data as jpeg
	start_time = clock();
	flag = write_JPEG_file(cdata, "/data/eeds/save1.jpeg", 90);
	if (!flag)
	{
		printf("Save failed!\n");
		return 0;
	}
	end_time=clock();
	printf(" Save jpeg Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间
	// Read jpeg
	start_time = clock();
	flag = read_JPEG_file(sdata, "/data/eeds/save1.jpeg");
	if (!flag)
	{
		printf("Read failed!\n");
		return 0;
	}
	end_time=clock();
	printf(" Read jpeg Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间
	// Save data as bmp
	start_time = clock();
	flag = SaveBMP(cdata, "/data/eeds/save2.bmp");
	if (!flag)
	{
		printf("Save failed!\n");
		return 0;
	}
	end_time=clock();
	printf(" Save bmp Running time is: %lf ms\n",static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000);//输出运行时间

	// Free memory
	free(sdata->iData);
	free(cdata->iData);
	free(sdata);
	free(cdata);
	printf("****************************************************************************\n");
	printf("\n");
	return 1;
}
