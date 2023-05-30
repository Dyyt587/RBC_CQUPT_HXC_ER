#include "kfifo.h"
#include <string.h>
 
 
#define min(a,b) ((a)<(b)?(a):(b))
 
/* is x a power of 2? */
#define is_power_of_2(x)	((x) != 0 && (((x) & ((x) - 1)) == 0))
 
/**
 * kfifo_init - initialize a FIFO using a preallocated buffer
 * @fifo: the fifo to assign the buffer
 * @buffer: the preallocated buffer to be used.
 * @size: the size of the internal buffer, this has to be a power of 2.
 *
 */
void kfifo_init(struct kfifo *fifo, uint8_t *buffer, unsigned int size)
{
	/* TODO: size must be a power of 2? */
 
	fifo->buffer = buffer;
	fifo->size = size;
 
	kfifo_reset(fifo);
}
 
static inline void __kfifo_in_data(struct kfifo *fifo,
		const uint8_t *from, unsigned int len, unsigned int off)
{
	unsigned int l;
 
	/*
	 * Ensure that we sample the fifo->out index -before- we
	 * start putting bytes into the kfifo.
	 */
 
	off = __kfifo_off(fifo, fifo->in + off);
 
	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - off);
	memcpy(fifo->buffer + off, from, l);
 
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, from + l, len - l);
}
 
static inline void __kfifo_out_data(struct kfifo *fifo,
		uint8_t *to, unsigned int len, unsigned int off)
{
	unsigned int l;
 
	/*
	 * Ensure that we sample the fifo->in index -before- we
	 * start removing bytes from the kfifo.
	 */
 
	off = __kfifo_off(fifo, fifo->out + off);
 
	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - off);
	memcpy(to, fifo->buffer + off, l);
 
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(to + l, fifo->buffer, len - l);
}
 
unsigned int __kfifo_in_n(struct kfifo *fifo,
	const uint8_t *from, unsigned int len, unsigned int recsize)
{
	if (kfifo_avail(fifo) < len + recsize)
		return len + 1;
 
	__kfifo_in_data(fifo, from, len, recsize);
	return 0;
}
 
 
/**
 * kfifo_in - puts some data into the FIFO
 * @fifo: the fifo to be used.
 * @from: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most @len bytes from the @from buffer into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
unsigned int kfifo_in(struct kfifo *fifo, const uint8_t *from,
				unsigned int len)
{
	len = min(kfifo_avail(fifo), len);
 
	__kfifo_in_data(fifo, from, len, 0);
	__kfifo_add_in(fifo, len);
	return len;
}
unsigned int __kfifo_out_n(struct kfifo *fifo,
	uint8_t *to, unsigned int len, unsigned int recsize)
{
	if (kfifo_len(fifo) < len + recsize)
		return len;
 
	__kfifo_out_data(fifo, to, len, recsize);
	__kfifo_add_out(fifo, len + recsize);
	return 0;
}
 
/**
 * kfifo_out - gets some data from the FIFO
 * @fifo: the fifo to be used.
 * @to: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most @len bytes from the FIFO into the
 * @to buffer and returns the number of copied bytes.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
unsigned int kfifo_out(struct kfifo *fifo, uint8_t *to, unsigned int len)
{
	len = min(kfifo_len(fifo), len);
 
	__kfifo_out_data(fifo, to, len, 0);
	__kfifo_add_out(fifo, len);
 
	return len;
}
