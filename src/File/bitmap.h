#ifndef GRAPHICAL_BITMAP_H
#define GRAPHICAL_BITMAP_H

#include <stdio.h>
#include <stdint.h>

template<typename T>
void write_bitmap32(
    const char filename[],
    const uint32_t horizontal_res,
    const uint32_t vertical_res,
    const T* bitmap
)
{
    FILE* f;
    const uint64_t bitmap_size = 4 * vertical_res * horizontal_res;
    const uint64_t filesize = 54 + bitmap_size;
    unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 32,0 };
    unsigned char bmppad[3] = { 0,0,0 };

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(horizontal_res);
    bmpinfoheader[5] = (unsigned char)(horizontal_res >> 8);
    bmpinfoheader[6] = (unsigned char)(horizontal_res >> 16);
    bmpinfoheader[7] = (unsigned char)(horizontal_res >> 24);
    bmpinfoheader[8] = (unsigned char)(vertical_res);
    bmpinfoheader[9] = (unsigned char)(vertical_res >> 8);
    bmpinfoheader[10] = (unsigned char)(vertical_res >> 16);
    bmpinfoheader[11] = (unsigned char)(vertical_res >> 24);

    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    fwrite(bitmap, 1, bitmap_size, f);
    fclose(f);
}

void read_bitmap32(const char filename[]) {}

#endif // !GRAPHICAL_BITMAP_H
