#ifndef __RGB_UTIL_H__
#define __RGB_UTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/*
 * rgb888_to_rgb565：
 * 将rgb888转成rgb565。
 * @param[in]
 *      in_buf      输入数据
 * @param[in]
 *      in_len      输入数据长度
 * @param[in]
 *      w           图片宽度
 * @param[in]
 *      h           图片高度
 * @retval
 *      返回转好的数据
 */
uint8_t *rgb888_to_rgb565(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h);
/*
 * rgb888_to_argb1555
 * 将rgb888转成argb1555。
 * @param[in]
 *      in_buf      输入数据
 * @param[in]
 *      in_len      输入数据长度
 * @param[in]
 *      w           图片宽度
 * @param[in]
 *      h           图片高度
 * @retval
 *      返回转好的数据
 */
uint8_t *rgb888_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h);
/*
 * rgb565_to_argb1555
 * 将rgb565转成argb1555。
 * @param[in]
 *      in_buf      输入数据
 * @param[in]
 *      in_len      输入数据长度
 * @param[in]
 *      w           图片宽度
 * @param[in]
 *      h           图片高度
 * @retval
 *      返回转好的数据
 */
uint8_t *rgb565_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h);

uint8_t *string_to_argb1555(char *ttf_path, char *str, uint32_t font_size, uint32_t *w, uint32_t *h);

#ifdef __cplusplus
}
#endif

#endif