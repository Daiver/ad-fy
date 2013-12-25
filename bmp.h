#ifndef BMP_H
#define BMP_H

#define BYTE unsigned char
#define BITMAP_HEADERS_LENGTH 54 //length of headers in bmp file
#define BITMAP_BIT_PALETTE_LENGTH 8 //CAUSE 

typedef struct TBITMAP{
  int size;
  const int headers;
  BYTE *data;
} BITMAP;


int readBitmap(BITMAP *bmp, const char *path);
BYTE getPixel(BITMAP *bmp, int index);
void printInfo(BITMAP *bmp);
void freeBitmap(BITMAP *bmp);


#endif
