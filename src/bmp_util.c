#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bmp_util.h"

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

    memcpy(bitmap_file->bfType, file_hand, sizeof(bitmap_file->bfType) - 1);
    bitmap_file->bfType[sizeof(bitmap_file->bfType) - 1] = 0;
    bitmap_file->bfSize = (0x00000000 | file_hand[2]) + (0x00000000 | file_hand[3] << 8) + (0x00000000 | file_hand[4] << 16) + (0x00000000 | file_hand[5] << 24);
    bitmap_file->bfReserved = (0x00000000 | file_hand[6]) + (0x00000000 | file_hand[7] << 8) + (0x00000000 | file_hand[8] << 16) + (0x00000000 | file_hand[9] << 24);
    bitmap_file->bfOffBits = (0x00000000 | file_hand[10]) + (0x00000000 | file_hand[11] << 8) + (0x00000000 | file_hand[12] << 16) + (0x00000000 | file_hand[13] << 24);

    read_len = fread(bmp_hand, 1, sizeof(bmp_hand), fp);
    if (read_len != sizeof(bmp_hand))
    {
        fprintf(stderr, "read %s bmp_hand failed\n", filename);
        return NULL;
    }

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
        fprintf(stderr, "malloc bitmap_buf failed\n");
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

void bitmap_print_head(bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image)
{
    printf("标识=%s\n", bitmap_file->bfType);
    printf("文件大小=%u\n", bitmap_file->bfSize);
    printf("保留字=%u\n", bitmap_file->bfReserved);
    printf("偏移数=%u\n\n", bitmap_file->bfOffBits);
    printf("信息头大小=%u\n", bitmap_image->biSize);
    printf("图宽=%u\n", bitmap_image->biWidth);
    printf("图高=%u\n", bitmap_image->biHeight);
    printf("固定值1=%hu\n", bitmap_image->biPlanes);
    printf("像素的位数=%hu\n", bitmap_image->biBitCount);
    printf("压缩方式=%u\n", bitmap_image->biCompression);
    printf("位图大小=%u\n", bitmap_image->biSizeImage);
    printf("水平分辨率=%u\n", bitmap_image->biXPelsPerMeter);
    printf("垂直分辨率=%u\n", bitmap_image->biYPelsPerMeter);
    printf("位图使用的颜色数=%u\n", bitmap_image->biClrUsed);
    printf("重要的颜色数=%u\n\n", bitmap_image->biClrImportant);
}

uint8_t *bitmap_real_data(bitmap_image_info_t *bitmap_image, uint8_t *buff, int if_print)
{
    unsigned char *real_data = NULL;
    int repair_byte = 0;
    int i, j = 0;
    uint8_t *p = NULL;
    uint8_t *real_p = NULL;

    if (bitmap_image == NULL || buff == NULL)
    {
        fprintf(stderr, "param err\n");
        return NULL;
    }

    if (bitmap_image->biBitCount == 24)
    {
        real_data = (unsigned char *)malloc(bitmap_image->biWidth * bitmap_image->biHeight * 3);
        if (real_data == NULL)
        {
            fprintf(stderr, "malloc real_data failed\n");
            return NULL;
        }
        real_p = real_data;

        // 3 x 5 + 1 = 16bytes，即每行最后补1个字节
        if (bitmap_image->biWidth * 3 % 4 != 0)
        {
            repair_byte = (bitmap_image->biWidth * 3 / 4 + 1) * 4 - bitmap_image->biWidth * 3;
        }

        // 保存的RGB图像是倒立的,所以从下往上
        for (i = bitmap_image->biHeight - 1; i >= 0; i--)
        {
            p = buff + (bitmap_image->biWidth * 3 + repair_byte) * i;
            for (j = 0; j < bitmap_image->biWidth * 3; j += 3)
            {
                real_p[0] = p[j + 2];
                real_p[1] = p[j + 1];
                real_p[2] = p[j];
                real_p += 3;
            }
        }
    }

    if (if_print)
    {
        for (i = 0; i < bitmap_image->biHeight; i++)
        {
            p = real_data + bitmap_image->biWidth * 3 * i;
            for (j = 0; j < bitmap_image->biWidth * 3; j += 3)
            {
                printf("%02x,", p[j]);
                printf("%02x,", p[j + 1]);
                printf("%02x,", p[j + 2]);
            }
            printf("\n");
        }
    }

    return real_data;
}

uint8_t *bitmap_format_data(bitmap_image_info_t *bitmap_image, uint8_t *buff)
{
    int repair_byte = 0;
    unsigned char *format_data = NULL;
    unsigned char *format_p = NULL;
    unsigned char *p = NULL;
    int i, j = 0;

    if (bitmap_image == NULL || buff == NULL)
    {
        fprintf(stderr, "param err\n");
        return NULL;
    }

    if (bitmap_image->biBitCount == 24)
    {
        // 3 x 5 + 1 = 16bytes，即每行最后补1个字节
        if (bitmap_image->biWidth * 3 % 4 != 0)
        {
            repair_byte = (bitmap_image->biWidth * 3 / 4 + 1) * 4 - bitmap_image->biWidth * 3;
        }

        bitmap_image->biSizeImage = (bitmap_image->biWidth * 3 + repair_byte) * bitmap_image->biHeight;
        format_data = (unsigned char *)malloc(bitmap_image->biSizeImage);
        if (format_data == NULL)
        {
            fprintf(stderr, "malloc format_data failed\n");
            return NULL;
        }
        format_p = format_data;

        // 保存的RGB图像是倒立的,所以从下往上
        for (i = bitmap_image->biHeight - 1; i >= 0; i--)
        {
            p = buff + (bitmap_image->biWidth * 3) * i;
            for (j = 0; j < bitmap_image->biWidth * 3; j += 3)
            {
                format_p[0] = p[j + 2];
                format_p[1] = p[j + 1];
                format_p[2] = p[j];
                format_p += 3;
            }
            for (j = 0; j < repair_byte; j++)
            {
                format_p[0] = 0x00;
                format_p += 1;
            }
        }
    }
    else if (bitmap_image->biBitCount == 16)    // argb1555
    {
        if (bitmap_image->biWidth * 2 % 4 != 0)
        {
            repair_byte = (bitmap_image->biWidth * 2 / 4 + 1) * 4 - bitmap_image->biWidth * 2;
        }

        bitmap_image->biSizeImage = (bitmap_image->biWidth * 2 + repair_byte) * bitmap_image->biHeight;
        format_data = (unsigned char *)malloc(bitmap_image->biSizeImage);
        if (format_data == NULL)
        {
            fprintf(stderr, "malloc format_data failed\n");
            return NULL;
        }
        format_p = format_data;

        // 保存的RGB图像是倒立的,所以从下往上
        for (i = bitmap_image->biHeight - 1; i >= 0; i--)
        {
            p = buff + (bitmap_image->biWidth * 2) * i;
            for (j = 0; j < bitmap_image->biWidth * 2; j += 2)
            {
                format_p[0] = p[j + 1];
                format_p[1] = p[j];
                format_p += 2;
            }
            for (j = 0; j < repair_byte; j++)
            {
                format_p[0] = 0x00;
                format_p += 1;
            }
        }
    }
    return format_data;
}

int bitmap_file_write(char *filename, bitmap_file_info_t *bitmap_file, bitmap_image_info_t *bitmap_image, uint8_t *buff)
{
    FILE *fp = NULL;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "open %s failed\n", filename);
        return -1;
    }

    fwrite(bitmap_file->bfType, 1, sizeof(bitmap_file->bfType) - 1, fp);
    fwrite(&bitmap_file->bfSize, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_file->bfReserved, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_file->bfOffBits, 1, sizeof(uint32_t), fp);

    fwrite(&bitmap_image->biSize, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biWidth, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biHeight, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biPlanes, 1, sizeof(uint16_t), fp);
    fwrite(&bitmap_image->biBitCount, 1, sizeof(uint16_t), fp);
    fwrite(&bitmap_image->biCompression, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biSizeImage, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biXPelsPerMeter, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biYPelsPerMeter, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biClrUsed, 1, sizeof(uint32_t), fp);
    fwrite(&bitmap_image->biClrImportant, 1, sizeof(uint32_t), fp);

    fwrite(buff, 1, bitmap_image->biSizeImage, fp);

    fclose(fp);
    return 0;
}