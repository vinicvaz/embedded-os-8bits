/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */


#ifndef MM_H
#define	MM_H

#include "types.h"

#define	MAX_HEAP_SIZE               0x200

#define NEAR

#define	_MAX_SEGMENT_SIZE           0x7F
#define _MAX_HEAP_SIZE              MAX_HEAP_SIZE-1


/*********************************************************************
 * Segment header data type
 ********************************************************************/
typedef union _SALLOC
{
	u_char byte;
	struct _BITS
	{
		unsigned count:7;
		unsigned alloc:1;	
	}bits;
}SALLOC;


/*********************************************************************
 * Set the memory type
 ********************************************************************/
#if defined(NEAR_MODEL)
#pragma		udata access	_SRAM_ALLOC
#else
#pragma		udata _SRAM_ALLOC
#endif


u_char* SRAMalloc(u_char nBytes);
void SRAMfree(u_char* pSRAM);
void SRAMInitHeap(void);
NEAR u_char _SRAMmerge(SALLOC * NEAR pSegA);


#endif	/* MM_H */

