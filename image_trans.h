#include <stdint.h>
#include <stdio.h>

ssize_t get_file_content(char *file_name, uint8_t **out_buf);
int bmp888_to_argb1555(uint8_t *in_buf, ssize_t in_len, uint16_t **out_buf);
int bmp888_to_argb1111(uint8_t *in_buf, ssize_t in_len, uint8_t **out_buf);
void destroy_file_content(uint8_t *buf);
void destroy_argb1555_content(uint16_t *buf);
void destroy_argb1111_content(uint8_t *buf);

