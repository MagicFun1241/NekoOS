# NekoOS

> NekoOS is an OS, written in C and assembly language.

### Building

To build NekoOS, you need to have the following:
- `GCC Cross-Compiler` (For i686: https://wiki.osdev.org/GCC_Cross-Compiler | For arm: Follow the steps from GCC_Cross-Compiler to build your own cross compiler but use `export TARGET=arm-none-eabi`)
- `make`
- `xorriso`
- `grub`

Once you acquired all the necessary tools, you are ready to build the OS.
Building is simple and only requires a few steps.

```
$ git clone https://github.com/MWGuy/NekoOS.git
$ cd NekoOS
$ make init ARCH=(i686, arm)
$ make ARCH=(i686, arm) CROSSCOMPILER=your cross compiler
```

After that's done with no reported errors, you should have a `NekoOS.iso` file in the project directory.

