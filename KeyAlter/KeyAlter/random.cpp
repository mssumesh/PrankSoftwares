#define _CRT_RAND_S

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "random.h"

unsigned int GetRandom ( UINT uiMin, UINT uiMax )
	{
	UINT uiRand, uiNumber;
	errno_t  err;

	err = rand_s ( &uiNumber );
	if (err != 0)
		{	
		srand( (unsigned)time( NULL ) );
		//uiRand = (UINT) ( ( (double)rand() / (double) RAND_MAX * uiMax )+ uiMin );
		uiRand = ((UINT)rand() + uiMin ) % ( uiMax + 1 ) ;
		}
	else
		//uiRand = (UINT) ( ( (double)uiNumber / (double) UINT_MAX * uiMax )  + uiMin );
		uiRand =   ((UINT) uiNumber + uiMin ) % ( uiMax + 1 );

	return uiRand;
	}






