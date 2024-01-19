#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "rgb_fusion.h"

unsigned char *rgb888_image_fusion(rgb_target_img_t bg_img, rgb_icon_t source_img, unsigned long x, unsigned long y)
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

unsigned char *rgb888_image_fusion_without_color(rgb_target_img_t bg_img, rgb_icon_t source_img, unsigned long x, unsigned long y, int color)
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

unsigned char *rgb888_image_fusion_without_color_ex(rgb_target_img_t bg_img, rgb_icon_t source_img, unsigned long x, unsigned long y, int color, int amplify)
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

unsigned char *argb1111_fusion_to_rgb888(rgb_target_img_t bg_img, rgb_icon_t source_img, unsigned long x, unsigned long y, int amplify)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int right4 = 0;
    int src_index = 0;
    unsigned char A, R, G, B = 0;
    unsigned char *bg_p = bg_img.data + (y * bg_img.width * 3) + (x * 3);

    if (x + source_img.width * amplify >= bg_img.width || y + source_img.height * amplify >= bg_img.height)
    {
        return NULL;
    }

    for (i = 0; i < source_img.height * amplify; i++)
    {
        for (j = 0; j < source_img.width; j++)
        {
            right4 = j % 2;
            src_index = j / 2;
            if (right4)
            {
                A = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x08) >> 3;
                R = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x04) >> 2;
                G = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x02) >> 1;
                B = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x01);
            }
            else
            {
                A = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x80) >> 7;
                R = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x40) >> 6;
                G = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x20) >> 5;
                B = (source_img.data[i / amplify * source_img.width / 2 + src_index] & 0x10) >> 4;
            }
            if (A)
            {
                for (k = 0; k < amplify; k++)
                {
                    bg_p[3 * k] = R ? 0xFF : 0x00;
                    bg_p[3 * k + 1] = G ? 0xFF : 0x00;
                    bg_p[3 * k + 2] = B ? 0xFF : 0x00;
                }
            }
            bg_p += 3 * amplify;
        }
        bg_p = bg_p + ((bg_img.width - source_img.width * amplify) * 3);
    }
    return bg_img.data;
}