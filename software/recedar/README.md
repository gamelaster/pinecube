# reCedar

reCedar is open-source implementation of library CedarX, which is used for hardware accelerated H.264 encoding.

The plan is to reverse engineer and implement all functions (and all their functions which those functions are calling), which are used by [this example encoding app](https://github.com/gamelaster/pinecube/tree/main/software/encoder). I started once again from scratch, because I was writting Terrenc as standalone app, and it would require a lot of functions to be implemented to get any result, and it would probably have errors inside, so it would be hard to debug it. Instead, reCedar is shared library, which exports same functions as `libvencoder.so`, and thanks to `LD_PRELOAD` thing, I can force `encoder` app to use reCedar's functions instead of original `libvencoder.so`, but still have ability to call original functions, so it's way easier to test the implementation if it works correctly. Also I made some mistakes when I was working on my IDA Pro IDB, so I started reverse engineering the blob from scratch too.

# State of project

<!-- BEGIN STATE AREA -->
There is at the moment 31 functions found, where 6 functions are implemented (0.19%)

| Function | Status | Notes |
|----------|:-----:|-------|
| AddOneInputBuffer | 🔀 Hooked (Work in Progress) |  |
| AllocInputBuffer | 🔀 Hooked (Work in Progress) |  |
| AlreadyUsedInputBuffer | 🔀 Hooked (Work in Progress) |  |
| EncAdapterGetICVersion |  |  |
| EncAdapterInitialize |  |  |
| EncAdapterLockVideoEngine |  |  |
| EncAdapterUnLockVideoEngine |  |  |
| FlushCacheAllocInputBuffer | 🔀 Hooked (Work in Progress) |  |
| FrameBufferManagerCreate |  |  |
| FreeOneBitStreamFrame | 🔀 Hooked (Work in Progress) |  |
| GetOneAllocInputBuffer | 🔀 Hooked (Work in Progress) |  |
| GetOneBitstreamFrame | 🔀 Hooked (Work in Progress) |  |
| H264EncClose | 🔀 Hooked (Work in Progress) |  |
| H264EncEncode | 🔀 Hooked (Work in Progress) |  |
| H264EncOpen | ✅ Implemented |  |
| H264FreeOneBitstream | 🔀 Hooked (Work in Progress) |  |
| H264GetOneBitstream | 🔀 Hooked (Work in Progress) |  |
| H264GetParameter | 🔀 Hooked (Work in Progress) |  |
| H264Init | 🔀 Hooked (Work in Progress) |  |
| H264SetParameter | 🔀 Hooked (Work in Progress) |  |
| H264UnInit | 🔀 Hooked (Work in Progress) |  |
| H264ValidBitStreamFrameNum | 🔀 Hooked (Work in Progress) |  |
| ReturnOneAllocInputBuffer | 🔀 Hooked (Work in Progress) |  |
| VencoderDeviceCreate | ✅ Implemented |  |
| VideoEncCreate | ✅ Implemented |  |
| VideoEncGetParameter | 🔀 Hooked (Work in Progress) |  |
| VideoEncInit | ✅ Implemented |  |
| VideoEncSetParameter | ✅ Implemented |  |
| VideoEncodeOneFrame | 🔀 Hooked (Work in Progress) |  |
| h264_get_ve_capability | 🔀 Hooked (Work in Progress) |  |
| h264_init_default_param | ✅ Implemented |  |
<!-- END STATE AREA -->
