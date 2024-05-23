#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "rgb_util.h"
#include "stb_truetype.h"

uint8_t *rgb888_to_rgb565(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint8_t rgb565_color[2] = {0, 0};
    uint8_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 3)
    {
        fprintf(stderr, "BMP data is invaild! in_len=%lu", in_len);
        return NULL;
    }

    data_count = w * h * 2;

    out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            memcpy(&R, in_buf + (i * w + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + (i * w + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&B, in_buf + (i * w + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 3) & 0x1F;
            G = (G >> 2) & 0x3F;
            B = (B >> 3) & 0x1F;

            if (R == 0 && G == 0 && B == 0)
            {
                rgb565_color[0] = 0x00;
                rgb565_color[1] = 0x00;
            }
            else
            {
                rgb565_color[0] = R << 3 | G >> 3;
                rgb565_color[1] = G << 5 | B;
            }
            memcpy(out_buf + i * w * 2 + j * 2, rgb565_color, sizeof(rgb565_color));
        }
    }

    return out_buf;
}

uint8_t *rgb888_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint8_t argb1555_color[2] = {0, 0};
    uint8_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 3)
    {
        fprintf(stderr, "BMP data is invaild! in_len=%lu", in_len);
        return NULL;
    }

    data_count = w * h * 2;

    out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            memcpy(&R, in_buf + (i * w + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + (i * w + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&B, in_buf + (i * w + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 3) & 0x1F;
            G = (G >> 3) & 0x1F;
            B = (B >> 3) & 0x1F;

            if (R == 0 && G == 0 && B == 0)
            {
                argb1555_color[0] = 0x00;
                argb1555_color[1] = 0x00;
            }
            else
            {
                argb1555_color[0] = 0x80 | R << 2 | G >> 3;
                argb1555_color[1] = G << 5 | B;
            }
            memcpy(out_buf + i * w * 2 + j * 2, argb1555_color, sizeof(argb1555_color));
        }
    }

    return out_buf;
}

uint8_t *rgb565_to_argb1555(uint8_t *in_buf, uint64_t in_len, uint32_t w, uint32_t h)
{
    ssize_t i, data_count;
    uint8_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h * 2)
    {
        fprintf(stderr, "BMP data is invaild! in_len=%lu", in_len);
        return NULL;
    }

    data_count = w * h * 2;

    out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);
    if (out_buf == NULL)
    {
        return NULL;
    }

    for (i = 0; i < in_len; i += 2)
    {
        out_buf[i] = 0x80 | (in_buf[i] >> 1);
        out_buf[i + 1] = (in_buf[i] << 7) | ((in_buf[i + 1] & 0xC0) >> 1) | (in_buf[i + 1] & 0x1F);
    }

    return out_buf;
}

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
    uint8_t *out_buf = NULL;

    if (in_buf == NULL || in_len < w * h)
    {
        fprintf(stderr, "bitmap is invaild!");
        return NULL;
    }

    data_count = w * h * 2;

    out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);
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
                argb1555_color = 0x0000;    // 透明底，0x0080黑底
            }
            memcpy(out_buf + i * w * 2 + j * 2, &argb1555_color, sizeof(argb1555_color));
        }
    }

    return out_buf;
}

uint8_t *string_to_argb1555(char *ttf_path, char *str, uint32_t font_size, uint32_t *w, uint32_t *h)
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