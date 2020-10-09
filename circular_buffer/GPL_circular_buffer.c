#include "GPL_circular_buffer.h"  

typedef struct{
    size_t  bufferSize;
    size_t  readIndex;
    size_t  writeIndex;
    int     full;
    char    *buffer;
}__GPL_CircularBuffer_t; 

GPL_CircularBuffer_t *GPL_CircularBuffer_create(size_t size){
    if(size < 1)
        return NULL;

    __GPL_CircularBuffer_t *handler = (__GPL_CircularBuffer_t *) malloc(sizeof(*handler));
    handler->buffer                 = (char *) calloc(size, sizeof(char *));
    handler->bufferSize             = size;
    handler->readIndex              = 0;
    handler->writeIndex             = 0;
    handler->full                   = 0;

    return (GPL_CircularBuffer_t)handler;
}  

char GPL_CircularBuffer_read(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;
    char data = -1;                                                
    if(!GPL_CircularBuffer_empty(handler)){
        data = __handler->buffer[__handler->readIndex];
        __handler->readIndex = (__handler->readIndex + 1) % __handler->bufferSize;
        __handler->full = 0;
    }
    return data;
}

size_t GPL_CircularBuffer_capacity(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;

    return __handler->bufferSize;
}

size_t GPL_CircularBuffer_size(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;

    size_t size = __handler->bufferSize;
    if(!__handler->full){  
        if(__handler->writeIndex >= __handler->readIndex)
            size = __handler->writeIndex - __handler->readIndex;
        else
            size = size + __handler->writeIndex - __handler->readIndex;
    }

    return size;
}

int GPL_CircularBuffer_write(GPL_CircularBuffer_t *handler, char data){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;
    if(!GPL_CircularBuffer_full(handler)){
        __handler->buffer[__handler->writeIndex] = data;
        __handler->writeIndex = (__handler->writeIndex + 1) % __handler->bufferSize;
        __handler->full = (__handler->writeIndex == __handler->readIndex);
        return 1;
    }else{
        return 0;
    }
}   

void GPL_CircularBuffer_reset(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;

    __handler->readIndex = 0;
    __handler->writeIndex = 0;
    __handler->full = 0;
}

void GPL_CircularBuffer_readWord(GPL_CircularBuffer_t *handler, char *string){
    while(!GPL_CircularBuffer_empty(handler)){
        *string = GPL_CircularBuffer_read(handler);
        string++;
        *string = '\0';
    }
}

int GPL_CircularBuffer_destroy(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;
    free(__handler->buffer);
    free(__handler);

    return 1;
}

int GPL_CircularBuffer_full(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;
    if(__handler->full)
        return 1;
    else
        return 0;
}

int GPL_CircularBuffer_empty(GPL_CircularBuffer_t *handler){
    __GPL_CircularBuffer_t *__handler = (__GPL_CircularBuffer_t *) handler;
    if((__handler->readIndex == __handler->writeIndex) && !__handler->full)
        return 1;
    else
        return 0;
}
