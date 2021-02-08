
# Terrenc

Terrenc (**Terr**ible **Enc**oder) is open-source implementation of video encoder for Allwinner SoCs.

# Why terrible?

It's bit terrible, but so bad yet so good. At the moment, this library uses old
cedar kernel module to expose VE registers to userspace, it resembles API of original cedar library,
so it doesn't fulfill Linux code style. Although, this is my way of reversing, developing and testing the
library, but in roadmap (which is below), there are tasks to get rid of those horrible things, which
will make the library less terrible, and final state would be implement all the code into mainline cedrus kernel module.

# Supported SoCs

- [x] S3, V3, V3S, S3L

# Current state

Can't do nothing much, more soon.

# Roadmap

- [ ] Get H.264 Video Encoding working on CAMdroid by using legacy cedar kernel module
- [ ] Get H.264 Video Encoding working on Mainline Linux by using updated cedar kernel module

And there are two choices:

- [ ] Convert this library to kernel module with userspace library for apps
- [ ] Port the code to cedrus mainline kernel module