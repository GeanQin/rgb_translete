#include <stdio.h>
#include <stdlib.h>

#include "bmp_trans.h"

int main(int argc, char *argv[])
{
    bitmap_file_info_t bitmap_file;
    bitmap_image_info_t bitmap_image;
    unsigned char *bitmap_buf = NULL;
    uint16_t *argb1555 = NULL;
    uint8_t *argb1111 = NULL;
    int w, h = 0;

    // bitmap_buf = bitmap_file_read("/share_data/bmp/hjq_640x360.bmp", &bitmap_file, &bitmap_image);
    bitmap_buf = bitmap_file_read("images/mijia/mijia_480p.bmp", &bitmap_file, &bitmap_image);
    printf("file type=%s size=%u reserved=%u offset=%u\n", bitmap_file.bfType, bitmap_file.bfSize, bitmap_file.bfReserved, bitmap_file.bfOffBits);
    printf("bitmap size=%u w=%u h=%u plane=%hu bitcount=%hu\n",
           bitmap_image.biSize, bitmap_image.biWidth, bitmap_image.biHeight, bitmap_image.biPlanes, bitmap_image.biBitCount);
    printf("bitmap compression=%u size image=%u x=%u y=%u color=%u import=%u\n",
           bitmap_image.biCompression, bitmap_image.biSizeImage, bitmap_image.biXPelsPerMeter, bitmap_image.biYPelsPerMeter, bitmap_image.biClrUsed, bitmap_image.biClrImportant);
    printf("bitmap_buf=%p\n", bitmap_buf);

    argb1555 = rgb888_to_argb1555(bitmap_buf, bitmap_image.biSizeImage, bitmap_image.biWidth, bitmap_image.biHeight);
#if 0   // 不知道为啥不能显示argb1111，bmp不支持？看了一下PS最小也是16位，不纠结了
    argb1111 = rgb888_to_argb1111(bitmap_buf, bitmap_image.biSizeImage, bitmap_image.biWidth, bitmap_image.biHeight);
    bitmap_file.bfSize = bitmap_file.bfOffBits + bitmap_image.biWidth * bitmap_image.biHeight / 2 + 2;
    bitmap_image.biBitCount = 4;
    bitmap_image.biSizeImage = bitmap_image.biWidth * bitmap_image.biHeight / 2 + 2;
    bitmap_file_write("test1111.bmp", bitmap_file, bitmap_image, (uint8_t *)argb1111);
#endif
    bitmap_file.bfSize = bitmap_file.bfOffBits + bitmap_image.biWidth * bitmap_image.biHeight * 2 + 2;
    bitmap_image.biBitCount = 16;
    bitmap_image.biSizeImage = bitmap_image.biWidth * bitmap_image.biHeight * 2 + 2;
    bitmap_file_write("test1555.bmp", bitmap_file, bitmap_image, (uint8_t *)argb1555);
    free(bitmap_buf);
    free(argb1555);

    argb1555 = string_to_argb1555("default.ttf", "2023-9-27 11:09:00", 64, &w, &h);
    bitmap_file.bfSize = bitmap_file.bfOffBits + w * h * 2 + 2;
    bitmap_image.biBitCount = 16;
    bitmap_image.biWidth = w;
    bitmap_image.biHeight = h;
    bitmap_image.biSizeImage = w * h * 2 + 2;
    bitmap_file_write("teststr1555.bmp", bitmap_file, bitmap_image, (uint8_t *)argb1555);
    free(argb1555);

    return 0;
}