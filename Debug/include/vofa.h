#ifndef __VOFA_H_
#define __VOFA_H_

#include <stdint.h>

#define VOFA_CHANNEL_COUNT 16
//  0 -- 15

#define VOFA_RUN_USB  
//#define VOFA_RUN_UART 



void vofa_init_method(void);

struct picture {
  int IMG_ID;
  int IMG_SIZE;   // N bytes, not WIDTH*HEIGHT
  int IMG_WIDTH;
  int IMG_HEIGHT;
  int IMG_FORMAT;
  int temp[2];

  void (*config)(int IMG_ID, int IMG_SIZE, int IMG_WIDTH, int IMG_HEIGHT,
                 int IMG_FORMAT);
  void (*send)(void *pic_ptr);
};

struct floatdata {
  float value[VOFA_CHANNEL_COUNT];
  uint8_t tail[4];

  void (*config)();
  void (*set)(uint8_t chan_no, float fdata);
  void (*send)(uint8_t chan_count);
};

typedef struct {
  struct picture pic;
  struct floatdata fdata;
  // user SHOULD complete thoes two methods following below
  void (*init)(void);
  void (*send)(uint16_t data_byte, uint8_t *mem);
} vofa_t;

extern vofa_t vofa;
extern int fputc(int ch, FILE *f);

#endif
