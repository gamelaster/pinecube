#ifndef _ISP_H_
#define _ISP_H_

#include <assert.h>

typedef struct 
{
	int baseAddress; // field_0 - None
	char gap4[299];
	char field_12F; // field_12F - None
} isp_context;
_Static_assert(sizeof(isp_context) == 0x130,"isp_context doesn't have correct size");

isp_context* IspCreate();
isp_context* SetIspBaseAddress(isp_context *isp, int baseAddress);

#endif