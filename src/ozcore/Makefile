# Compiler and flags
CXX = gcc
LD = gcc
CXXFLAGS := -D_FILE_OFFSET_BITS=64
LINKFLAGS := 

# Objects for sort
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
STORE = ../../bin/OZoneStore

# For all binary files
all: $(STORE)

$(STORE): $(OBJS)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS) -o $(STORE)

%.o:%.c
	$(CXX) $(CFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS) $(OJBS:.o=.d) $(STORE)

