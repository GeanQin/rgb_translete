#ifndef __BMP_UTIL_H__
#define __BMP_UTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct
{
    char bfType[3];         // 标识，就是"BM"二字
    uint32_t bfSize;        // 整个BMP文件的大小
    uint32_t bfReserved;    // 保留字，没用
    uint32_t bfOffBits;     // 偏移数，即 位图文件头+位图信息头+调色板 的大小
}bitmap_file_info_t;

typedef struct
{
    uint32_t biSize;            // 位图信息头的大小，为40
    uint32_t biWidth;           // 位图的宽度，单位是像素
    uint32_t biHeight;          // 位图的高度，单位是像素
    uint16_t biPlanes;          // 固定值1
    uint16_t biBitCount;        // 每个像素的位数，1555就是16，888就是24
    uint32_t biCompression;     // 压缩方式，BI_RGB(0)为不压缩
    uint32_t biSizeImage;       // 位图全部像素占用的字节数，BI_RGB时可设为0
    uint32_t biXPelsPerMeter;   // 水平分辨率(像素/米)
    uint32_t biYPelsPerMeter;   // 垂直分辨率(像素/米)
    uint32_t biClrUsed;         // 位图使用的颜色数，如果为0，则颜色数为2的biBitCount次方
    uint32_t biClrImportant;    // 重要的颜色数，0代表所有颜色都重要
}bitmap_image_info_t;

/*
 * bitmap_file_read:
 * 读取bmp文件。
 * @param[in]
 *      filename        文件名称。
 * @param[out]
 *      bitmap_file     文件信息。
 * @param[out]
 *      bitmap_image    图像信息。
 * @retval
 *      bmp文件中图片数据，此时的数据每行4字节对齐且倒立，buff需要free。
 *      失败时为NULL。
 */
uint8_t *bitmap_file_read(char *filename, bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image);
/*
 * bitmap_print_head:
 * 打印头部信息。
 * @param[in]
 *      bitmap_file     文件信息。
 * @param[in]
 *      bitmap_image    图像信息。
 */
void bitmap_print_head(bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image);
/*
 * bitmap_real_data:
 * 获取真实数据。
 * @param[in]
 *      bitmap_image    图像信息。
 * @param[in]
 *      buff            文件中读到的图像数据。
 * @param[in]
 *      if_print        是否打印。
 * @retval
 *      真实图片数据，传进来的buff会被free，返回新的buff地址。
 *      失败时为NULL。
 */
uint8_t *bitmap_real_data(bitmap_image_info_t *bitmap_image, uint8_t *buff, int if_print);
/*
 * bitmap_real_data:
 * 格式化要写入的数据。
 * @param[in]
 *      bitmap_image    图像信息。
 * @param[in]
 *      buff            图像的真实数据。
 * @retval
 *      格式化后的图片数据，传进来的buff会被free，返回新的buff地址。
 *      失败时为NULL。
 */
uint8_t *bitmap_format_data(bitmap_image_info_t *bitmap_image, uint8_t *buff);
/*
 * bitmap_file_write:
 * 格式化要写入的数据。
 * @param[in]
 *      filename        输出文件名。
 * @param[in]
 *      bitmap_file     文件信息。
 * @param[in]
 *      bitmap_image    图像信息。
 * @param[in]
 *      buff            图像数据。
 * @retval
 *      0：             成功。
 *      -1：            失败。
 */
int bitmap_file_write(char *filename, bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image, uint8_t *buff);


#ifdef __cplusplus
}
#endif

#endif