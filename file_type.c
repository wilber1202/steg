#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_SIZE (30 * 1024 * 1024) /* 30M */

void identify(unsigned char *f, int len) {
    int i;

    /* JPG head */
    for (i = 0; i < len; i++) {
        if ((0xFF == f[i]) && (0xD8 == f[i + 1]) && (0xFF == f[i + 2]) && (0xE0 == (f[i + 3] & 0xF0))
            && ((0x4A == f[i + 6]) || (0x45 == f[i + 6])) && ((0x46 == f[i + 7]) || (0x78 == f[i + 7])) 
            && ((0x49 == f[i + 8]) || (0x69 == f[i + 8])) && ((0x46 == f[i + 9]) || (0x66 == f[i + 9]))) {
            printf("JPG head\r\n");
        }
    }

    /* JPG tail */
    for (i = 0; i < len; i++) {
        if ((0xFF == f[i]) && (0xD9 == f[i + 1])) {
            printf("JPG tail\r\n");
        }
    }

    /* PNG head */
    for (i = 0; i < len; i++) {
        if ((0x89 == f[i]) && (0x50 == f[i + 1]) && (0x4E == f[i + 2]) && (0x47 == f[i + 3])
            && (0x0D == f[i + 4]) && (0x0A == f[i + 5]) && (0x1A == f[i + 6]) && (0x0A == f[i + 7])) {
            printf("PNG head\r\n");
        }
    }

    /* PNG tail */
    for (i = 0; i < len; i++) {
        if ((0x49 == f[i]) && (0x45 == f[i + 1]) && (0x4E == f[i + 2]) && (0x44 == f[i + 3])
            && (0xAE == f[i + 4]) && (0x42 == f[i + 5]) && (0x60 == f[i + 6]) && (0x82 == f[i + 7])) {
            printf("PNG tail\r\n");
            if ((i + 7) != len - 1) {
                printf("Some data after PNG tail\r\n");
            }
        }
    }

    /* BMP head */
    for (i = 0; i < len; i++) {
        if ((0x42 == f[i]) && (0x4D == f[i + 1])) {
            printf("BMP head\r\n");
        }
    }

    /* GIF head */
    for (i = 0; i < len; i++) {
        if ((0x47 == f[i]) && (0x49 == f[i + 1]) && (0x46 == f[i + 2]) && (0x38 == f[i + 3]) 
            && ((0x39 == f[i + 4]) || (0x37 == f[i + 4])) && (0x61 == f[i + 5])) {
            printf("GIF head\r\n");
        }
    }

    /* GIF tail */
    for (i = 0; i < len; i++) {
        if ((0x00 == f[i]) && (0x3B == f[i + 1])) {
            printf("GIF tail\r\n");
        }
    }

    /* ZIP head */
    for (i = 0; i < len; i++) {
        if ((0x50 == f[i]) && (0x4B == f[i + 1]) && (0x03 == f[i + 2]) && (0x04 == f[i + 3])) {
            printf("ZIP head\r\n");
        }
    }

    /* ZIP tail */
    for (i = 0; i < len; i++) {
        if ((0x50 == f[i]) && (0x4B == f[i + 1]) && (0x05 == f[i + 2]) && (0x06 == f[i + 3])) {
            printf("ZIP tail\r\n");
        }
    }

    /* RAR head */
    for (i = 0; i < len; i++) {
        if ((0x52 == f[i]) && (0x61 == f[i + 1]) && (0x72 == f[i + 2]) && (0x21 == f[i + 3]) && (0x1A == f[i + 4])
            && (0x07 == f[i + 5]) && (0x00 == f[i + 6])) {
            printf("RAR head\r\n");
        }
    }

    /* RAR tail */
    for (i = 0; i < len; i++) {
        if ((0xC4 == f[i]) && (0x3D == f[i + 1]) && (0x7B == f[i + 2]) && (0x00 == f[i + 3]) && (0x40 == f[i + 4])
            && (0x07 == f[i + 5]) && (0x00 == f[i + 6])) {
            printf("RAR tail\r\n");
        }
    }

    /* WAV head */
    for (i = 0; i < len; i++) {
        if ((0x52 == f[i]) && (0x49 == f[i + 1]) && (0x46 == f[i + 2]) && (0x46 == f[i + 3])) {
            printf("WAV head\r\n");
        }
    }

    return;
}

int main(int argc, char *argv[]) {
    int i = 0;
    int f_len = 0;
    unsigned char low4bits;
    unsigned char high4bits;
    FILE *fp = NULL;
    FILE *fp_re = NULL;         /* file reverse */
    FILE *fp_ex = NULL;         /* file reverse & exchange low and high 4 bits */
    FILE *fp_xor = NULL;        /* file xor 0xff */
    unsigned char *f = NULL;
    unsigned char *f_tmp = NULL;

    if (2 != argc) {
        printf("Miss args\r\n");
        return -1;
    }

    if (NULL == (fp = fopen(argv[1], "rb"))) {
        printf("File open error\r\n");
        return -1;
    }

    if (NULL == (fp_re = fopen("/share/1_Steg/file_reverse", "wb"))) {
        printf("File open error\r\n");
        fclose(fp);
        return -1;
    }

    if (NULL == (fp_ex = fopen("/share/1_Steg/file_reverse_exchange", "wb"))) {
        printf("File open error\r\n");
        fclose(fp);
        fclose(fp_re);
        return -1;
    }

    if (NULL == (fp_xor = fopen("/share/1_Steg/file_xor", "wb"))) {
        printf("File open error\r\n");
        fclose(fp);
        fclose(fp_re);
        fclose(fp_ex);
        return -1;
    }

    f = malloc(FILE_SIZE);
    if (NULL == f) {
        printf("Malloc error\r\n");
        fclose(fp);
        fclose(fp_re);
        fclose(fp_ex);
        fclose(fp_xor);
        return -1;
    }

    memset(f, 0, FILE_SIZE);

    f_len = fread(f, 1, FILE_SIZE, fp);
    if (f_len + 10 >= FILE_SIZE) {
        printf("File too large\r\n");
        free(f);
        fclose(fp);
        fclose(fp_re);
        fclose(fp_ex);
        fclose(fp_xor);
        return -1;
    }

    f_tmp = malloc(FILE_SIZE);
    if (NULL == f_tmp) {
        printf("Malloc error\r\n");
        free(f);
        fclose(fp);
        fclose(fp_re);
        fclose(fp_ex);
        fclose(fp_xor);
        return -1;
    }

    memset(f_tmp, 0, FILE_SIZE);

    /* normal file */
    printf("****************Normal file****************\r\n");
    identify(f, f_len);
    printf("\r\n\r\n");

    /* reverse file */
    memset(f_tmp, 0, FILE_SIZE);
    printf("****************Reverse file****************\r\n");
    for (i = 0; i < f_len; i++) {
        f_tmp[i] = f[f_len - i -1];
    }

    identify(f_tmp, f_len);
    fwrite(f_tmp, 1, f_len, fp_re);
    fflush(fp_re);
    printf("\r\n\r\n");

    /* reverse file & exchange bits */
    /* do not need to memset */
    printf("****************Reverse file & exchange bits****************\r\n");
    for (i = 0; i < f_len; i++) {
        low4bits    = f_tmp[i] & 0x0f;
        high4bits   = f_tmp[i] & 0xf0;
        f_tmp[i]    = (low4bits << 4) | (high4bits >> 4);
    }

    identify(f_tmp, f_len);
    fwrite(f_tmp, 1, f_len, fp_ex);
    fflush(fp_ex);
    printf("\r\n\r\n");

    /* xor file */
    memset(f_tmp, 0, FILE_SIZE);
    printf("****************XOR file****************\r\n");
    for (i = 0; i < f_len; i++) {
        f_tmp[i] = f[i] ^ 0xFF;
    }

    identify(f_tmp, f_len);
    fwrite(f_tmp, 1, f_len, fp_xor);
    fflush(fp_xor);

    free(f);
    free(f_tmp);
    fclose(fp);
    fclose(fp_re);
    fclose(fp_ex);
    fclose(fp_xor);

    printf("\r\nDone\r\n");
    return 0;
}

