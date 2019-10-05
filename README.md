# v-os
Open source C++ operating system. It's recommended you do not use/copy this code as it is terrible and purely a fun project for me. *Also mind the profanity throughout the code.*
This is purely a hobby OS and thus cannot do anything actually useful so don't expect to be hacking into NASA's servers with this!
Type `help` into the command line to see a list of commands. Be wary that some may lock up the system or not work.
v-os does not require any toolchain to be installed, but in the `Building` section there  list of the programs required to build this OS.

## Building
Make sure `xorriso`, `G++`, `grub-mkrescue` and `NASM` are installed before trying to build.
Run command `make` in your **Linux** terminal in the v-os directory to build the project.
The object files will be placed in `v-os/obj` and the .iso that can be loaded is placed in the `v-os` directory.
This will not work on Windows or OSX (unless you perform black magic?)

## Execution
I recommend using Oracle VirualBox as it emulates using a PC quite accurately (afaik), but in `Makefile` there is a commented out line to enable using Qemu (qemu-system-i386), if that is installed, which is good for quick testing.

## Screenshots
![](https://i.imgur.com/Rf6c1bi.png)
