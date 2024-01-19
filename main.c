#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_common.h"
#include "rgb_trans.h"
#include "rgb_fusion.h"

void print_usage(char* cmd)
{
    printf("Usage:\n\t%s [print, trans, fusion] ...\n", cmd);
    printf("Such as:\n\t%s print test.bmp\n", cmd);
    printf("\t%s trans test.bmp 1555\n", cmd);
    printf("\t%s trans \"123一二三\" 16 1555\n", cmd);
    printf("\t%s fusion bg.bmp source.bmp\n", cmd);
}

int main(int argc, char *argv[])
{
    unsigned char *bmp_buf = NULL;
    bitmap_file_info_t bmp_file;
    bitmap_image_info_t bmp_image;

    if (argc < 2)
    {
        print_usage(argv[0]);
        exit(-1);
    }

    if (strncmp("print", argv[1], strlen(argv[1])) == 0)
    {
        bmp_buf = bitmap_file_read(argv[2], &bmp_file, &bmp_image);
        bitmap_print_head(&bmp_file, &bmp_image, bmp_buf);
    }
    else if (strncmp("trans", argv[1], strlen(argv[1])) == 0)
    {
        if (strncmp(".bmp", argv[2] + strlen(argv[2]) - 4, 4) == 0)
        {
            bmp_buf = bitmap_file_read(argv[2], &bmp_file, &bmp_image);
            if (bmp_image.biBitCount == 24 && strncmp("1555", argv[3], strlen(argv[3])) == 0)
            {
                uint16_t *argb1555 = rgb888_to_argb1555(bmp_buf, bmp_image.biSizeImage, bmp_image.biWidth, bmp_image.biHeight);
                bmp_image.biBitCount = 16;
                bmp_image.biSizeImage = bmp_image.biWidth * bmp_image.biHeight * 2;
                bmp_file.bfSize = bmp_file.bfOffBits + bmp_image.biSizeImage;
                bitmap_file_write("test.bmp", &bmp_file, &bmp_image, (unsigned char *)argb1555);
            }
        }
        else
        {
            if (strncmp("1555", argv[4], strlen(argv[4])) == 0)
            {
                int w, h = 0;
                uint16_t *argb1555 = string_to_argb1555("default.ttf", argv[2], atoi(argv[3]), &w, &h);
                bmp_image.biBitCount = 16;
                bmp_image.biClrImportant = 0;
                bmp_image.biClrUsed = 0;
                bmp_image.biCompression = 0;
                bmp_image.biHeight = h;
                bmp_image.biWidth = w;
                bmp_image.biPlanes = 1;
                bmp_image.biSize = 40;
                bmp_image.biSizeImage = w * h * 2;
                bmp_image.biXPelsPerMeter = 0;
                bmp_image.biYPelsPerMeter = 0;

                bmp_file.bfOffBits = 54;
                bmp_file.bfReserved = 0;
                bmp_file.bfSize = bmp_file.bfOffBits + bmp_image.biSizeImage;
                memcpy(bmp_file.bfType, "BM", 2);
                bitmap_file_write("test.bmp", &bmp_file, &bmp_image, (unsigned char *)argb1555);
            }
        }
    }
    else if (strncmp("fusion", argv[1], strlen(argv[1])) == 0)
    {
        bmp_buf = bitmap_file_read(argv[2], &bmp_file, &bmp_image);

        bitmap_file_info_t src_file;
        bitmap_image_info_t src_image;
        unsigned char *src_buf = bitmap_file_read(argv[3], &src_file, &src_image);

        rgb_target_img_t bg_img;
        bg_img.data = bmp_buf;
        bg_img.data_len = bmp_image.biSizeImage;
        bg_img.height = bmp_image.biHeight;
        bg_img.width = bmp_image.biWidth;

        rgb_icon_t src_img;
        src_img.data = src_buf;
        src_img.data_len = src_image.biSizeImage;
        src_img.height = src_image.biHeight;
        src_img.width = src_image.biWidth;
        unsigned char *ret_buf = rgb888_image_fusion_without_color_ex(bg_img, src_img, 10, 10, 0x000000, 2);
        bitmap_file_write("test.bmp", &bmp_file, &bmp_image, ret_buf);
    }
}