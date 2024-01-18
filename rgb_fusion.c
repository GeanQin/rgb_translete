#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "rgb_fusion.h"

unsigned char *rgb888_image_fusion(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y)
{
    int i = 0;
    unsigned char *bg_p = bg_img.data + (y * bg_img.width * 3) + (x * 3);

    if (x + source_img.width >= bg_img.width || y + source_img.height >= bg_img.height)
    {
        return NULL;
    }

    for (i = 0; i < source_img.height; i++)
    {
        if (bg_p >= bg_img.data + bg_img.data_len - source_img.width * 3)
        {
            break;
        }
        memcpy(bg_p, source_img.data + (i * source_img.width * 3), source_img.width * 3);
        bg_p = bg_p + (bg_img.width * 3);
    }
    return bg_img.data;
}

unsigned char *rgb888_image_fusion_without_color(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y, int color)
{
    int i = 0;
    int j = 0;
    unsigned char R, G, B = 0;
    unsigned char *bg_p = bg_img.data + (y * bg_img.width * 3) + (x * 3);

    if (x + source_img.width >= bg_img.width || y + source_img.height >= bg_img.height)
    {
        return NULL;
    }

    R = color >> 16;
    G = color >> 8;
    B = color;

    for (i = 0; i < source_img.height; i++)
    {
        for (j = 0; j < source_img.width * 3; j += 3)
        {
            if (bg_p >= bg_img.data + bg_img.data_len - 3)
            {
                break;
            }
            if (source_img.data[i * source_img.width * 3 + j] == R &&
                source_img.data[i * source_img.width * 3 + j + 1] == G &&
                source_img.data[i * source_img.width * 3 + j + 2] == B)
            {
                bg_p += 3;
                continue;
            }
            else
            {
                memcpy(bg_p, source_img.data + i * source_img.width * 3 + j, 3);
                bg_p += 3;
            }
        }
        bg_p = bg_p + ((bg_img.width - source_img.width) * 3);
    }
    return bg_img.data;
}

unsigned char *rgb888_image_fusion_without_color_ex(bg_image_t bg_img, source_image_t source_img, unsigned long x, unsigned long y, int color, int amplify)
{
    int i = 0;
    int j = 0;
    int k = 0;
    unsigned char R, G, B = 0;
    unsigned char *bg_p = bg_img.data + (y * bg_img.width * 3) + (x * 3);

    if (x + source_img.width * amplify >= bg_img.width || y + source_img.height * amplify >= bg_img.height)
    {
        return NULL;
    }

    R = color >> 16;
    G = color >> 8;
    B = color;

    for (i = 0; i < source_img.height * amplify; i++)
    {
        for (j = 0; j < source_img.width * 3; j += 3)
        {
            if (bg_p >= bg_img.data + bg_img.data_len - 3 * amplify)
            {
                break;
            }
            if (source_img.data[(i / amplify) * source_img.width * 3 + j] == R &&
                source_img.data[(i / amplify) * source_img.width * 3 + j + 1] == G &&
                source_img.data[(i / amplify) * source_img.width * 3 + j + 2] == B)
            {
                bg_p += 3 * amplify;
                continue;
            }
            else
            {
                for (k = 0; k < amplify; k++)
                {
                    memcpy(bg_p + 3 * k, source_img.data + (i / amplify) * source_img.width * 3 + j, 3);
                }
                bg_p += 3 * amplify;
            }
        }
        bg_p = bg_p + ((bg_img.width - source_img.width * amplify) * 3);
    }
    return bg_img.data;
}