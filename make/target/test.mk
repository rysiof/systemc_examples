# MIT License
#
# Copyright (c) 2020 rysiof@gmail.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

include make/defs.mk

INCLUDE_DIRS = $(TOP)src/$(TARGET)
INCLUDE_DIRS += /home/mkusio/systemc-2.3.3/include
INCLUDE_DIRS += $(TOP)src/common/include
CFLAGS = -Wall -Werror -O3
CXXFLAGS = $(CFLAGS)
CPPFLAGS = $(CFLAGS)
CCFLAGS = $(CFLAGS)

LDFLAGS = -L /home/mkusio/systemc-2.3.3/lib-linux64 -Wl,-rpath /home/mkusio/systemc-2.3.3/lib-linux64 -lsystemc -lm

.PHONY: $(TARGET)
$(TARGET): print_program_info | $(OUTPUT_PATH)/program

$(OUTPUT_PATH)/program: $(OBJ) # Here you can add internal dependencies
	$(LD) $^ -o $@ $(LDFLAGS)
	@echo Build successfull. Output binary: $(OUTPUT_PATH)/program
