#ifndef GPL_SCRATCH_PAD_PROTOCLE_H_
#define GPL_SCRATCH_PAD_PROTOCLE_H_

#include <stdlib.h>

typedef void*           GPL_ScratchPad_t;
GPL_ScratchPad_t        *GPL_ScratchPad_create          (char * const scratchPad, size_t scratchPad_size);
int						GPL_ScratchPad_processByte      (GPL_ScratchPad_t *handler, char byte);
void                    GPL_ScratchPad_reset            (GPL_ScratchPad_t *handler);
void                    GPL_ScratchPad_destroy          (GPL_ScratchPad_t *handler);
char					GPL_ScratchPad_read				( GPL_ScratchPad_t *handler, char address );




#endif