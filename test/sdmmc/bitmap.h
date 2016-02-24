#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "bwos.h"
#include "ff.h"

class Bitmap {

	typedef struct tagBITMAPFILEHEADER {
	  WORD    bfType;
	  DWORD   bfSize;
	  WORD    bfReserved1;
	  WORD    bfReserved2;
	  DWORD   bfOffBits;
	} __attribute__((packed)) BITMAPFILEHEADER;

	typedef struct tagBITMAPINFOHEADER {
	  DWORD  biSize;
	  LONG   biWidth;
	  LONG   biHeight;
	  WORD   biPlanes;
	  WORD   biBitCount;
	  DWORD  biCompression;
	  DWORD  biSizeImage;
	  LONG   biXPelsPerMeter;
	  LONG   biYPelsPerMeter;
	  DWORD  biClrUsed;
	  DWORD  biClrImportant;
	} __attribute__((packed)) BITMAPINFOHEADER;

	typedef	struct {
		BITMAPFILEHEADER	f;
		BITMAPINFOHEADER	i;
	} __attribute__((packed)) BITMAPFILE;

public:
	Bitmap() : m_image(NULL), m_is_loaded(false) {}

	void loadFile(const char* fname) {
		FIL fil;
		FILINFO stat;
		unsigned int bytes;
		f_stat(fname, &stat);
		void* buff = sys_malloc_align(stat.fsize+128, CACHE_LINE_SIZE);
		f_open(&fil, fname, FA_READ);
		f_read(&fil, buff, stat.fsize, &bytes);
		f_close(&fil);
		if ( m_image ) {
			sys_free(m_image);
		}
		m_image = static_cast<BITMAPFILE*>(buff);
		m_is_loaded = true;
	}

	void drawImage(void* buff, uint32_t win_width, uint32_t win_height, uint32_t pos_x, uint32_t pos_y) {
		if ( (win_width <= pos_x) || (win_height <= pos_y) ) {
			return;
		}
		uint8_t* bits = reinterpret_cast<uint8_t*>(m_image)+m_image->f.bfOffBits;
		uint32_t* mbuff = static_cast<uint32_t*>(buff);
		uint32_t byte_count = m_image->i.biBitCount / 8;
		for ( uint32_t cur_y = pos_y; (cur_y < win_height) && ((cur_y-pos_y) < m_image->i.biHeight); cur_y++ ) {
			for ( uint32_t cur_x = pos_x; (cur_x < win_width) && ((cur_x-pos_x) < m_image->i.biWidth); cur_x++ ) {
				uint8_t* dots = bits+(m_image->i.biHeight-1-(cur_y-pos_y))*m_image->i.biWidth*byte_count + (cur_x-pos_x)*byte_count;
				if ( byte_count == 4 ) {
					dots++;
				}
				*(mbuff+cur_y*win_width+cur_x) = makeDots(dots[0], dots[1], dots[2]);
			}
		}
	}

protected:
	uint32_t makeDots(uint32_t r, uint32_t g, uint32_t b) {
		return (b<<16)|(g<<8)|r;
	}

	BITMAPFILE*	m_image;
	bool		m_is_loaded;
};




#endif /* _BITMAP_H_ */
