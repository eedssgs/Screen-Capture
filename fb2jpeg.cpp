# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <jpeglib.h>
# include "fb2jpeg.h"

//#include <time.h>


// Read JPEG file and get data in RGB form.
int read_JPEG_file (IMAGE_DATA * jpeg_data, char * filename)
{
	struct jpeg_decompress_struct dinfo;
	struct my_error_mgr jerr;
	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	int row_num = 0;
	if ((infile = fopen(filename, "rb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}

	// Step 1
	dinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&dinfo);
		fclose(infile);
		return 0;
	}

	jpeg_create_decompress(&dinfo);

	// Step 2
	jpeg_stdio_src(&dinfo, infile);

	// Step 3
	(void) jpeg_read_header(&dinfo, TRUE);

	// Step 4

	// Step 5
	(void) jpeg_start_decompress(&dinfo);
	row_stride = dinfo.output_width * dinfo.output_components;
	buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr) &dinfo, JPOOL_IMAGE, row_stride, 1);

	// Step 6
	jpeg_data->pixHeight = dinfo.output_height;
	jpeg_data->pixWidth = dinfo.output_width;
	while (dinfo.output_scanline < dinfo.output_height) 
	{
		(void) jpeg_read_scanlines(&dinfo, buffer, 1);
		memcpy(jpeg_data->iData + row_num * row_stride,buffer[0],row_stride);
		row_num++;
	}

	// Step 7
	(void) jpeg_finish_decompress(&dinfo);

	// Step 8: Release JPEG decompression object
	jpeg_destroy_decompress(&dinfo);

	fclose(infile);
	return 1;
}// End of read JPEG file.


// Write RGB data into JPEG file.
int write_JPEG_file (IMAGE_DATA * rgb_data, char * filename, int quality)//
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;				/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;				/* physical row width in image buffer */

	// Step 1
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	// Step 2
	if ((outfile = fopen(filename, "wb")) == NULL) 
	{
	    fprintf(stderr, "can't open %s\n", filename);
	    return 0;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	// Step 3
	cinfo.image_width = rgb_data->pixWidth; 	/* image width and height, in pixels */
	cinfo.image_height = rgb_data->pixHeight;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	// Step 4
	jpeg_start_compress(&cinfo, TRUE);

	// Step 5
	row_stride = rgb_data->pixWidth * 3;	/* JSAMPLEs per row in image_buffer */
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could pass
		 * more than one scanline at a time if that's more convenient.
		 */
		row_pointer[0] = & rgb_data->iData[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// Step 6
	jpeg_finish_compress(&cinfo);
 	fclose(outfile);

	// Step 7: release JPEG compression object 
	jpeg_destroy_compress(&cinfo);

	return 1;
}// End of wirte JPEG file.
