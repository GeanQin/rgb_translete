#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_DATA_OFF_VAL_ADDR 10
#define BMP_IMG_W_ADDR 18
#define BMP_IMG_H_ADDR 22
#define BMP_HEAD_LEN 54

static int bmp888_to_argb1555(uint8_t *in_buf, ssize_t in_len, uint16_t **out_buf)
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

static ssize_t get_file_content(char *file_name, uint8_t **out_buf)
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

static void destroy_file_content(uint8_t *buf)
{
    if (buf)
    {
        free(buf);
    }
}

static void destroy_argb1555_content(uint16_t *buf)
{
    if (buf)
    {
        free(buf);
    }
}

static ssize_t write_to_file(char *file_name, uint16_t *buf, ssize_t len)
{
    FILE *fp = NULL;
    size_t write_size;

    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open %s!\n", file_name);
        return -1;
    }

    write_size = fwrite(buf, sizeof(uint16_t), len, fp);

    fclose(fp);
    return write_size;
}

int main(int argc, char *argv[])
{
    ssize_t read_size, argb1555_size;
    uint8_t *read_buf = NULL;
    uint16_t *argb1555_buf = NULL;
    size_t write_size;

    if (argc < 3)
    {
        fprintf(stderr, "please add param!\n bmp888_to_argb1555 in out\n");
        return 0;
    }

    printf("Start read %s\n", argv[1]);
    read_size = get_file_content(argv[1], &read_buf);
    printf("read %lu/B\n", read_size);

    argb1555_size = bmp888_to_argb1555(read_buf, read_size, &argb1555_buf);

    write_size = write_to_file(argv[2], argb1555_buf, argb1555_size);
    printf("write to %s (%ld)\n", argv[2], write_size * 2);

    destroy_file_content(read_buf);
    destroy_argb1555_content(argb1555_buf);
    return 0;
}