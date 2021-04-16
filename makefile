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

# This makefile is only command wrapper for build.mk

CFG ?= release
PLATFORM ?= x86
QUIET ?=

.PHONY: all_%
all_%:
	-@make -s clean_$(@:all_%=%) -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)
	-@make $(@:all_%=%) -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)
	-@make run_$(@:all_%=%) QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)

.PHONY: rebuild_%
rebuild_%:
	@make -s clean_$(@:rebuild_%=%) -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)
	@make $(@:rebuild_%=%) -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)

.PHONY: clean_%
clean_%:
	@make -s $@ -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM)


.PHONY: run_%
run_%:
	@./bin/$(@:run_%=%)/$(PLATFORM)/$(CFG)/program

.PHONY: %
%:
	make $@ -f build.mk QUIET=$(QUIET) CFG=$(CFG) PLATFORM=$(PLATFORM) -j 16