# ZSim Simultor with PIM and Debuggable IDE Support

Zsim is a Pin-based simulator that support the simulation of fast, simple, and accurate, with a focus on simulating memory hierarchies and large, heterogeneous systems. However, it leaks the support to real-time debug in the IDE. This project enables the real-time debug of ZSim in the IDE.


## Setup

Reuqired: Visual Studio 2018 or later, VisualGDB 5.1.6 or later, Xming (optional)

1. Build original ZSim in the Linux server.
2. Install VisualGDB and open Visual Studio.
3. In the Visual Studio, create new Linux Project and Choose "use GNU make".
4. Copy all files to the root of the project.
5. In VisualGDB Project Properties->Makefile settings, add Include directory:

> /path_to_pin/source/include/pin  
> /path_to_pin/source/include/pin/gen  
> /path_to_pin/extras/components/include  
> /path_to_pin/extras/xed-intel64/include  
> /tmp/VisualGDB/c/Projects/libzsim/libzsim

6. In VisualGDB Project Properties->Makefile settings, add Library directory:

> /usr/lib  
> /usr/lib/x86_64-linux-gnu  
> /path_to_pin/extras/xed-intel64/lib  
> /path_to_pin/intel64/lib  
> /path_to_pin/intel64/lib-ext  
> /usr/local/lib  

7. In VisualGDB Project Properties->Makefile settings, add Library names:

> config++ pin xed pindwarf elf dl rt hdf5 hdf5_hl

8. In VisualGDB Project Properties->Makefile settings, add CXXFLAGS:

> -g -O0 -std=c++0x -Wall -Wno-unknown-pragmas -fomit-frame-pointer -fno-stack-protector -MMD -DBIGARRAY_MULTIPLIER=1 -DUSING_XED -DTARGET_IA32E -DHOST_IA32E -Werror -fPIC -DTARGET_LINUX -DPIN_PATH="/path_to_pin/intel64/bin/pinbin" -DZSIM_PATH="/tmp/VisualGDB/c/Projects/libzsim/libzsim/Debug/libzsim.so" -DMT_SAFE_LOG

9. In VisualGDB Project Properties->Makefile settings, add LDFLAGS:

> -Wl,-gc-sections -Wl,--hash-style=sysv -Wl,-Bsymbolic -Wl,--version-script=/path_to_pin/source/include/pin/pintool.ver -shared

10. In VisualGDB Project Properties->Debug settings, change "Start GDB in the following mode" to "Attach to an existing instance". Then modify the Executable to "/path_to_pin/intel64/bin/pinbin". Check "Run debugger as root with sudo".

11. Enjoy!
