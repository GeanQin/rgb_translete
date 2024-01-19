#ifndef __RGB_COMMON_H__
#define __RGB_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    RGB888 = 0,
    ARGB1111,
    ARGB1555,
}rgb_img_type_e;

typedef struct
{
    char name[16];
    const unsigned char *data;
    unsigned long data_len;
    rgb_img_type_e type;
    unsigned int width;
    unsigned int height;
}rgb_icon_t;

typedef struct
{
    unsigned char *data;
    unsigned long data_len;
    rgb_img_type_e type;
    unsigned int width;
    unsigned int height;
}rgb_target_img_t;

#ifdef __cplusplus
}
#endif

#endif