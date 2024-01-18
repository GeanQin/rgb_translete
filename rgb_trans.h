#ifndef __RGB_TRANS_H__
#define __RGB_TRANS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

uint16_t *rgb888_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h);
uint8_t *rgb888_to_argb1111(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h);
uint16_t *string_to_argb1555(char *ttf_path, char *str, uint32_t font_size, uint32_t *w, uint32_t *h);

#ifdef __cplusplus
}
#endif

#endif