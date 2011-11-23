# Compiler and flags
CXX = gcc
LD = gcc
CXXFLAGS := -D_FILE_OFFSET_BITS=64
LINKFLAGS := 

# Objects for sort
OBJS_SORT := $(patsubst %.c, %.o, $(wildcard *.c))
SORT = ../../bin/OZoneSort 

# For all binary files
all: $(SORT)

$(SORT): $(OBJS_SORT)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS_SORT) -o $(SORT)

%.o:%.c
	$(CXX) $(CFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS_SORT) $(OJBS_SORT:.o=.d) $(SORT)

