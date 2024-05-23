#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_util.h"
#include "rgb_util.h"
#include "rgb_fusion.h"

void print_data(unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i += 2)
    {
        printf("%02x", buf[i]);
        printf("%02x,", buf[i+1]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    unsigned char *file_buff = NULL;
    unsigned char *real_buff = NULL;
    unsigned char *rgb565_buf = NULL;
    unsigned char *argb1555_buf = NULL;
    bitmap_file_info_t bmp_file;
    bitmap_image_info_t bmp_image;
    // file_buff = bitmap_file_read(argv[2], &bmp_file, &bmp_image);
    // bitmap_print_head(&bmp_file, &bmp_image);
    // real_buff = bitmap_real_data(&bmp_image, file_buff, 1);
    // free(file_buff);

    // rgb565_buf = rgb888_to_rgb565(real_buff, bmp_image.biWidth * bmp_image.biHeight * 3, bmp_image.biWidth, bmp_image.biHeight);
    // free(real_buff);

    // argb1555_buf = rgb565_to_argb1555(rgb565_buf, bmp_image.biWidth * bmp_image.biHeight * 2, bmp_image.biWidth, bmp_image.biHeight);
    // free(rgb565_buf);

    // bmp_image.biBitCount = 16;
    // file_buff = bitmap_format_data(&bmp_image, argb1555_buf);
    // free(argb1555_buf);

    // bmp_file.bfSize = bmp_file.bfOffBits + bmp_image.biSizeImage;
    // bitmap_file_write("test.bmp", &bmp_file, &bmp_image, file_buff);

    // int w, h = 0;
    // argb1555_buf = string_to_argb1555("default.ttf", "123", 48, &w, &h);
    // bmp_image.biBitCount = 16;
    // bmp_image.biClrImportant = 0;
    // bmp_image.biClrUsed = 0;
    // bmp_image.biCompression = 0;
    // bmp_image.biHeight = h;
    // bmp_image.biWidth = w;
    // bmp_image.biPlanes = 1;
    // bmp_image.biSize = 40;
    // bmp_image.biSizeImage = w * h * 2;
    // bmp_image.biXPelsPerMeter = 0;
    // bmp_image.biYPelsPerMeter = 0;

    // bmp_file.bfOffBits = 54;
    // bmp_file.bfReserved = 0;
    // bmp_file.bfSize = bmp_file.bfOffBits + bmp_image.biSizeImage;
    
    // memcpy(bmp_file.bfType, "BM", 2);

    // file_buff = bitmap_format_data(&bmp_image, argb1555_buf);
    // free(argb1555_buf);
    
    // bitmap_file_write("test.bmp", &bmp_file, &bmp_image, file_buff);

    bitmap_file_info_t bg_file;
    bitmap_image_info_t bg_image;
    unsigned char *rgb888_bg_buf = bitmap_file_read("./images/rgb888/rgb888_1024x800_black.bmp", &bg_file, &bg_image);
    unsigned char *rgb888_bg_realbuf = bitmap_real_data(&bg_image, rgb888_bg_buf, 0);
    free(rgb888_bg_buf);
    unsigned char *rgb565_bg_buf = rgb888_to_rgb565(rgb888_bg_realbuf, bg_image.biWidth * bg_image.biHeight * 3, bg_image.biWidth, bg_image.biHeight);
    free(rgb888_bg_realbuf);

    bitmap_file_info_t src_file;
    bitmap_image_info_t src_image;
    unsigned char *rgb888_src_buf = bitmap_file_read("./images/rgb888/rgb888_864x480_src.bmp", &src_file, &src_image);
    unsigned char *rgb888_src_realbuf = bitmap_real_data(&src_image, rgb888_src_buf, 0);
    free(rgb888_src_buf);
    unsigned char *rgb565_src_buf = rgb888_to_rgb565(rgb888_src_realbuf, src_image.biWidth * src_image.biHeight * 3, src_image.biWidth, src_image.biHeight);
    free(rgb888_src_realbuf);

    rgb_target_img_t bg_img;
    bg_img.data = rgb565_bg_buf;
    bg_img.data_len = bg_image.biSizeImage;
    bg_img.height = bg_image.biHeight;
    bg_img.width = bg_image.biWidth;

    rgb_icon_t src_img;
    src_img.data = rgb565_src_buf;
    src_img.data_len = src_image.biSizeImage;
    src_img.height = src_image.biHeight;
    src_img.width = src_image.biWidth;

    unsigned char *ret_buf = rgb565_image_fusion_without_color(bg_img, src_img, 10, 10, 0x0000);
    argb1555_buf = rgb565_to_argb1555(ret_buf, bg_image.biWidth * bg_image.biHeight * 2, bg_image.biWidth, bg_image.biHeight);

    bg_image.biBitCount = 16;
    file_buff = bitmap_format_data(&bg_image, argb1555_buf);
    free(argb1555_buf);

    bg_file.bfSize = bg_file.bfOffBits + bg_image.biSizeImage;
    bitmap_file_write("test.bmp", &bg_file, &bg_image, file_buff);

}