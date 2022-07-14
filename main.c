#include <string.h>
#include "image_trans.h"

static ssize_t write_to_file(char *file_name, uint8_t *buf, ssize_t len)
{
    FILE *fp = NULL;
    size_t write_size;

    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open %s!\n", file_name);
        return -1;
    }

    write_size = fwrite(buf, sizeof(uint8_t), len, fp);

    fclose(fp);
    return write_size;
}

int main(int argc, char *argv[])
{
    ssize_t read_size, rgb_size;
    uint8_t *read_buf = NULL;
    uint16_t *argb1555_buf = NULL;
    uint8_t *argb1111_buf = NULL;
    size_t write_size;

    if (argc < 3)
    {
        fprintf(stderr, "Please add param!\n bmp888_to_argb [format] [in] [out]\n");
        return 0;
    }

    printf("Start read %s, ", argv[2]);
    read_size = get_file_content(argv[2], &read_buf);
    printf("read %lu/B\n", read_size);

    if (!strcmp("1111", argv[1]))
    {
        rgb_size = bmp888_to_argb1111(read_buf, read_size, &argb1111_buf);
        write_size = write_to_file(argv[3], argb1111_buf, rgb_size);
        destroy_argb1111_content(argb1111_buf);
    }
    else if (!strcmp("1555", argv[1]))
    {
        rgb_size = bmp888_to_argb1555(read_buf, read_size, &argb1555_buf);
        write_size = write_to_file(argv[3], (uint8_t *)argb1555_buf, rgb_size * 2);
        destroy_argb1555_content(argb1555_buf);
    }
    else
    {
        fprintf(stderr, "Do not support format [%s]\n", argv[1]);
        return 0;
    }

    printf("write to %s (%ld)\n", argv[3], write_size);
    destroy_file_content(read_buf);
    return 0;
}