#Compiler and flags
CC = gcc
LD = gcc
CXXFLAGS := -O3 -D_FILE_OFFSET_BITS=64
LINKFLAGS :=  
#Objects
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
#Generate binary name
TEST = ../bin/TestOzoneStore
 
all: $(TEST)
 
$(TEST): $(OBJS)
	$(LD) $(OBJS) $(LINKFLAGS) -o $(TEST)
 
%.o:%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<
 
.PHONY: all clean
clean:
	rm -rf $(OBJS) $(OJBS:.o=.d) $(TEST)
