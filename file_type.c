#include <stdio.h>
#include <stdlib.h>

#define FILE_SIZE (10 * 1024 * 1024) /* 10M */

int main(int argc, char *argv[]) {
	int i = 0;
	int f_len = 0;
	FILE *fp = NULL;
	unsigned char *f = NULL;

	if (2 != argc) {
		printf("Miss args\r\n");
		return -1;
	}
	
	if (NULL == (fp = fopen(argv[1], "r"))) {
		printf("File open error\r\n");
		return -1;
	}
	
	f = malloc(FILE_SIZE);
	if (NULL == f) {
		printf("Malloc error\r\n");
		fclose(fp);
		return -1;
	}
	
	f_len = fread(f, 1, FILE_SIZE, fp);
	if (f_len + 10 >= FILE_SIZE) {
		printf("File too large\r\n");
		free(f);
		fclose(fp);
		return -1;
	}
	
	/* JPG head */
	for (i = 0; i < f_len; i++) {
		if ((0xFF == f[i]) && (0xD8 == f[i + 1]) && (0xFF == f[i + 2]) && (0xE0 == (f[i + 3] & 0xF0))
			&& ((0x4A == f[i + 6]) || (0x45 == f[i + 6])) && ((0x46 == f[i + 7]) || (0x78 == f[i + 7])) 
			&& ((0x49 == f[i + 8]) || (0x69 == f[i + 8])) && ((0x46 == f[i + 9]) || (0x66 == f[i + 9]))) {
			printf("JPG head\r\n");
		}
	}
	
	/* JPG tail */
	for (i = 0; i < f_len; i++) {
		if ((0xFF == f[i]) && (0xD9 == f[i + 1])) {
			printf("JPG tail\r\n");
		}
	}
	
	/* PNG head */
	for (i = 0; i < f_len; i++) {
		if ((0x89 == f[i]) && (0x50 == f[i + 1]) && (0x4E == f[i + 2]) && (0x47 == f[i + 3])
			&& (0x0D == f[i + 4]) && (0x0A == f[i + 5]) && (0x1A == f[i + 6]) && (0x0A == f[i + 7])) {
			printf("PNG head\r\n");
		}
	}
	
	/* PNG tail */
	for (i = 0; i < f_len; i++) {
		if ((0x49 == f[i]) && (0x45 == f[i + 1]) && (0x4E == f[i + 2]) && (0x44 == f[i + 3])
			&& (0xAE == f[i + 4]) && (0x42 == f[i + 5]) && (0x60 == f[i + 6]) && (0x82 == f[i + 7])) {
			printf("PNG tail\r\n");
		}
	}
	
	/* BMP head */
	for (i = 0; i < f_len; i++) {
		if ((0x42 == f[i]) && (0x4D == f[i + 1])) {
			printf("BMP head\r\n");
		}
	}
	
	/* GIF head */
	for (i = 0; i < f_len; i++) {
		if ((0x47 == f[i]) && (0x49 == f[i + 1]) && (0x46 == f[i + 2]) && (0x38 == f[i + 3]) 
			&& ((0x39 == f[i + 4]) || (0x37 == f[i + 4])) && (0x61 == f[i + 5])) {
			printf("GIF head\r\n");
		}
	}

	/* GIF tail */
	for (i = 0; i < f_len; i++) {
		if ((0x00 == f[i]) && (0x3B == f[i + 1])) {
			printf("GIF tail\r\n");
		}
	}
		
	/* ZIP head */
	for (i = 0; i < f_len; i++) {
		if ((0x50 == f[i]) && (0x4B == f[i + 1]) && (0x03 == f[i + 2]) && (0x04 == f[i + 3])) {
			printf("ZIP head\r\n");
		}
	}
	
	/* ZIP tail */
	for (i = 0; i < f_len; i++) {
		if ((0x50 == f[i]) && (0x4B == f[i + 1]) && (0x05 == f[i + 2]) && (0x06 == f[i + 3])) {
			printf("ZIP tail\r\n");
		}
	}

	/* RAR head */
	for (i = 0; i < f_len; i++) {
		if ((0x52 == f[i]) && (0x61 == f[i + 1]) && (0x72 == f[i + 2]) && (0x21 == f[i + 3]) && (0x1A == f[i + 4])
			&& (0x07 == f[i + 5]) && (0x00 == f[i + 6])) {
			printf("RAR head\r\n");
		}
	}
	
	/* RAR tail */
	for (i = 0; i < f_len; i++) {
		if ((0xC4 == f[i]) && (0x3D == f[i + 1]) && (0x7B == f[i + 2]) && (0x00 == f[i + 3]) && (0x40 == f[i + 4])
			&& (0x07 == f[i + 5]) && (0x00 == f[i + 6])) {
			printf("RAR tail\r\n");
		}
	}
	
	/* WAV head */
	for (i = 0; i < f_len; i++) {
		if ((0x52 == f[i]) && (0x49 == f[i + 1]) && (0x46 == f[i + 2]) && (0x46 == f[i + 3])) {
			printf("WAV head\r\n");
		}
	}

	/* Reverse file */
	/* JPG reverse head */
	for (i = 0; i < f_len; i++) {
		if (((0x46 == f[i]) || (0x66 == f[i])) && ((0x49 == f[i + 1]) || (0x69 == f[i + 1])) 
			&& ((0x46 == f[i + 2]) || (0x78 == f[i + 2])) && ((0x4A == f[i + 3]) || (0x45 == f[i + 3])) 
			&& (0xE0 == (f[i + 6] & 0xF0)) && (0xFF == f[i + 7]) && (0xD8 == f[i + 8]) && (0xFF == f[i + 9])) {
			printf("JPG reverse head\r\n");
		}
	}

	/* JPG reverse tail */
	for (i = 0; i < f_len; i++) {
		if ((0xD9 == f[i]) && (0xFF == f[i + 1])) {
			printf("JPG reverse tail\r\n");
		}
	}
	
	/* PNG reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x0A == f[i]) && (0x1A == f[i + 1]) && (0x0A == f[i + 2]) && (0x0D == f[i + 3])
			&& (0x47 == f[i + 4]) && (0x4E == f[i + 5]) && (0x50 == f[i + 6]) && (0x89 == f[i + 7])) {
			printf("PNG reverse head\r\n");
		}
	}
	
	/* PNG reverse tail */
	for (i = 0; i < f_len; i++) {
		if ((0x82 == f[i]) && (0x60 == f[i + 1]) && (0x42 == f[i + 2]) && (0xAE == f[i + 3])
			&& (0x44 == f[i + 4]) && (0x4E == f[i + 5]) && (0x45 == f[i + 6]) && (0x49 == f[i + 7])) {
			printf("PNG reverse tail\r\n");
		}
	}
	
	/* BMP reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x4D == f[i]) && (0x42 == f[i + 1])) {
			printf("BMP reverse head\r\n");
		}
	}
	
	/* GIF reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x61 == f[i]) && ((0x39 == f[i + 1]) || (0x37 == f[i + 1])) && (0x38 == f[i + 2]) 
			&& (0x46 == f[i + 3]) && (0x49 == f[i + 4]) && (0x47 == f[i + 5])) {
			printf("GIF reverse head\r\n");
		}
	}
	
	/* ZIP reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x04 == f[i]) && (0x03 == f[i + 1]) && (0x4B == f[i + 2]) && (0x50 == f[i + 3])) {
			printf("ZIP reverse head\r\n");
		}
	}
	
	/* ZIP reverse tail */
	for (i = 0; i < f_len; i++) {
		if ((0x06 == f[i]) && (0x05 == f[i + 1]) && (0x4B == f[i + 2]) && (0x50 == f[i + 3])) {
			printf("ZIP reverse tail\r\n");
		}
	}
	
	/* RAR reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x00 == f[i]) && (0x07 == f[i + 1]) && (0x1A == f[i + 2]) && (0x21 == f[i + 3]) && (0x72 == f[i + 4]) 
			&& (0x61 == f[i + 3]) && (0x52 == f[i + 4])) {
			printf("RAR reverse head\r\n");
		}
	}
	
	/* RAR reverse tail */
	for (i = 0; i < f_len; i++) {
		if ((0x00 == f[i]) && (0x07 == f[i + 1]) && (0x40 == f[i + 2]) && (0x00 == f[i + 3]) && (0x7B == f[i + 4])
			&& (0x3D == f[i + 5]) && (0xC4 == f[i + 6])) {
			printf("RAR reverse tail\r\n");
		}
	}
	
	/* WAV reverse head */
	for (i = 0; i < f_len; i++) {
		if ((0x46 == f[i]) && (0x46 == f[i + 1]) && (0x49 == f[i + 2]) && (0x52 == f[i + 3])) {
			printf("WAV reverse head\r\n");
		}
	}

	free(f);
	fclose(fp);
	
	printf("Done\r\n");
	return 0;
}
