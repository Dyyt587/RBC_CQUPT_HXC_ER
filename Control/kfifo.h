#ifndef _KFIFO_H
#define _KFIFO_H
 
#include <stddef.h>
typedef unsigned char uint8_t;

struct kfifo {
	unsigned char *buffer;	/* the buffer holding the data */
	unsigned int size;	/* the size of the allocated buffer */
	unsigned int in;	/* data is added at offset (in % size) */
	unsigned int out;	/* data is extracted from off. (out % size) */
};
 
/*
 * Macros for declaration and initialization of the kfifo datatype
 */
 
 
extern void kfifo_init(struct kfifo *fifo, uint8_t *buffer,
			unsigned int size);
extern unsigned int kfifo_in(struct kfifo *fifo,
				const uint8_t *from, unsigned int len);
extern unsigned int kfifo_out(struct kfifo *fifo,
				uint8_t *to, unsigned int len);
 
/**
 * kfifo_initialized - Check if kfifo is initialized.
 * @fifo: fifo to check
 * Return %true if FIFO is initialized, otherwise %false.
 * Assumes the fifo was 0 before.
 */
static inline int kfifo_initialized(struct kfifo *fifo)
{
	return fifo->buffer != NULL;
}
 
/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
static inline void kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}
 
/**
 * kfifo_reset_out - skip FIFO contents
 * @fifo: the fifo to be emptied.
 */
static inline void kfifo_reset_out(struct kfifo *fifo)
{
	fifo->out = fifo->in;
}
 
/**
 * kfifo_size - returns the size of the fifo in bytes
 * @fifo: the fifo to be used.
 */
static inline unsigned int kfifo_size(struct kfifo *fifo)
{
	return fifo->size;
}
 
/**
 * kfifo_len - returns the number of used bytes in the FIFO
 * @fifo: the fifo to be used.
 */
static inline unsigned int kfifo_len(struct kfifo *fifo)
{
	register unsigned int	out;
 
	out = fifo->out;
	
	return fifo->in - out;
}
 
/**
 * kfifo_is_empty - returns true if the fifo is empty
 * @fifo: the fifo to be used.
 */
static inline int kfifo_is_empty(struct kfifo *fifo)
{
	return fifo->in == fifo->out;
}
 
/**
 * kfifo_is_full - returns true if the fifo is full
 * @fifo: the fifo to be used.
 */
static inline int kfifo_is_full(struct kfifo *fifo)
{
	return kfifo_len(fifo) == kfifo_size(fifo);
}
 
/**
 * kfifo_avail - returns the number of bytes available in the FIFO
 * @fifo: the fifo to be used.
 */
static inline unsigned int kfifo_avail(struct kfifo *fifo)
{
	return kfifo_size(fifo) - kfifo_len(fifo);
}
 
extern void kfifo_skip(struct kfifo *fifo, unsigned int len);
 
/*
 * __kfifo_add_out internal helper function for updating the out offset
 */
static inline void __kfifo_add_out(struct kfifo *fifo,
				unsigned int off)
{
	fifo->out += off;
}
 
/*
 * __kfifo_add_in internal helper function for updating the in offset
 */
static inline void __kfifo_add_in(struct kfifo *fifo,
				unsigned int off)
{
	fifo->in += off;
}
 
/*
 * __kfifo_off internal helper function for calculating the index of a
 * given offeset
 */
static inline unsigned int __kfifo_off(struct kfifo *fifo, unsigned int off)
{
	return off & (fifo->size - 1);
}
 
 
 
#endif	/* _KFIFO_H */
