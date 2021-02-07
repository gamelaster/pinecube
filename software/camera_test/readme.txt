1. start demo
   ./cdr_test -r      //start recoder without previewing.
   ./cdr_test -p      //start preview without recording.
   ./cdr_test -r -p   //start preview along with recording.
   ./cdr_test -c 0 -p //start preview from csi camera
   ./cdr_test -c 1 -p //start preview from uvc camera
   ./cdr_test -c 2 -p //start preview from both csi camera and uvc camera
   
2.during previewing
  input character 'r' to start recorder
  input charactor 's' to stop recorder
  input charactor 'q' to exit demo
  press CTRL + C to interrupt demo
  input kill -9 <demo pid> to kill demo
  
3.you can change configs in file "CdrConfig.h"
  all supported camera parameters can be dumped by calling method dumpParameter of class CameraDemo.
  
  
