#include "GPL_scratch_pad_protocle.h"

enum scratchPad_e {IDLE, PROCESS, READ, WRITE_ADDRESS, WRITE_VALUE};
	
typedef struct {
	enum scratchPad_e state;
	size_t scratchPad_size;
	char *scratchPad;
}__GPL_ScratchPad_t;



GPL_ScratchPad_t *GPL_ScratchPad_create( char * const scratchPad, size_t scratchPad_size ) {
	if ( scratchPad_size < 1 || scratchPad == NULL )
		return NULL;

	__GPL_ScratchPad_t *handler     = ( __GPL_ScratchPad_t * ) malloc( sizeof( *handler ) );
	handler->state					= IDLE;
	handler->scratchPad             = scratchPad;
	handler->scratchPad_size        = scratchPad_size;


	return ( GPL_ScratchPad_t )handler;
}

void GPL_ScratchPad_reset( GPL_ScratchPad_t *handler ) {
	__GPL_ScratchPad_t *__handler = ( __GPL_ScratchPad_t * ) handler;
	__handler->state = IDLE;
}

void GPL_ScratchPad_destroy( GPL_ScratchPad_t *handler ) {
	__GPL_ScratchPad_t *__handler = ( __GPL_ScratchPad_t * ) handler;
	free( __handler );
}

static void _GPL_ScratchPad_write( GPL_ScratchPad_t *handler, char byte ) {
	__GPL_ScratchPad_t *__handler = ( __GPL_ScratchPad_t * ) handler;
	static char address;
	
	if( __handler->state == WRITE_ADDRESS ) {
		address = byte - __handler->scratchPad_size;
		__handler->state = WRITE_VALUE;
		return;
	}
	
	if( __handler->state == WRITE_VALUE ) {
		__handler->scratchPad[(size_t) address] = byte;
		__handler->state = IDLE;
	}
}

int GPL_ScratchPad_processByte( GPL_ScratchPad_t *handler, char byte ) {
	__GPL_ScratchPad_t *__handler = ( __GPL_ScratchPad_t * ) handler;
	
	if( __handler->state == IDLE && byte < __handler->scratchPad_size )
		__handler->state = READ;
	
	if( __handler->state == IDLE && byte >= __handler->scratchPad_size  && byte < ( __handler->scratchPad_size * 2 ) )
		__handler->state = WRITE_ADDRESS;
	
	if (__handler->state == WRITE_ADDRESS || __handler->state == WRITE_VALUE)
		_GPL_ScratchPad_write( handler, byte );
	
	return __handler->state == READ ? 1 : 0;
}


char GPL_ScratchPad_read( GPL_ScratchPad_t *handler, char address ) {
	__GPL_ScratchPad_t *__handler = ( __GPL_ScratchPad_t * ) handler;
	
	__handler->state = IDLE;
	
	return __handler->scratchPad[(size_t) address];
}

