#include "Defination.h"
#include "vofa.h"

#ifdef VOFA_RUN_UART
  #ifndef VOFA_UART
    #error "No Define VOFA_UART"
  #endif
#endif

uint8_t vofaMemPool[ VOFA_CHANNEL_COUNT * 4 + 4];

static void vofa_fdata_config(void);

////////////////////////////////////////////////////////////////////////////
#ifdef VOFA_RUN_UART
static void vofa_uart_method(uint16_t data_bytes, uint8_t *mem) {
  // printf("%s\n", mem);
	HAL_UART_Transmit_DMA(&VOFA_UART,mem,data_bytes);
}
#endif

#ifdef VOFA_RUN_USB
#include "stdarg.h"

static void vofa_usb_method(uint16_t data_bytes, uint8_t *mem) {
  // printf("%s\n", mem);
	// HAL_UART_Transmit_DMA(&VOFA_UART,mem,data_bytes);
  CDC_Transmit_FS(mem, data_bytes);
}

void usb_printf(const uint8_t * format, ...) {
  va_list args;
  uint32_t len;
  uint8_t buff[200];  // 200 Bytes

  va_start(args, format);
  len = vsnprintf((uint8_t *)buff, 200, (uint8_t *)format, args);
  va_end(args);
  CDC_Transmit_FS(buff, len);
}
#endif
////////////////////////////////////////////////////////////////////////////

static void vofa_init_method() {
  vofa_fdata_config();
}

static void vofa_fdata_config(void) {
  vofa.fdata.tail[0] = 0x00;
  vofa.fdata.tail[1] = 0x00;
  vofa.fdata.tail[2] = 0x80;
  vofa.fdata.tail[3] = 0x7f;
}

static void vofa_fdata_set_data(uint8_t chan_no, float fdata) {
  vofa.fdata.value[chan_no] = fdata;
}

static void vofa_fdata_send(uint8_t chan_count) {   // if cnt = 3, then send 0, 1, 2
  memcpy(vofaMemPool, vofa.fdata.value, chan_count * 4);
  memcpy(vofaMemPool + chan_count * 4, vofa.fdata.tail, 4);
  vofa.send(4 * (chan_count + 1), vofaMemPool);
}

static void vofa_pic_config(int IMG_ID, int IMG_SIZE, int IMG_WIDTH,
                            int IMG_HEIGHT, int IMG_FORMAT) {
  vofa.pic.IMG_FORMAT = IMG_FORMAT;
  vofa.pic.IMG_ID = IMG_ID;
  vofa.pic.IMG_SIZE = IMG_SIZE;
  vofa.pic.IMG_WIDTH = IMG_WIDTH;
  vofa.pic.IMG_HEIGHT = IMG_HEIGHT;
  vofa.pic.temp[0] = 0x7F800000;
  vofa.pic.temp[1] = 0x7F800000;
  // https://www.vofa.plus/plugin_detail/?name=JustFloat
}

static void vofa_pic_send(void *pic_ptr) {
  vofa.send(28, (uint8_t *)&(vofa.pic));  // 7*4 
  vofa.send(vofa.pic.IMG_SIZE, pic_ptr);
}

#ifdef VOFA_RUN_UART
vofa_t vofa = {
    .pic.config = vofa_pic_config,
    .pic.send = vofa_pic_send,

    .fdata.config = vofa_fdata_config,
    .fdata.set = vofa_fdata_set_data,
    .fdata.send = vofa_fdata_send,

    .init = vofa_init_method,
    .send = vofa_uart_method,
};
#endif

#ifdef VOFA_RUN_USB
vofa_t vofa = {
    .pic.config = vofa_pic_config,
    .pic.send = vofa_pic_send,

    .fdata.config = vofa_fdata_config,
    .fdata.set = vofa_fdata_set_data,
    .fdata.send = vofa_fdata_send,

    .init = vofa_init_method,
    .send = vofa_usb_method,
};
#endif

int fputc(int ch, FILE *f)
{
	uint8_t tem=ch;
  vofa.send(1,&tem);
	return ch;
}
