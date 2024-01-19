#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_common.h"
#include "rgb_common.h"
#include "rgb_fusion.h"
#include "rgb888_source.h"

int main(int argc, char *argv[])
{
    unsigned char *bmp_buf = NULL;
    bitmap_file_info_t bmp_file;
    bitmap_image_info_t bmp_image;
    rgb_target_img_t target_img;
    rgb_icon_t icons[] = {
        {"num0", rgb111_20x30_num0, sizeof(rgb111_20x30_num0), ARGB1111, 20, 30},
        {"num1", rgb111_20x30_num1, sizeof(rgb111_20x30_num1), ARGB1111, 20, 30},
        {"num2", rgb111_20x30_num2, sizeof(rgb111_20x30_num2), ARGB1111, 20, 30},
        {"num3", rgb111_20x30_num3, sizeof(rgb111_20x30_num3), ARGB1111, 20, 30},
        {"num4", rgb111_20x30_num4, sizeof(rgb111_20x30_num4), ARGB1111, 20, 30},
        {"num5", rgb111_20x30_num5, sizeof(rgb111_20x30_num5), ARGB1111, 20, 30},
        {"num6", rgb111_20x30_num6, sizeof(rgb111_20x30_num6), ARGB1111, 20, 30},
        {"num7", rgb111_20x30_num7, sizeof(rgb111_20x30_num7), ARGB1111, 20, 30},
        {"num8", rgb111_20x30_num8, sizeof(rgb111_20x30_num8), ARGB1111, 20, 30},
        {"num9", rgb111_20x30_num9, sizeof(rgb111_20x30_num9), ARGB1111, 20, 30},
        {"minus", rgb111_20x30_minus, sizeof(rgb111_20x30_minus), ARGB1111, 20, 30},
        {"colon", rgb111_20x30_colon, sizeof(rgb111_20x30_colon), ARGB1111, 20, 30},
        {"xing", rgb111_20x30_xing, sizeof(rgb111_20x30_xing), ARGB1111, 20, 30},
        {"qi", rgb111_20x30_qi, sizeof(rgb111_20x30_qi), ARGB1111, 20, 30},
        {"ri", rgb111_20x30_ri, sizeof(rgb111_20x30_ri), ARGB1111, 20, 30},
        {"yin", rgb111_20x30_yin, sizeof(rgb111_20x30_yin), ARGB1111, 20, 30},
        {"centigrade", rgb111_30x30_centigrade, sizeof(rgb111_30x30_centigrade), ARGB1111, 30, 30},
    };

    bmp_buf = bitmap_file_read(argv[1], &bmp_file, &bmp_image);

    target_img.data = bmp_buf;
    target_img.data_len = bmp_image.biSizeImage;
    target_img.type = ARGB1111;
    target_img.height = bmp_image.biHeight;
    target_img.width = bmp_image.biWidth;

    unsigned char *ret_buf = argb1111_fusion_to_rgb888(target_img, icons[atoi(argv[2])], 10, 10, 2);
    bitmap_file_write("test.bmp", &bmp_file, &bmp_image, ret_buf);
}