# PineCube
Notes, software and everything related to PineCube (mostly Video Encoding stuff)

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