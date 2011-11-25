# Compiler and flags
CC = gcc
LD = gcc
CFLAGS := -D_FILE_OFFSET_BITS=64
LINKFLAGS := 

# Objects for sort
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
STORE = ../../bin/OZoneStore

# For all binary files
all: $(STORE)

$(STORE): $(OBJS)
	$(LD) $(CFLAGS) $(LINKFLAGS) $(OBJS) -o $(STORE)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS) $(OJBS:.o=.d) $(STORE)

