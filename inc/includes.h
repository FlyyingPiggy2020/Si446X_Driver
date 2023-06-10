#ifndef INCLUDES_H
#define INCLUDES_H
 


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

//typedef bit BIT;
//typedef unsigned char BIT;

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
		U16 U16;
		S16 S16;
		U8 U8[2];
		S8 S8[2];
} UU16;

typedef union UU32
{
		U32 U32;
		S32 S32;
		UU16 UU16[2];
		U16 U16[2];
		S16 S16[2];
		U8 U8[4];
		S8 S8[4];
} UU32;

#endif
	
