#include "my_printf.h"
#include "stdarg.h"
#if defined __USART_H__
#include "usart.h"
#endif
#if defined __USB_CDC_H__
#include "usb_cdc_if.h"
#endif
#if defined SEGGER_RTT_H
#include "SEGGER_RTT.h"
#endif


#if defined __USART_H__
void uart_printf(UART_HandleTypeDef *huart, const char *format, ...)
{
  va_list args;
  uint32_t len;
  uint8_t buff[64]; // 64 Bytes

  va_start(args, format);
  len = vsnprintf((char *)buff, 64, (char *)format, args);
  va_end(args);

  HAL_UART_Transmit(huart, buff, len, 0xFF);
}
#endif


#if defined __USB_CDC_H__
void usb_printf(const char *format, ...)
{
  va_list args;
  uint32_t len;
  uint8_t buff[64]; // 64 Bytes

  va_start(args, format);
  len = vsnprintf((char *)buff, 64, (char *)format, args);
  va_end(args);

  // hal_usb_cdc_...;
}
#endif


#if defined SEGGER_RTT_H
inline void rtt_printf(uint8_t BufferIndex, const char *format, ...)
{
  int r;
  va_list ParamList;

  va_start(ParamList, sFormat);
  r = SEGGER_RTT_vprintf(BufferIndex, sFormat, &ParamList);
  va_end(ParamList);
  return r;
}
#endif


