#ifndef FB2JPEG_H_
#define FB2JPEG_H_

# include <jpeglib.h>
# include <setjmp.h>
# include "getfb.h"

struct my_error_mgr 
{
	struct jpeg_error_mgr pub;	
	jmp_buf setjmp_buffer;	
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{

  my_error_ptr myerr = (my_error_ptr) cinfo->err;


  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
};

int read_JPEG_file (IMAGE_DATA * jpeg_data, char * filename);

int write_JPEG_file (IMAGE_DATA * rgb_data, char * filename, int quality);// quality : 0-100

#endif
