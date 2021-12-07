# reCedar

reCedar is open-source implementation of library CedarX, which is used for hardware accelerated H.264 encoding.

The plan is to reverse engineer and implement all functions (and all their functions which those functions are calling), which are used by [this example encoding app](https://github.com/gamelaster/pinecube/tree/main/software/encoder). I started once again from scratch, because I was writting Terrenc as standalone app, and it would require a lot of functions to be implemented to get any result, and it would probably have errors inside, so it would be hard to debug it. Instead, reCedar is shared library, which exports same functions as `libvencoder.so`, and thanks to `LD_PRELOAD` thing, I can force `encoder` app to use reCedar's functions instead of original `libvencoder.so`, but still have ability to call original functions, so it's way easier to test the implementation if it works correctly. Also I made some mistakes when I was working on my IDA Pro IDB, so I started reverse engineering the blob from scratch too.

# State of project

<!-- BEGIN STATE AREA -->
There is at the moment 34 functions found, where 9 functions are implemented (0.26%)

Legend:

- 🔀 Hooked - Means that this function is hooked, so original function is called and needs to be implemented
- 🔁 Implementing - Feature is being implemented (with progress)
- 🕒 Not Needed Yet - Function is hooked, but it's not required to implement it now, because it's not important, or because it will be implemented differently from original function (but function should be Reverse Engineered already)
- ✅ Implemented - Function is implemented and working

| Function | Status | Notes |
|----------|:-----:|-------|
| AddOneInputBuffer | 🔀 Hooked |  |
| AllocInputBuffer | 🔀 Hooked |  |
| AlreadyUsedInputBuffer | 🔀 Hooked |  |
| EncAdapterGetICVersion | 🕒 Not Needed Yet |  |
| EncAdapterInitialize | 🕒 Not Needed Yet |  |
| EncAdapterLockVideoEngine | 🕒 Not Needed Yet |  |
| EncAdapterUnLockVideoEngine | 🕒 Not Needed Yet |  |
| EncAdapterVeGetBaseAddress | 🕒 Not Needed Yet |  |
| FlushCacheAllocInputBuffer | 🔀 Hooked |  |
| FrameBufferManagerCreate | 🕒 Not Needed Yet |  |
| FreeOneBitStreamFrame | 🔀 Hooked |  |
| GetOneAllocInputBuffer | 🔀 Hooked |  |
| GetOneBitstreamFrame | 🔀 Hooked |  |
| H264EncClose | 🔀 Hooked |  |
| H264EncEncode | 🔀 Hooked |  |
| H264EncOpen | ✅ Implemented |  |
| H264FreeOneBitstream | 🔀 Hooked |  |
| H264GetOneBitstream | 🔀 Hooked |  |
| H264GetParameter | 🔀 Hooked |  |
| H264Init | ✅ Implemented |  |
| H264InitMemory | 🔀 Hooked |  |
| H264SetParameter | 🔀 Hooked |  |
| H264UnInit | 🔀 Hooked |  |
| H264ValidBitStreamFrameNum | 🔀 Hooked |  |
| IspCreate | ✅ Implemented |  |
| ReturnOneAllocInputBuffer | 🔀 Hooked |  |
| SetIspBaseAddress | ✅ Implemented |  |
| VencoderDeviceCreate | ✅ Implemented |  |
| VideoEncCreate | ✅ Implemented |  |
| VideoEncGetParameter | 🔀 Hooked |  |
| VideoEncInit | ✅ Implemented |  |
| VideoEncSetParameter | ✅ Implemented |  |
| VideoEncodeOneFrame | 🔀 Hooked |  |
| h264_check_capability | 🔀 Hooked |  |
| h264_get_ve_capability | 🔀 Hooked |  |
| h264_init_Poc | 🔀 Hooked |  |
| h264_init_default_param | ✅ Implemented |  |
| h264_init_rc_quene | 🔀 Hooked |  |
| h264_init_regInfo | 🔀 Hooked |  |
| h264_init_sps_pps | 🔀 Hooked |  |
<!-- END STATE AREA -->
