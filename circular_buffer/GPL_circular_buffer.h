#ifndef GPL_CIRCULAR_BUFFER_H_
#define GPL_CIRCULAR_BUFFER_H_

#include <stdio.h>      
#include <stdlib.h>

typedef void*           GPL_CircularBuffer_t;    
GPL_CircularBuffer_t    *GPL_CircularBuffer_create          (size_t buferSize); 
size_t                  GPL_CircularBuffer_capacity         (GPL_CircularBuffer_t *handler);
size_t                  GPL_CircularBuffer_size             (GPL_CircularBuffer_t *handler); 
char                    GPL_CircularBuffer_read             (GPL_CircularBuffer_t *handler);
int                     GPL_CircularBuffer_destroy          (GPL_CircularBuffer_t *handler);
int                     GPL_CircularBuffer_full             (GPL_CircularBuffer_t *handler); 
int                     GPL_CircularBuffer_empty            (GPL_CircularBuffer_t *handler);
int                     GPL_CircularBuffer_write            (GPL_CircularBuffer_t *handler, char data);
void                    GPL_CircularBuffer_reset            (GPL_CircularBuffer_t *handler);
void                    GPL_CircularBuffer_readWord         (GPL_CircularBuffer_t *handler, char *string);                           

#endif  /* GPL_CIRCULAR_BUFFER_H_ */