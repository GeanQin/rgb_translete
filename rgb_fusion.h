#ifndef __RGB_FUSION_H__
#define __RGB_FUSION_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    const unsigned char *data;
    unsigned long data_len;
    unsigned int width;
    unsigned int height;
}source_image_t;

typedef struct
{
    unsigned char *data;
    unsigned long data_len;
    unsigned int width;
    unsigned int height;
}bg_image_t;

/*
 * rgb888_image_fusion：
 * 简单叠加两张rgb888的图。
 * @param[in]
 *      bg_img          底图
 * @param[in]
 *      source_img      上图
 * @param[in]
 *      x               x坐标
 * @param[in]
 *      y               y坐标
 * @retval
 *      叠加后的图片buff指针，就是底图bg_img的buff指针
 *      失败时为NULL
 */
unsigned char *rgb888_image_fusion(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y);
/*
 * rgb888_image_fusion_without_color：
 * 叠加两张rgb888的图，去除上图底色。
 * @param[in]
 *      color           底色，例子：0x000000（黑底）
 */
unsigned char *rgb888_image_fusion_without_color(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y, int color);
/*
 * rgb888_image_fusion_without_color：
 * 叠加两张rgb888的图，去除上图底色，并且放大上图。
 * 为了省内存和flash，如大数字和小数字，只存小数字，大数字靠小数字放大。
 * @param[in]
 *      amplify         放大倍数
 */
unsigned char *rgb888_image_fusion_without_color_ex(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y, int color, int amplify);

#ifdef __cplusplus
}
#endif

#endif