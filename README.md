# simple systemc code for educational purposes

verified and tested on ubuntu for x64

## preparing setup

1. download and unpack systemc from https://www.accellera.org/downloads/standards/systemc 
2. set SYSTEMC_PATH to path to systemc location
3. follow installation of systemc

## compiling source code

1. goto source code location
2. execute commands:
   - ``make port_export CFG=release PLATFORM=x64`` to compile source code
   - ``make clean_port_export CFG=release PLATFORM=x64`` to clean solution
   - ``make run_port_export CFG=release PLATFORM=x64`` to run compiled program
   - ``make all_port_export CFG=release PLATFORM=x64`` to clean, compile and run program

