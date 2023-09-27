#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <byteswap.h>

#include "stb_truetype.h"

#include "bmp_trans.h"

static void get_string_bmp_size(uint32_t font_size, uint32_t *w, uint32_t *h)
{
    if (font_size == 48)
    {
        *w = 420;
        *h = 48;
    }
    else if (font_size == 92)
    {
        *w = 716;
        *h = 92;
    }
    else if (font_size == 24)
    {
        *w = 200;
        *h = 24;
    }
    else if (font_size == 64)
    {
        *w = 552;
        *h = 64;
    }
    else if (font_size == 20)
    {
        *w = 192;
        *h = 20;
    }
    else if (font_size == 32)
    {
        *w = 276;
        *h = 32;
    }
    else
    {
        *w = 140;
        *h = 16;
    }
}

static uint16_t *bitmap_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t one_bit;
    uint16_t argb1555_color = 0;
    uint16_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h)
    {
        fprintf(stderr, "bitmap is invaild!");
        return NULL;
    }

    data_count = w * h;

    out_buf = (uint16_t *)malloc(sizeof(uint16_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            memcpy(&one_bit, in_buf + (i * w + j), sizeof(uint8_t));
            if (one_bit)
            {
                argb1555_color = 0xffff;    // 白字
            }
            else
            {
                argb1555_color = 0x0000;    // 透明底，0x8000黑底
            }
            // memcpy(out_buf + i * w + j, &argb1555_color, sizeof(uint16_t));
            memcpy(out_buf + (h - 1 - i) * w + j, &argb1555_color, sizeof(uint16_t));
        }
    }

    return out_buf;
}

uint8_t *bitmap_file_read(char *filename, bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image)
{
    FILE *fp = NULL;
    size_t read_len = 0;
    unsigned char file_hand[14] = {0};
    unsigned char bmp_hand[40] = {0};
    unsigned char *bitmap_buf = NULL;

    if (bitmap_file == NULL || bitmap_image == NULL)
    {
        fprintf(stderr, "param err\n");
        return NULL;
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "open %s failed\n", filename);
        return NULL;
    }

    read_len = fread(file_hand, 1, sizeof(file_hand), fp);
    if (read_len != sizeof(file_hand))
    {
        fprintf(stderr, "read %s file_hand failed\n", filename);
        return NULL;
    }

#if 0
    for (int i = 0; i < sizeof(file_hand); i++)
    {
        printf("%02x ", file_hand[i]);
    }
    printf("\n");
#endif

    memcpy(bitmap_file->bfType, file_hand, sizeof(bitmap_file->bfType));
    bitmap_file->bfSize = (0x00000000 | file_hand[2]) + (0x00000000 | file_hand[3] << 8) + (0x00000000 | file_hand[4] << 16) + (0x00000000 | file_hand[5] << 24);
    bitmap_file->bfReserved = (0x00000000 | file_hand[6]) + (0x00000000 | file_hand[7] << 8) + (0x00000000 | file_hand[8] << 16) + (0x00000000 | file_hand[9] << 24);
    bitmap_file->bfOffBits = (0x00000000 | file_hand[10]) + (0x00000000 | file_hand[11] << 8) + (0x00000000 | file_hand[12] << 16) + (0x00000000 | file_hand[13] << 24);

    read_len = fread(bmp_hand, 1, sizeof(bmp_hand), fp);
    if (read_len != sizeof(bmp_hand))
    {
        fprintf(stderr, "read %s bmp_hand failed\n", filename);
        return NULL;
    }

#if 0
    for (int i = 0; i < sizeof(bmp_hand); i++)
    {
        printf("%02x ", bmp_hand[i]);
    }
    printf("\n");
#endif

    bitmap_image->biSize = (0x00000000 | bmp_hand[0]) + (0x00000000 | bmp_hand[1] << 8) + (0x00000000 | bmp_hand[2] << 16) + (0x00000000 | bmp_hand[3] << 24);
    bitmap_image->biWidth = (0x00000000 | bmp_hand[4]) + (0x00000000 | bmp_hand[5] << 8) + (0x00000000 | bmp_hand[6] << 16) + (0x00000000 | bmp_hand[7] << 24);
    bitmap_image->biHeight = (0x00000000 | bmp_hand[8]) + (0x00000000 | bmp_hand[9] << 8) + (0x00000000 | bmp_hand[10] << 16) + (0x00000000 | bmp_hand[11] << 24);
    bitmap_image->biPlanes = (0x00000000 | bmp_hand[12]) + (0x00000000 | bmp_hand[13] << 8);
    bitmap_image->biBitCount = (0x00000000 | bmp_hand[14]) + (0x00000000 | bmp_hand[15] << 8);
    bitmap_image->biCompression = (0x00000000 | bmp_hand[16]) + (0x00000000 | bmp_hand[17] << 8) + (0x00000000 | bmp_hand[18] << 16) + (0x00000000 | bmp_hand[19] << 24);
    bitmap_image->biSizeImage = (0x00000000 | bmp_hand[20]) + (0x00000000 | bmp_hand[21] << 8) + (0x00000000 | bmp_hand[22] << 16) + (0x00000000 | bmp_hand[23] << 24);
    bitmap_image->biXPelsPerMeter = (0x00000000 | bmp_hand[24]) + (0x00000000 | bmp_hand[25] << 8) + (0x00000000 | bmp_hand[26] << 16) + (0x00000000 | bmp_hand[27] << 24);
    bitmap_image->biYPelsPerMeter = (0x00000000 | bmp_hand[28]) + (0x00000000 | bmp_hand[29] << 8) + (0x00000000 | bmp_hand[30] << 16) + (0x00000000 | bmp_hand[31] << 24);
    bitmap_image->biClrUsed = (0x00000000 | bmp_hand[32]) + (0x00000000 | bmp_hand[33] << 8) + (0x00000000 | bmp_hand[34] << 16) + (0x00000000 | bmp_hand[35] << 24);
    bitmap_image->biClrImportant = (0x00000000 | bmp_hand[36]) + (0x00000000 | bmp_hand[37] << 8) + (0x00000000 | bmp_hand[38] << 16) + (0x00000000 | bmp_hand[39] << 24);

    bitmap_buf = (unsigned char *)malloc(bitmap_image->biSizeImage);
    if (bitmap_buf == NULL)
    {
        return NULL;
    }
    read_len = fread(bitmap_buf, 1, bitmap_image->biSizeImage, fp);
    if (read_len != bitmap_image->biSizeImage)
    {
        fprintf(stderr, "read %s bmp_hand failed\n", filename);
        return NULL;
    }

    fclose(fp);
    return bitmap_buf;
}

uint16_t *rgb888_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint16_t argb1555_color = 0;
    uint16_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 3)
    {
        fprintf(stderr, "BMP data is invaild!");
        return NULL;
    }

    data_count = w * h;

    out_buf = (uint16_t *)malloc(sizeof(uint16_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            memcpy(&B, in_buf + (i * w + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + (i * w + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&R, in_buf + (i * w + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 3) & 0x1F;
            G = (G >> 3) & 0x1F;
            B = (B >> 3) & 0x1F;

            argb1555_color = (R << 10) | (G << 5) | B | 0x8000; // 1000,0000,0000,0000 1代表透明度
            // memcpy(out_buf + (h - 1 - i) * w + j, &argb1555_color, sizeof(uint16_t));
            memcpy(out_buf + i * w + j, &argb1555_color, sizeof(uint16_t));
        }
    }

    return out_buf;
}

uint8_t *rgb888_to_argb1111(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint8_t argb1111_color = 0;
    uint8_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 3)
    {
        fprintf(stderr, "BMP data is invaild!");
        return NULL;
    }

    data_count = w * h / 2;

    out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            memcpy(&B, in_buf + (i * w + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + (i * w + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&R, in_buf + (i * w + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 7) & 0x01;
            G = (G >> 7) & 0x01;
            B = (B >> 7) & 0x01;

            if (j % 2 == 0)
            {
                argb1111_color = 0x88 | (R << 2) | (G << 1) | B; // 1000,1000 1代表透明度
            }
            else
            {
                argb1111_color = argb1111_color | ((R << 6) | (G << 5) | (B << 4));
                // memcpy(out_buf + (h - 1 - i) * w / 2 + j / 2, &argb1111_color, sizeof(uint8_t));
                memcpy(out_buf + i * w / 2 + j / 2, &argb1111_color, sizeof(uint8_t));
            }
        }
    }

    return out_buf;
}

uint16_t *string_to_argb1555(char *ttf_path, char *str, uint32_t font_size, uint32_t *w, uint32_t *h)
{
    int ret = 0;
    FILE *ttf_fp = NULL;
    long ttf_size = 0;
    uint8_t *ttf_buf = NULL;
    uint8_t *bitmap = NULL;

    ttf_fp = fopen(ttf_path, "r");
    if (ttf_fp == NULL)
    {
        fprintf(stderr, "cannot open %s\n", ttf_path);
        return NULL;
    }

    fseek(ttf_fp, 0, SEEK_END);
    ttf_size = ftell(ttf_fp);
    fseek(ttf_fp, 0, SEEK_SET);

    ttf_buf = (uint8_t *)malloc(ttf_size + 2);
    if (ttf_buf == NULL)
    {
        fclose(ttf_fp);
        return NULL;
    }

    memset(ttf_buf, 0, ttf_size + 2);
    if (fread(ttf_buf, 1, ttf_size, ttf_fp) != ttf_size)
    {
        free(ttf_buf);
        fclose(ttf_fp);
        return NULL;
    }
    fclose(ttf_fp);

    get_string_bmp_size(font_size, w, h);

    bitmap = (uint8_t *)malloc((*w) * (*h));
    if (bitmap == NULL)
    {
        free(ttf_buf);
        return NULL;
    }

    ret = stbtt_BakeFontBitmap1(ttf_buf, ttf_size, 0, font_size, bitmap, *w, *h, str);
    if (ret < 0)
    {
        free(bitmap);
        free(ttf_buf);
        return NULL;
    }

    free(ttf_buf);

    return bitmap_to_argb1555(bitmap, (*w) * (*h), *w, *h);
}

int bitmap_file_write(char *filename, bitmap_file_info_t bitmap_file, bitmap_image_info_t bitmap_image, uint8_t *buf)
{
    FILE *fp = NULL;
    uint32_t tmp32 = 0;
    uint16_t tmp16 = 0;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "open %s failed\n", filename);
        return -1;
    }

#if 0 // 如果大端
    fwrite(bitmap_file.bfType, 1, sizeof(bitmap_file.bfType), fp);
    tmp32 = bswap_32(bitmap_file.bfSize);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_file.bfReserved);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_file.bfOffBits);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);

    tmp32 = bswap_32(bitmap_image.biSize);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biWidth);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biHeight);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp16 = bswap_16(bitmap_image.biPlanes);
    fwrite(&tmp16, 1, sizeof(uint16_t), fp);
    tmp16 = bswap_16(bitmap_image.biBitCount);
    fwrite(&tmp16, 1, sizeof(uint16_t), fp);
    tmp32 = bswap_32(bitmap_image.biCompression);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biSizeImage);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biXPelsPerMeter);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biYPelsPerMeter);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biClrUsed);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
    tmp32 = bswap_32(bitmap_image.biClrImportant);
    fwrite(&tmp32, 1, sizeof(uint32_t), fp);
#else
    fwrite(bitmap_file.bfType, 1, sizeof(bitmap_file.bfType), fp);
    fwrite(&bitmap_file.bfSize, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_file.bfReserved, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_file.bfOffBits, 1, sizeof(uint32_t), fp);

    fwrite(&bitmap_image.biSize, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biWidth, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biHeight, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biPlanes, 1, sizeof(uint16_t), fp);
    fwrite(&bitmap_image.biBitCount, 1, sizeof(uint16_t), fp);
    fwrite(&bitmap_image.biCompression, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biSizeImage, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biXPelsPerMeter, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biYPelsPerMeter, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biClrUsed, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image.biClrImportant, 1, sizeof(uint32_t), fp);

#endif
    if (bitmap_image.biBitCount == 16)
    {
        fwrite((uint16_t *)buf, sizeof(uint16_t), bitmap_image.biWidth * bitmap_image.biHeight, fp);
    }
    else if (bitmap_image.biBitCount == 4)
    {
        fwrite(buf, 1, bitmap_image.biWidth * bitmap_image.biHeight / 2, fp);
    }
    else
    {
        fprintf(stderr, "do not support this bitcount\n");
    }

    tmp16 = 0;
    fwrite(&tmp16, 1, sizeof(uint16_t), fp);

    fclose(fp);
    return 0;
}