#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"

int fileSize(FILE *f){
  fseek(f, 0L, SEEK_END);
  int size = ftell(f);
  fseek(f, 0L, SEEK_SET);
  return size;
}

int seekPixels(FILE *f){
  if(!f)
    return -1;
  const int offset = BITMAP_HEADERS_LENGTH + BITMAP_BIT_PALETTE_LENGTH;
  const int length = fileSize(f) - offset;
  fseek(f, offset, SEEK_SET);
  return length;
}

int readBitmap(BITMAP *bmp, const char *path){
  if(!path || !bmp)
    return -1;
  bmp->size = 0;
  bmp->data = NULL;
  FILE *file = fopen(path, "r");
  if(!file)
    return -1;
  bmp->size = seekPixels(file);
  if(bmp->size > 0){
    bmp->data = (BYTE *) malloc(bmp->size);
    fread(bmp->data, sizeof(BYTE), bmp->size, file);
  }
  fclose(file);
  return 0;
}

void freeBitmap(BITMAP *bmp){
  if(!bmp->data)
    free(bmp->data);
}

BYTE getPixel(BITMAP *bmp, int index){
  int nbyte =  index / 8;
  int nbit = index % 8;
  BYTE pixel =  ((bmp->data[nbyte] << nbit) & 128) / 128;
//  printf("[index = %d : byte = %d: bit = %d] = %d\n", index, nbyte, nbit, pixel);
  return pixel;
}

void printInfo(BITMAP *bmp){
  printf("SIZE=%d\n", bmp->size);
  int i;
  for(i = 0; i < bmp->size * 8; i+=8){
    printf("%x: ", bmp->data[i/8]);
    int j;
    for(j = 0; j < 8; ++j)
      printf("%d",getPixel(bmp, i+j));
    printf("\n");
  }
  printf("\n\n");
}


