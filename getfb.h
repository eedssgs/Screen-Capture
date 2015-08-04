#ifndef GETFB_H_
#define GETFB_H_

# include <stdio.h>
# include <malloc.h>
# include <fcntl.h>
# include <unistd.h>
# include <linux/fb.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/mman.h>
//# include <time.h>

// Data structure
typedef struct
{
	int 			pixChannel;
	int 			pixHeight;
	int 			pixWidth;
	unsigned char * iData;
}IMAGE_DATA;

// Get whole screen data from fb.
int GetFramebuffer(IMAGE_DATA * pData);

// Cut the fb data to satisfy the needs.
int CutFbData(IMAGE_DATA * src, IMAGE_DATA * dest);

#endif
