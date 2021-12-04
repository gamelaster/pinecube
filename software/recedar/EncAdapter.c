#include "EncAdapter.h"
#include "recedar_hooks.h"

// -RC status=hooked
int EncAdapterInitialize(void)
{
	return _EncAdapterInitialize();
}

// -RC status=hooked
unsigned int EncAdapterGetICVersion(void)
{
	return _EncAdapterGetICVersion();
}