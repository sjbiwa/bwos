/*
 * video.h
 *
 *  Created on: 2015/04/25
 *      Author: biwa
 */

#ifndef VIDEO_H_
#define VIDEO_H_

#include "bwos.h"

typedef	enum {
	VIDEO_LAYER_0 = 0,
	VIDEO_LAYER_1,
	VIDEO_LAYER_2,
	VIDEO_LAYER_3,
	VIDEO_LAYER_NUM,
} VideoLayer;

#define	VIDEO_IRQ_FRAME_START		(0x1u<<1)
#define	VIDEO_IRQ_FRAME_END			(0x1u<<2)
#define	VIDEO_IRQ_MASK				(VIDEO_IRQ_FRAME_START|VIDEO_IRQ_FRAME_END)

extern void video_init(void);
extern void video_layer_init(VideoLayer layer, uint32_t addr);
extern void video_layer_set(VideoLayer layer, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height, uint32_t addr, uint32_t stride);
extern void video_register_handler(void (*handler)(uint32_t event));

#endif /* VIDEO_H_ */
