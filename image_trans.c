#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "image_trans.h"

#define BMP_DATA_OFF_VAL_ADDR 10
#define BMP_IMG_W_ADDR 18
#define BMP_IMG_H_ADDR 22
#define BMP_HEAD_LEN 54

int bmp888_to_argb1555(uint8_t *in_buf, ssize_t in_len, uint16_t **out_buf)
{
    uint32_t data_offset, image_width, image_height;
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint16_t argb1555_color;

    if (in_len < BMP_HEAD_LEN)
    {
        fprintf(stderr, "BMP data is invaild!");
        return -1;
    }

    memcpy(&data_offset, in_buf + BMP_DATA_OFF_VAL_ADDR, sizeof(uint32_t));
    memcpy(&image_width, in_buf + BMP_IMG_W_ADDR, sizeof(uint32_t));
    memcpy(&image_height, in_buf + BMP_IMG_H_ADDR, sizeof(uint32_t));

    data_count = image_width * image_height;

    *out_buf = (uint16_t *)malloc(sizeof(uint16_t) * data_count);

    for (i = 0; i < image_height; i++)
    {
        for (j = 0; j < image_width; j++)
        {
            memcpy(&B, in_buf + data_offset + (i * image_width + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + data_offset + (i * image_width + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&R, in_buf + data_offset + (i * image_width + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 3) & 0x1F;
            G = (G >> 3) & 0x1F;
            B = (B >> 3) & 0x1F;

            argb1555_color = (R << 10) | (G << 5) | B | 0x8000;
            memcpy((*out_buf) + (image_height - 1 - i) * image_width + j, &argb1555_color, sizeof(uint16_t));
        }
    }

    return data_count;
}

int bmp888_to_argb1111(uint8_t *in_buf, ssize_t in_len, uint8_t **out_buf)
{
    uint32_t data_offset, image_width, image_height;
    ssize_t i, j, data_count;
    uint8_t R, G, B;
    uint8_t argb1111_color = 0;

    if (in_len < BMP_HEAD_LEN)
    {
        fprintf(stderr, "BMP data is invaild!");
        return -1;
    }

    memcpy(&data_offset, in_buf + BMP_DATA_OFF_VAL_ADDR, sizeof(uint32_t));
    memcpy(&image_width, in_buf + BMP_IMG_W_ADDR, sizeof(uint32_t));
    memcpy(&image_height, in_buf + BMP_IMG_H_ADDR, sizeof(uint32_t));

    data_count = image_width * image_height / 2;

    *out_buf = (uint8_t *)malloc(sizeof(uint8_t) * data_count);

    for (i = 0; i < image_height; i++)
    {
        for (j = 0; j < image_width; j++)
        {
            memcpy(&B, in_buf + data_offset + (i * image_width + j) * 3, sizeof(uint8_t));
            memcpy(&G, in_buf + data_offset + (i * image_width + j) * 3 + 1, sizeof(uint8_t));
            memcpy(&R, in_buf + data_offset + (i * image_width + j) * 3 + 2, sizeof(uint8_t));
            R = (R >> 7) & 0x01;
            G = (G >> 7) & 0x01;
            B = (B >> 7) & 0x01;

            if (j % 2 == 0)
            {
                if (R == 0 && G == 0 && B == 0)
                {
                    argb1111_color = 0x0C;
                }
                else
                {
                    argb1111_color = (R << 2) | (G << 1) | B;
                }
            }
            else
            {
                if (R == 0 && G == 0 && B == 0)
                {
                    argb1111_color = argb1111_color | 0xC0;
                }
                else
                {
                    argb1111_color = argb1111_color | ((R << 6) | (G << 5) | (B << 4));
                }

                memcpy((*out_buf) + (image_height - 1 - i) * image_width / 2 + j / 2, &argb1111_color, sizeof(uint8_t));
            }
        }
    }

    return data_count;
}

ssize_t get_file_content(char *file_name, uint8_t **out_buf)
{
    int fd;
    size_t file_size;
    ssize_t read_size;

    fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Cannot open %s!\n", file_name);
        return -1;
    }

    file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    *out_buf = (uint8_t *)malloc(sizeof(uint8_t) * file_size);

    read_size = read(fd, *out_buf, file_size);
    if (read_size != file_size)
    {
        fprintf(stderr, "Have not get all connect from %s!\n", file_name);
        return -1;
    }

    close(fd);
    return read_size;
}

void destroy_file_content(uint8_t *buf)
{
    if (buf)
    {
        free(buf);
    }
}

void destroy_argb1555_content(uint16_t *buf)
{
    if (buf)
    {
        free(buf);
    }
}

void destroy_argb1111_content(uint8_t *buf)
{
    if (buf)
    {
        free(buf);
    }
}