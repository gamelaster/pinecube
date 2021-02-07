# What is this?

This example can record (.mp4) and preview (on PineCube LCD Screen) camera.
(This uses CAMdroids API (so basically it's low-level Android API for manipulating with camera and video engine),
but it uses Cedar userspace library, so it's wrapper.)

# Build instructions

1. Set up build environment (check root README.md)
2. Place this source code somewhere (can be in `packages/apps/camera_test`)
3. Write `m`
4. You will find binary at `out/target/product/crane-cdr/obj/EXECUTABLES/camera_test_intermediates`

# Usage

```
Usage: ./camera_test <-c camera source> <-r | -p>
           camera source:0 for csi, 1 for uvc, 2 for both
```