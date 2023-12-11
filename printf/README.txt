1. 使用printf打印%f时需要注意。有一些帖子提到了FreeRTOS+FPU+打印浮点数可能有问题：https://forum.digikey.com/t/easily-use-printf-on-stm32/20157
2. 如何print_to_uart():
    1. #include <stdio.h>
    2. 添加下面的语句：
===== BEGIN =====
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart?, (uint8_t *)&ch, 1, 0xFF);  // modify this to correct huart_handle
  return ch;
}
=====   END  =====
个人感觉其实效率挺低，我宁愿自己用vnsprinf()