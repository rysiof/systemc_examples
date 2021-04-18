# simple systemc code for educational purposes

verified and tested on ubuntu for x64.

available examples:
- ``port_export`` - quick showcase of how and when to use ``sc_port`` and ``sc_export``. After analyzing example you can quickly realize that ``sc_port``/``sc_export`` is just more fancy way of providing callback functions. Why then use it? Simple answer is just: because it already has integration to systemc infrastructure. If ``sc_port``/``sc_export`` are not properly connected (see documentation of ``sc_port`` [here](http://www.cecs.uci.edu/~doemer/risc/v021/html_oopsc/a00148.html) for example), we will get exception.

## preparing setup

1. download and unpack systemc from https://www.accellera.org/downloads/standards/systemc to LOCATION
2. set ``SYSTEMC_PATH`` to path to ``LOCATION``
   ```bash
   export SYSTEMC_PATH=LOCATION
   ```
   for example:
   ```bash
   export SYSTEMC_PATH=/home/myuser/systemc-2.3.3
   ```
   recomendataion is to set as global variable. example for ubuntu, base on [this](https://help.ubuntu.com/community/EnvironmentVariables#A.2Fetc.2Fprofile.d.2F.2A.sh):
   ```bash
   echo export SYSTEMC_PATH=/home/myuser/systemc-2.3.3 > ~/temp.sh
   sudo cp ~/temp.sh /etc/profile.d/systemc_init.sh
   rm ~/temp.sh
   ```
3. goto ``LOCATION``
4. execute sequence
   ```bash
    mkdir objdir
    cd objdir
    ../configure CXXFLAGS="-DSC_CPLUSPLUS=201703L -std=c++17"
    make 
    make install
    cd ..
    rm -rf objdir
   ```
   > **ATTENTION**: when executing ``../configure`` make sure to add ``CXXFLAGS="-DSC_CPLUSPLUS=201703L -std=c++17"`` as we want to use C++17. Let's embrance new technologies!

## compiling source code

1. goto source code location
2. execute commands:
   - ``make port_export CFG=release PLATFORM=x64`` to compile source code
   - ``make clean_port_export CFG=release PLATFORM=x64`` to clean solution
   - ``make run_port_export CFG=release PLATFORM=x64`` to run compiled program
   - ``make all_port_export CFG=release PLATFORM=x64`` to clean, compile and run program

