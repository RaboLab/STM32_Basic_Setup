#ifndef __VOFA_H_
#define __VOFA_H_

#include <stdint.h>
#include <stdio.h>


#define VOFA_CHANNEL_COUNT 4

//  0 -- 15
struct picture {
  int IMG_ID;
  int IMG_SIZE;   
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
  uint8_t vofa_run_type;  // 1: UART, 2: USB, 3: RTT, 0: no_init
#ifdef __USART_H__
  UART_HandleTypeDef * vofa_uart;
#endif 
  struct picture pic;
  struct floatdata fdata;
  void (*printf)(const char * format, ...);
  // user SHOULD complete thoes two methods following below
  void (*init)(void);
  void (*send)(uint8_t *mem, uint16_t data_byte);
} vofa_t;

extern vofa_t vofa;

#endif
