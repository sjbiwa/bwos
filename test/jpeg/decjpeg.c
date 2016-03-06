#include <ctype.h>
#include "bwos.h"
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"
#include "jversion.h"

static void output_start(j_decompress_ptr cinfo)
{
}
static void output_scanline(j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION num_scanlines)
{
}
static void output_finish(j_decompress_ptr cinfo)
{
}


static void parse_switches(j_decompress_ptr cinfo)
{
	cinfo->err->trace_level = 0;
	cinfo->quantize_colors = FALSE;
	//cinfo->dct_method = JDCT_ISLOW;
	//cinfo->dct_method = JDCT_IFAST;
	cinfo->dct_method = JDCT_FLOAT;
	//cinfo->dither_mode = JDITHER_FS;
	cinfo->dither_mode = JDITHER_NONE;
	//cinfo->dither_mode = JDITHER_ORDERED;
	//cinfo->err->trace_level++; (?)
	cinfo->two_pass_quantize = FALSE;
	cinfo->do_fancy_upsampling = FALSE;
	//cinfo->out_color_space = JCS_GRAYSCALE;
	cinfo->out_color_space = JCS_RGB;
	//cinfo->mem->max_memory_to_use = lval * 1000L;
	cinfo->do_fancy_upsampling = FALSE;
	cinfo->two_pass_quantize = FALSE;
}


void jpeg_decompress(void* image, size_t image_size, void* output, uint32_t* r_width, uint32_t* r_height)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JDIMENSION num_scanlines;

	/* Initialize the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	parse_switches(&cinfo);

	/* Specify data source for decompression */
	jpeg_mem_src(&cinfo, image, image_size);

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);

	/* Adjust default decompression parameters by re-parsing the options */
	parse_switches(&cinfo);

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);

	/* Write output file header */
	*r_width = cinfo.image_width;
	*r_height = cinfo.image_height;
	output_start(&cinfo);

	/* Process data */

	while (cinfo.output_scanline < cinfo.output_height) {
		num_scanlines = jpeg_read_scanlines(&cinfo, &output, 1);
		output_scanline(&cinfo, output, num_scanlines);
		output = (uint8_t*)output + num_scanlines * 3 * cinfo.image_width;
	}

	/* Finish decompression and release memory.
	* I must do it in this order because output module has allocated memory
	* of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
	*/
	output_finish(&cinfo);
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}
