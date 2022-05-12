# RPi-Network-Communication-TCP-UDP-in-C
Raspberry pi as TCP/UDP server and laptop as TCP/UDP client. We stream live video from RPi camera to a client.

## TCP part
Use
```console
server_TCP.c
```
on the RPi and
```console
client_TCP.c
```
on your laptop or other machine.

### Steps to run TCP stream
First run this command to make a fifo file.
```console
mkfifo video_test.h264
```
On RPi run (adjust H and W to your screen):
```console
raspivid -w 1280 -h 1024 -t 0 -o video_test.h264
```

Open another shell and run:
```console
./server_TCP
```

On your machine run:
```console
mkfifo video
```

Then:
```console
./client_TCP
```

Then run:
gst-launch-1.0 -v filesrc location=test.h264 ! h264parse ! avdec_h264
! autovideosink sync=false
```console
gst-launch-1.0 -v filesrc location=video ! h264parse ! avdec_h264 ! autovideosink sync=false
```
You should see live TCP stream on your display.
