#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <byteswap.h>

#include "stb_truetype.h"

#include "rgb_trans.h"

static void get_string_bmp_size(uint32_t font_size, uint32_t *w, uint32_t *h)
{
    *w = font_size * 10 + 16;
    *h = font_size;
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

uint16_t *rgb888_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint16_t argb1555_color = 0;
    uint16_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 3)
    {
        fprintf(stderr, "BMP data is invaild! in_len=%lu", in_len);
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

            if (R == 0 && G == 0 && B == 0)
            {
                argb1555_color = 0x0000;
            }
            else
            {
                argb1555_color = (R << 10) | (G << 5) | B | 0x8000; // 1000,0000,0000,0000 1代表透明度
            }
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
                argb1111_color = 0x88 | ((R << 6) | (G << 5) | (B << 4)); // 1000,1000 1代表透明度
            }
            else
            {
                argb1111_color = argb1111_color | (R << 2) | (G << 1) | B;
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

    bitmap = (uint8_t *)malloc((*w) * (*h) * 2);
    if (bitmap == NULL)
    {
        free(ttf_buf);
        return NULL;
    }

    ret = stbtt_BakeFontBitmap1(ttf_buf, ttf_size, 0, font_size, bitmap, *w, *h, str);
    if (ret < 0)
    {
        fprintf(stderr, "stbtt_BakeFontBitmap1 err\n");
        free(bitmap);
        free(ttf_buf);
        return NULL;
    }

    free(ttf_buf);

    return bitmap_to_argb1555(bitmap, (*w) * (*h), *w, *h);
}