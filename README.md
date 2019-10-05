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


## Side notes
- This is not even nearly close to completion and has MANY different things to be done on it, so pull requests to update it are very welcome.
 - It is just a personal project with many broken things and pieces of code taken from good projects (sadly I forgot to reference them in the actual code) so don't use my code to do anything worthwhile.
 - There is no license on this code so don't worry about copying it and anybody getting annoyed, I would be honoured if somebody copied any code from this haha.
 - IRQ's are broken and I don't have enough operating system development knowledge to fix them, so the keyboard input code is awful beyond what you would expect. 
 - There is no tasking system (*yet*) so don't expect it to be calculating PI to the 1 billionth digit while drawing a pixel perfect replica of the Mona Lisa.
