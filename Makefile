# Usage:
#   make
#   ./traffic.x 60 0     # 60 seconds, random mode
#   ./traffic.x 60 1     # 60 seconds, targeted mode
#
# The final executable
PROGRAM = traffic.x

# The source files (*.cc)
SRCS = TrafficController.cc TrafficGenerator.cc Monitor.cc main_testbench.cc

##################################
## DO NOT CHANGE ANYTHING BELOW ##
##################################

SYSTEMC = /courses/TDTS07/sw/systemc
TARGET_ARCH = linux64

LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
LIBS = -Wl,-Bstatic -lsystemc -Wl,-Bdynamic -lm -lpthread
LDFLAGS = -g

CC = g++
OPT = -O3
CFLAGS = -std=c++14 -Wno-deprecated
EXE = $(PROGRAM)

INCDIR= -I. -I.. -I$(SYSTEMC)/include

OBJS = $(SRCS:.cc=.o)

.SUFFIXES: .cc .o .x

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(INCDIR) $(LIBDIR) -o $(EXE) $(OBJS) $(LIBS)

# To build the whole project we must compile each .cc file into a .o file, and
# then link all .o-files into an executable. The rule above tells the make tool
# how to link the object files into an executable. To create the object files we
# need additional rules (one for each .cc-file). For each .cc-file we add two
# lines in the make file. The first line consists of a list of one .cc file
# (corresponding to the .o-file beeing created) and possibly a list of .h-files.
# Whenever any of the files in the list has changed, the make tool will
# recompile the object file. The list should be the .cc-file beeing compiled,
# and all the .h-files that are included in that .cc-file.

main_testbench.o: main_testbench.cc traffic.h
	$(CC) -c $(CFLAGS) $(INCDIR) main_testbench.cc -o main_testbench.o

TrafficController.o: TrafficController.cc traffic.h
	$(CC) -c $(CFLAGS) $(INCDIR) TrafficController.cc -o TrafficController.o

TrafficGenerator.o: TrafficGenerator.cc traffic.h
	$(CC) -c $(CFLAGS) $(INCDIR) TrafficGenerator.cc -o TrafficGenerator.o

Monitor.o: Monitor.cc traffic.h
	$(CC) -c $(CFLAGS) $(INCDIR) Monitor.cc -o Monitor.o

clean:
	rm -f $(OBJS) $(EXE) *~

