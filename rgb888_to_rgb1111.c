#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rgb_trans.h"
#include "rgb_common.h"
#include "rgb888_source.h"

#define OUT_FILE "argb1111_img.c"

void print_rgb888_to_argb1111(rgb_icon_t *icon)
{
    int i = 0;
    int j = 0;
    FILE *fp = NULL;
    char write_buf[128] = {0};
    unsigned char *argb1111_buf = NULL;

    if (icon->type != RGB888)
    {
        printf("[%s]Param is not rgb888.\n", __func__);
        return;
    }

    argb1111_buf =rgb888_to_argb1111((uint8_t *)icon->data, icon->data_len, icon->width, icon->height);

    for (i = 0; i < icon->height; i++)
    {
        for (j = 0; j < icon->width / 2; j++)
        {
            printf("%02x,", argb1111_buf[i * icon->width / 2 + j]);
        }
        printf("\n");
    }

    fp = fopen(OUT_FILE, "a");
    if (fp == NULL)
    {
        printf("[%s]Open %s failed.\n", __func__, OUT_FILE);
        return;
    }

    snprintf(write_buf, sizeof(write_buf), "const unsigned char rgb111_%ux%u_%s[] = {\n\t", icon->width, icon->height, icon->name);
    fwrite(write_buf, 1, strlen(write_buf), fp);
    for (i = 0; i < icon->height; i++)
    {
        for (j = 0; j < icon->width / 2; j++)
        {
            snprintf(write_buf, sizeof(write_buf), "0x%02x, ", argb1111_buf[i * icon->width / 2 + j]);
            fwrite(write_buf, 1, strlen(write_buf), fp);
        }
        fwrite("\n\t", 1, strlen("\n\t"), fp);
    }
    fwrite("\n};\n\n", 1, strlen("\n};\n\n"), fp);
    fclose(fp);
}

int main()
{
    int i = 0;
    rgb_icon_t icons[] = {
        // {"num0", rgb888_20x30_num0, sizeof(rgb888_20x30_num0), RGB888, 20, 30},
        // {"num1", rgb888_20x30_num1, sizeof(rgb888_20x30_num1), RGB888, 20, 30},
        // {"num2", rgb888_20x30_num2, sizeof(rgb888_20x30_num2), RGB888, 20, 30},
        // {"num3", rgb888_20x30_num3, sizeof(rgb888_20x30_num3), RGB888, 20, 30},
        // {"num4", rgb888_20x30_num4, sizeof(rgb888_20x30_num4), RGB888, 20, 30},
        // {"num5", rgb888_20x30_num5, sizeof(rgb888_20x30_num5), RGB888, 20, 30},
        // {"num6", rgb888_20x30_num6, sizeof(rgb888_20x30_num6), RGB888, 20, 30},
        // {"num7", rgb888_20x30_num7, sizeof(rgb888_20x30_num7), RGB888, 20, 30},
        // {"num8", rgb888_20x30_num8, sizeof(rgb888_20x30_num8), RGB888, 20, 30},
        // {"num9", rgb888_20x30_num9, sizeof(rgb888_20x30_num9), RGB888, 20, 30},
        // {"minus", rgb888_20x30_minus, sizeof(rgb888_20x30_minus), RGB888, 20, 30},
        // {"colon", rgb888_20x30_colon, sizeof(rgb888_20x30_colon), RGB888, 20, 30},
        // {"xing", rgb888_20x30_xing, sizeof(rgb888_20x30_xing), RGB888, 20, 30},
        // {"qi", rgb888_20x30_qi, sizeof(rgb888_20x30_qi), RGB888, 20, 30},
        // {"ri", rgb888_20x30_ri, sizeof(rgb888_20x30_ri), RGB888, 20, 30},
        {"yin", rgb888_20x30_yin, sizeof(rgb888_20x30_yin), RGB888, 20, 30},
        {"centigrade", rgb888_30x30_centigrade, sizeof(rgb888_30x30_centigrade), RGB888, 30, 30},
    };

    for (i = 0; i < sizeof(icons) / sizeof(rgb_icon_t); i++)
    {
        printf("\n===================%s===================\n", icons[i].name);
        print_rgb888_to_argb1111(&icons[i]);
    }

    return 0;
}