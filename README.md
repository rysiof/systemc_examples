# simple systemc code for educational purposes

verified and tested on ubuntu for x64.

available examples:
- ``port_export`` - quick showcase of how and when to use ``sc_port`` and ``sc_export``. After analyzing example you can quickly realize that ``sc_port``/``sc_export`` is just more fancy way of providing callback functions. Why then use it? Simple answer is just: because it already has integration to systemc infrastructure. If ``sc_port``/``sc_export`` are not properly (see documentation of ``sc_port``, for example http://www.cecs.uci.edu/~doemer/risc/v021/html_oopsc/a00148.html) connected, we will get exception.

## preparing setup

1. download and unpack systemc from https://www.accellera.org/downloads/standards/systemc 
2. set ``SYSTEMC_PATH`` to path to systemc location
3. follow installation of systemc.
   > **ATTENTION**: when executing ``../configure`` make sure to add ``CXXFLAGS="-DSC_CPLUSPLUS=201703L -std=c++17"`` as we want to use C++17. Let's embrance new technologies!

## compiling source code

1. goto source code location
2. execute commands:
   - ``make port_export CFG=release PLATFORM=x64`` to compile source code
   - ``make clean_port_export CFG=release PLATFORM=x64`` to clean solution
   - ``make run_port_export CFG=release PLATFORM=x64`` to run compiled program
   - ``make all_port_export CFG=release PLATFORM=x64`` to clean, compile and run program

