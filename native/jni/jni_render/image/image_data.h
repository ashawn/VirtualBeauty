//
// Created by 赖振奇 on 2017/3/10.
//

#ifndef VIRTUALBEAUTY_IMAGE_DATA_H
#define VIRTUALBEAUTY_IMAGE_DATA_H

#include <GLES2/gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const int width;
    const int height;
    const int size;
    const GLenum gl_color_format;
    const void *data;
} RawImageData;

/* Returns the decoded image data, or aborts if there's an error during decoding. */
RawImageData get_raw_image_data_from_png(const void *png_data, const int png_data_size);

void release_raw_image_data(const RawImageData *data);

#ifdef __cplusplus
}
#endif
#endif //VIRTUALBEAUTY_IMAGE_DATA_H
