#include "ringbuff.h"
#include "string.h"
#define kmalloc(size, mask) 		malloc(size)
#define kfree(ptr) 							free(ptr)
#define BUG_ON(cond) 						ASSERT(!(cond))

#define min(a,b)  ((a)>(b) ? (b) : (a)) 

#if USE_MUTEX

static int32_t create_mutex(MUTEX_T *mutex)
{
	*mutex = osMutexNew(NULL);	/* FreeRTOS */
	return (*mutex != NULL);
}

static int32_t deleta_mutex(MUTEX_T *mutex)
{
	return osMutexDelete(*mutex);
}

static int32_t request_mutex(MUTEX_T *mutex)
{
	return osMutexAcquire(*mutex, MUTEX_TIMEOUT);
}

static int32_t release_mutex(MUTEX_T *mutex)
{
	return osMutexRelease(*mutex);
}
#endif
/*********************************** mutex **************************************************/

/*********************************** data **************************************************/
static int32_t fls(int32_t x)
{
  int r = 32;

  if (!x)
          return 0;
  if (!(x & 0xffff0000u)) {
          x <<= 16;
          r -= 16;
  }
  if (!(x & 0xff000000u)) {
          x <<= 8;
          r -= 8;
  }
  if (!(x & 0xf0000000u)) {
          x <<= 4;
          r -= 4;
  }
  if (!(x & 0xc0000000u)) {
          x <<= 2;
          r -= 2;
  }
  if (!(x & 0x80000000u)) {
          x <<= 1;
          r -= 1;
  }
  return r;
}

static unsigned long roundup_pow_of_two(unsigned long x)
{
	return (1 << (fls(x-1)-1));				
}


int32_t Create_RingBuff(RingBuff_t* rb, 
                      uint8_t *buffer,
                      uint32_t size
								)
{
	if((rb == NULL)||(buffer == NULL)||(size == 0))
	{
		return -1;
	}
	
	if(size&(size - 1))
	{
		size = roundup_pow_of_two(size);
	}

	rb->buffer = buffer;
	rb->size = size;
	rb->in = rb->out = 0;
#if USE_MUTEX	

  if(!create_mutex(rb->mutex))
  {
    return -2;
  }
#endif
	return 0;
}


int32_t Delete_RingBuff(RingBuff_t *rb)
{
	return -1;
}

uint32_t Write_RingBuff(RingBuff_t *rb,
                        uint8_t *wbuff, 
                        uint32_t len)
{
  uint32_t l;
#if USE_MUTEX
  
  if(!request_mutex(rb->mutex))
  {
    return -1;
  }
  else
  {
#endif
    len = min(len, rb->size - rb->in + rb->out);

    l = min(len, rb->size - (rb->in & (rb->size - 1)));
    memcpy(rb->buffer + (rb->in & (rb->size - 1)), wbuff, l);


    memcpy(rb->buffer, wbuff + l, len - l);

    rb->in += len;
#if USE_MUTEX
  }
  release_mutex(rb->mutex);
#endif
  return len;
}

uint32_t Read_RingBuff(RingBuff_t *rb,
                       uint8_t *rbuff, 
                       uint32_t len)
{
  uint32_t l;
#if USE_MUTEX
  if(!request_mutex(rb->mutex))
  {
    return -1;
  }
  else
  {
#endif
    len = min(len, rb->in - rb->out);

    l = min(len, rb->size - (rb->out & (rb->size - 1)));
    memcpy(rbuff, rb->buffer + (rb->out & (rb->size - 1)), l);
    memcpy(rbuff + l, rb->buffer, len - l);

    rb->out += len;

#if USE_MUTEX
  }
  release_mutex(rb->mutex);
#endif
  return len;
}


uint32_t CanRead_RingBuff(RingBuff_t *rb)
{
	if(NULL == rb)
	{
		return 0;
	}
	if(rb->in == rb->out)
		return 0;
	
	if(rb->in > rb->out)
		return (rb->in - rb->out);
	
	return (rb->size - (rb->out - rb->in));
}


uint32_t CanWrite_RingBuff(RingBuff_t *rb)
{
	if(NULL == rb)
	{
		return 0;
	}

	return (rb->size - CanRead_RingBuff(rb));
}

/******************************** DEMO *****************************************************/
//	RingBuff_t ringbuff_handle;
//	
//	uint8_t rb[64];
//	uint8_t res[64];
//	Create_RingBuff(&ringbuff_handle, 
//								rb,
//								sizeof(rb));
//			Write_RingBuff(&ringbuff_handle,
//                     res, 
//                     datapack.data_length);
//			
//			PRINT_DEBUG("CanRead_RingBuff = %d!",CanRead_RingBuff(&ringbuff_handle));
//			PRINT_DEBUG("CanWrite_RingBuff = %d!",CanWrite_RingBuff(&ringbuff_handle));
//			
//			Read_RingBuff(&ringbuff_handle,
//                     res, 
//                     datapack.data_length);
/******************************** DEMO *****************************************************/
