#include "terrenc.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

int veDriverFd = -1; //gVeDriverFd
unsigned int veMemoryMap = 0;

void VeSetDramType()
{
	// In S3, there is 32-DDR2 or 16-DDR3
	// TODO: Change this to something normal when there will be VE register struct (or other type of memory map description)
	*(unsigned int*)veMemoryMap = (*(unsigned int*)veMemoryMap & 0xFFFCFFFF) | 0x20000;
	// TODO: Mutex
}

void VeReset(void) {
	ioctl(veDriverFd, IOCTL_RESET_VE, 0);
	VeSetDramType();
}

int VeInitialize(void) {
	int ret = 0;
	// TODO: Make VE initialization check.
	veDriverFd = open("/dev/cedar_dev", O_RDWR);
	if (veDriverFd < 0) {
		printf("Can't open /dev/cedar_dev\n");
		veDriverFd = -1;
		goto err;
	}
	ret = ioctl(veDriverFd, IOCTL_SET_REFCOUNT, 0);
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_SET_REFCOUNT failed\n");
		goto err;
	}

	ret = ioctl(veDriverFd, IOCTL_ENGINE_REQ, 0);
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_ENGINE_REQ failed\n");
		goto err;
	}

	/* ret = ioctl(veDriverFd, IOCTL_GET_ENV_INFO, 0); // This is no longer required due currect Cedar implementation uses ION.
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_GET_ENV_INFO failed\n");
		goto err;
	} */

	veMemoryMap = (unsigned int)mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, veDriverFd, veMemoryMap); // last arg should be just zero?
	VeReset();


	// /dev/ifm is kernel module which exposes area with some identification number, nothing more is known.
	int ifmFd = open("/dev/ifm", O_RDWR);
	if (ifmFd < 0) {
		printf("Can't read ifm\n");
		abort(); // Looks like it's very important
	}
	int id = -1;
	read(ifmFd, &id, 1);
	close(ifmFd);

	printf("IFM ID: %d\n", id);
	if (id > 0 && id != 3 && id != 7) {
		printf("Bad IFM\n");
		abort();
	}

	return 0;
err:
	return -1;
}

int EncAdapterInitialize(void) {
	if (VeInitialize() < 0) {
		return -1;
	}
	/*if (MemAdapterOpen() < 0) {
		return -1;
	}*/ // TODO:
	//a0 = SecureMemAdapterOpen();
	//SecureMemAdapterClose();

	return 0;
}

int VideoEncCreate(int type)
{
	if (EncAdapterInitialize() != 0) {
		fprintf(stderr, "Can't setup video engine.\n");
		return NULL;
	}
	printf("VE Version: %X\n", *(unsigned short*)(veMemoryMap + 0xF2));
	// 1681 VE version
	return 0;
}