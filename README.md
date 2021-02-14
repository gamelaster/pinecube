# PineCube
Notes, software and everything related to PineCube (mostly Video Encoding stuff)

# Terrenc

You're probably looking for [Terrenc](https://github.com/gamelaster/pinecube/tree/main/software/terrenc), open-source encoder for Allwinner S3

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
