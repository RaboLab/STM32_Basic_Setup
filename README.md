# STM32_Basic_Setup
Basic Setup with Vofa Serial, and normed folder


 - Debug
     - Vofa    上位机
 - Config
     - HardwareDef   硬件定义
     - Defination  全局定义
 - Kernel
     - loop 内核接管
     - loop_it 中断接管，接管stm32  systick函数，放置用户回调函数

如何使用本库：
1. 使用STM32CubeMX正常创建工程
2. 在NVIC 二级菜单 Code generator 取消Systick Generate IRQ
3. 复制上述文件夹到工程目录 ./Core/ 下
4. Keil mdk下魔法棒 选项菜单C/C++添加上述路径到 Include Path （例： ../Core/Debug）
5. 在Keil mdk左侧添加Group，并添加上述所有文件（例： HardwareDef.h Vofa.c   ）
6. 按需定义HardwareDef, 修改Defination.h
        

注：
        本套util库极其规范，使用前后台系统进行编程，如果需要移植到其他单片机上，请【尽量】按照保留代码风格。
        可以在魔法棒里把 ARM Compiler改成V6.13.1，可提高编译效率。
        vofa可使用printf、usb、usart方式
