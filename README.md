## v-os
Open source C++ operating system. It's recommended you do not use/copy this code as it is terrible and purely a fun project for me. *Also mind the profanity throughout the code.*

# Building
Make sure `xorriso`, `G++` and `NASM` are installed before trying to build.
Run command `make` in your **Linux** terminal in the v-os directory to build the project.
The object files will be placed in `v-os/obj` and the .iso that can be loaded is placed in the `v-os` directory.
This will not work on Windows or OSX (unless you perform black magic?)

# Execution
I recommend using Oracle VirualBox as it emulates using a PC quite accurately (afaik), but in `Makefile` there is a commented out line to enable using Qemu (qemu-system-i386), if that is installed, which is good for quick testing.
