# This file is subject to the license agreement located in the file LICENSE
# and cannot be distributed without it. This notice cannot be
# removed or modified.

TARGET = main
SRC = main.c foo.c
ROSE_SRC = rose_main.c rose_foo.c
TOOL = ../../../bin/globals2db
ROSE_TARGET = rose_$(TARGET)
ROSE_OPT = -rose:skipfinalCompileStep

include $(CURDIR)/../../../../Makefile.def

all: run


$(ROSE_TARGET): $(SRC) $(TOOL)
	$(TOOL)  $(ROSE_OPT) $(CFLAGS) $(CC_INC) $(SRC) $(MPI_FLAGS) -o $(TARGET) > build.log
	@if test `grep -e "applications with MPI support" build.log | wc -l` -eq 1; \
		then echo "$(TOOL) PASSED!" $(OUTPUT) ; \
		else echo "$(TOOL) FAILED." $(OUTPUT) ; \
	fi


run: $(ROSE_TARGET)

dot:
	dotGenerator $(TARGET).c
	$(ZGRVIEW) -Pdot $(TARGET).c.dot &

clean:
	rm -f *~ ffwd.h $(TARGET) $(TARGET).log $(ROSE_TARGET) $(ROSE_SRC) *.o $(TARGET).c.dot build.log $(ROSE_TARGET).log



