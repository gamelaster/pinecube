#include "EncAdapter.h"
#include "recedar_hooks.h"

// -RC status=not-needed-yet
int EncAdapterInitialize(void)
{
	return _EncAdapterInitialize();
}

// -RC status=not-needed-yet
unsigned int EncAdapterGetICVersion(void)
{
	return _EncAdapterGetICVersion();
}

// -RC status=not-needed-yet
int EncAdapterLockVideoEngine(void)
{
	return _EncAdapterLockVideoEngine();
}

// -RC status=not-needed-yet
void EncAdapterUnLockVideoEngine(void)
{
	return _EncAdapterUnLockVideoEngine();
}