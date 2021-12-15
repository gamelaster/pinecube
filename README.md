# WARNING: This is obsolete

I made the latest CedarC blob working on mainline kernel, thus my working environment doesn't depend on CamDroid and old BSP anymore. Still, you can find a lot of information about CamDroid here, and also I might re-use this repo later when I will reverse G2D or other driver.

# PineCube
Notes, software and everything related to PineCube (mostly Video Encoding stuff)

# reCedar

[reCedar](https://github.com/gamelaster/pinecube/tree/main/software/recedar) is effort of creating open-source H.264 encoder for Allwinner S3, by hooking original
CedarX library and step by step reverse engineering and reimplementing it's exported and internal functions.

# Terrenc

[Terrenc](https://github.com/gamelaster/pinecube/tree/main/software/terrenc) is abandoned effort of creating open-source encoder for Allwinner S3 and was suppresed by reCedar

# CamDroid notes
## Turning off testing software (dragonboard)
```sh
stop dragonboard
```
## Show IP address
```sh
netcfg
```
## Connect to CAMdroid through FTP

Firstly, copy `busybox-arm` and `startftp.sh` into SD Card. Then plug-in it to PineCube. Via TTL, go to `/mnt/extsd/` and run `startftp.sh`

## Build Environment Setup

```sh
source build/envsetup.sh
lunch # select [1] here
````

## Disable printk logging
(Sometimes there is division by zero in kernel, and it happens periodically so it's quite annoying)

```sh
echo 1 > /proc/sys/kernel/printk
```
