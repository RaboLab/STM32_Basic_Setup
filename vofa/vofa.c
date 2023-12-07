#include "vofa.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "SEGGER_RTT.h"


#ifdef VOFA_RUN_USB
	#include "usbd_cdc_if.h"
#else
  #define VOFA_RUN_UART
  #include "usart.h"
  #ifndef VOFA_UART
    #error "No Define VOFA_UART"
  #endif
#endif

uint8_t vofaMemPool[ VOFA_CHANNEL_COUNT * 4 + 4];

void vofa_init() {
  vofa_fdata_config();
}
static void vofa_fdata_config(void);

////////////////////////////////////////////////////////////////////////////
static void vofa_send_method(uint8_t *mem, uint16_t data_bytes) 
{
	#ifdef VOFA_RUN_USB
		CDC_Transmit_FS(mem, data_bytes);
	#endif
	#ifdef VOFA_RUN_UART
		HAL_UART_Transmit(&VOFA_UART, mem, data_bytes,0xff); //256ms Overtime
	#endif
}

////////////////////////////////////////////////////////////////////////////

static void vofa_printf(const char * format, ...) {
  va_list args;
  uint32_t len;
  uint8_t buff[200];  // 200 Bytes

  va_start(args, format);
  len = vsnprintf((char *)buff, 200, (char *)format, args);
  va_end(args);
  SEGGER_RTT_printf(0, (char *)buff);
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
  vofa.send(vofaMemPool, 4 * (chan_count + 1));
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
  vofa.send((uint8_t *)&(vofa.pic), 28);  // 7*4   send head
  vofa.send(pic_ptr, vofa.pic.IMG_SIZE);
}

vofa_t vofa = {
    .pic.config = vofa_pic_config,
    .pic.send = vofa_pic_send,

    .fdata.config = vofa_fdata_config,
    .fdata.set = vofa_fdata_set_data,
    .fdata.send = vofa_fdata_send,

    .printf = vofa_printf,
};
