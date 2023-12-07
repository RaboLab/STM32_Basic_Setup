#ifndef __DATA_FIFO_H
#define __DATA_FIFO_H

#include "main.h"

#define  USE_MUTEX   0

#if USE_MUTEX
#define  MUTEX_TIMEOUT   10     
#define  MUTEX_T         osMutexId_t  
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ringbuff 
{
	uint8_t *buffer;  	/* buffer_pointer */
	uint32_t size;      /* size = 2**n */
	uint32_t in;        /* (in % size) */
	uint32_t out;       /* (out % size) */
#if USE_MUTEX
	MUTEX_T *mutex;       /* ֧��rtos�Ļ��� */
#endif
}RingBuff_t ;

extern int32_t Create_RingBuff(RingBuff_t *rb, 
                             uint8_t *buffer,
                             uint32_t size
                            );
extern uint32_t Write_RingBuff(RingBuff_t *rb,
                               uint8_t *buffer, 
                               uint32_t len);	
extern uint32_t Read_RingBuff(RingBuff_t *rb,
                              uint8_t *buffer, 
                              uint32_t len);
extern uint32_t CanRead_RingBuff(RingBuff_t *rb);
extern uint32_t CanWrite_RingBuff(RingBuff_t *rb);
#endif /* __DATA_FIFO_H */
