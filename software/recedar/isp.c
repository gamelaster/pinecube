#include "isp.h"
#include "recedar_hooks.h"
#include <stdio.h>

// -RC status=implemented
isp_context* IspCreate()
{
	// return _IspCreate();
	isp_context* isp = (isp_context *)malloc(sizeof(isp_context));
	if (isp == NULL) {
		fprintf(stderr, "[recedar] malloc isp context error\n");
		return NULL;
	}
	memset(isp, 0, sizeof(isp_context));
	return isp;
}

// -RC status=implemented
isp_context* SetIspBaseAddress(isp_context *isp, int baseAddress)
{
	// return _SetIspBaseAddress(isp, baseAddress);
	isp->baseAddress = baseAddress;
	return isp;
}