# What is this?

This example reads raw YUV422 frames and encode it to H264 file. (Uses cedar userspace library)

# Build instructions

1. Set up build environment (check root README.md)
2. Place this source code somewhere (can be in `packages/apps/encoder`)
3. Write `m`
4. You will find binary at `out/target/product/crane-cdr/obj/EXECUTABLES/encoder_intermediates`

# Usage

Firstly, use `ipctest` to generate raw frames, and then run `encoder`. It will generate `1080p.264` file.