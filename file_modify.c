#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define FILE_SIZE           (10 * 1024 * 1024) /* 10M */
#define BMP_HEAD_SIZE       54
#define PNG_HEAD_SIZE       33
#define PNG_WIDTH_OFFSET    22

enum image_type {
    jpg = 1,
    bmp,
    png
};

#pragma pack(1)
typedef struct tag_bmp_head {
    unsigned char   bfType[2];
    unsigned int    bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned int    bfOffBits;
    unsigned int    biSize;
    unsigned int    biWidth;
    unsigned int    biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biComppression;
    unsigned int    biSizeImage;
}BMP_HEAD_S;
#pragma pack()

int file_type(unsigned char *f) {
    if ((0xFF == f[0]) && (0xD8 == f[1]) && (0xFF == f[2]) && (0xE0 == (f[3] & 0xF0))) {
        return jpg;
    } else if ((0x42 == f[0]) && (0x4D == f[1])) {
        return bmp;
    } else if ((0x89 == f[0]) && (0x50 == f[1]) && (0x4E == f[2]) && (0x47 == f[3])) {
        return png;
    }

    return 0;
}

int modify_jpg(unsigned char *f, int file_size) {
    unsigned char *p_last = NULL;
    unsigned char *p_height = NULL;

    p_height = memchr(f, 0xFF, file_size);
    while (NULL != p_height) {
        if (0xC0 == *(p_height + 1)) {
            p_last = p_height;
        }
        
        p_height = memchr(p_height + 2, 0xFF, file_size - (p_height - f));
    }

    if (NULL == p_last) {
        printf("JPG can't find 0xFFC0\r\n");
        return -1;
    }

    *(p_last + 5) = 0x0A;
    
    return 0;
}

int modify_bmp(unsigned char *f, int file_size) {
    unsigned int bitcount = 0;
    unsigned int trueheight = 0;
    BMP_HEAD_S *p_bmp_head = (BMP_HEAD_S *)f;

    if (file_size <= BMP_HEAD_SIZE) {
        printf("BMP length less than head size\r\n");
        return -1;
    }

    if (0 == p_bmp_head->biWidth) {
        printf("BMP width is zero\r\n");
        return -1;
    }
    
    bitcount = p_bmp_head->biBitCount / 8;
    if (0 == bitcount) {
        bitcount = 1;
    }

    trueheight = ((p_bmp_head->bfSize - p_bmp_head->bfOffBits) / bitcount) / p_bmp_head->biWidth;
    if (trueheight != p_bmp_head->biHeight) {
        printf("BMP height abnormal, true height: %u, current height: %u\r\n", trueheight, p_bmp_head->biHeight);
    }

    p_bmp_head->biHeight = trueheight;
    
    return 0;
}

int modify_png(unsigned char *f, int file_size) {
    unsigned char *p_height = NULL;

    if (file_size <= PNG_HEAD_SIZE) {
        printf("PNG length less than head size\r\n");
        return -1;
    }

    p_height = f + PNG_WIDTH_OFFSET;
    *p_height = 0x06;
    
    return 0;
}

int main(int argc, char *argv[]) {
    int ret = 0;
    int f_len = 0;
    FILE *orif = NULL;
    FILE *newf = NULL;
    unsigned char *f = NULL;
    char new_fname[64] = "new_";
    enum image_type img;

    if (2 != argc) {
        printf("Miss args\r\n");
        return -1;
    }

    if (NULL == (orif = fopen(argv[1], "rb"))) {
        printf("File open error\r\n");
        return -1;
    }

    f = malloc(FILE_SIZE);
    if (NULL == f) {
        printf("Malloc error\r\n");
        fclose(orif);
        return -1;
    }

    f_len = fread(f, 1, FILE_SIZE, orif);
    if (f_len + 10 >= FILE_SIZE) {
        printf("File too large\r\n");
        free(f);
        fclose(orif);
        return -1;
    }

    img = file_type(f);
    switch (img) {
        case jpg:
            ret = modify_jpg(f, f_len);
            break;
        case bmp:
            ret = modify_bmp(f, f_len);
            break;
        case png:
            ret = modify_png(f, f_len);
            break;
        default:
            free(f);
            fclose(orif);
            printf("Unknown image type\r\n");
            return -1;
    }

    if (0 != ret) {
        free(f);
        fclose(orif);
        printf("Modify file error\r\n");
        return -1;
    }

    if (NULL == (newf = fopen(strcat(new_fname, argv[1]), "wb"))) {        
        free(f);
        fclose(orif);
        printf("New file open error\r\n");
        return -1;
    }

    (void)fwrite(f, f_len, 1, newf);
    
    free(f);
    fclose(orif);
    fclose(newf);
	
    printf("Done\r\n");
    return 0;
}

