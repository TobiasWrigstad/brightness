# brightness

Simple stupid "script" to enable me to control brightness on my linux machine (Dell XPS 13).
(xbacklight stopped working after I updated the kernel to 4.8.6.)

## Usage

Assuming `brightness.c` is compiled into an executable called `tb` in the path:

```
# Get the current brightness value for the screen
> tb --get
# Set the screen brightness to a value V between 1 and 937
> tb --set V
# Increase or decrease brightness by a value V 
> tb --inc V
> tb --dec V
# Set the brightness to 0 (black screen)
> tb --BLACK
# Get help
> tb --help
```
